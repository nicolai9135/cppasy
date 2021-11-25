#include "parameter_synthesis.hpp"

void synthesis::print_deque(std::deque<std::unique_ptr<polytope>> &my_deque)
{
    for (const auto &p : my_deque)
    {
        p->print();
    }
}

synthesis::synthesis(options o)
  : formula(ctx),
    variable_names(ctx),
    solver_pos(ctx),
    solver_neg(ctx),
    max_depth(o.max_depth)
{
    for(auto const &variable_name : o.variable_names)
    {
        variable_names.push_back(ctx.real_const(variable_name.c_str()));
    }

    intervals boundaries;
    // transform initial string-intervals from options into z3-intervals
    for(const auto &interval_string : o.initial_intervals)
    {
        interval current = {ctx.real_val(std::get<1>(interval_string).c_str()), ctx.real_val(std::get<2>(interval_string).c_str())};
        boundaries.push_back(current);
    }
    
    // use initial boundaries to create first (unknown) orthotope
    synthesis_areas.unknown_areas.push_back(std::unique_ptr<polytope>(new orthotope(boundaries, 1)));

    // read formula and transform vector into expression
    if (o.formula_as_file)
    {
        z3::expr_vector formula_vector = ctx.parse_file(o.formula_str.c_str());
        formula = mk_and(formula_vector);
    }
    else
    {
        z3::expr_vector formula_vector = ctx.parse_string(o.formula_str.c_str());
        formula = mk_and(formula_vector);
    }

    // initialize solvers
    solver_pos.add(formula);
    solver_neg.add(!formula);
}

void synthesis::print_all_areas()
{
    // print queues
    std::cout << "SAFE areas" << std::endl;
    std::cout << "==========" << std::endl;
    print_deque(synthesis_areas.safe_areas);

    std::cout << "UNSAFE areas" << std::endl;
    std::cout << "============" << std::endl;
    print_deque(synthesis_areas.unsafe_areas);

    std::cout << "UNKNOWN areas" << std::endl;
    std::cout << "=============" << std::endl;
    print_deque(synthesis_areas.unknown_areas);
}

void synthesis::execute()
{
    // create stamps for incremental solving
    solver_pos.push();
    solver_neg.push();

    // main loop
    while (!synthesis_areas.unknown_areas.empty())
    {
        // get first polytope in queue
        std::unique_ptr<polytope>& current_polytope = synthesis_areas.unknown_areas.front();

        // end calculation if maximal depth is reached
        if (current_polytope->get_depth() >= max_depth) break;

        // get boundaries of the polytope in Z3 format
        z3::expr_vector boundaries_z3 = current_polytope->get_boundaries_z3(ctx, variable_names);

        // add boundaries of current polytope to pos solver
        solver_pos.add(boundaries_z3);

        if (solver_pos.check())
        {
            // add boundaries of current box to neg solver
            solver_neg.add(boundaries_z3);
            if (solver_neg.check())
            {
                // split current polytope
                std::deque<std::unique_ptr<polytope>> new_polytopes = current_polytope->split(bisect_all);

                // append new areas to unknown areas
                synthesis_areas.unknown_areas.insert(synthesis_areas.unknown_areas.end(), std::make_move_iterator(new_polytopes.begin()), std::make_move_iterator(new_polytopes.end()));
            }
            else
            {
                synthesis_areas.safe_areas.push_back(std::move(current_polytope));
            }
            solver_neg.pop();
            solver_neg.push();
        }
        else
        {
            synthesis_areas.unsafe_areas.push_back(std::move(current_polytope));
        }
        solver_pos.pop();
        solver_pos.push();
        
        // delete processed element
        synthesis_areas.unknown_areas.pop_front();
    }
}

void synthesis::continue_synthesis(unsigned int increment)
{
    max_depth += increment;
    execute();
}

areas *synthesis::get_synthesis_areas_ptr()
{
    return &synthesis_areas;
}

z3::expr_vector synthesis::get_variable_names()
{
    return variable_names;
}

void options::sanity_check_intervals()
{
    z3::context ctx_sanity;

    for(const auto &ii : initial_intervals)
    {
        std::string name = std::get<0>(ii);
        std::string lower = std::get<1>(ii);
        z3::expr z3_lower(ctx_sanity);
        std::string upper = std::get<2>(ii);
        z3::expr z3_upper(ctx_sanity);

        try
        {
            z3_lower = ctx_sanity.real_val(lower.c_str());
            z3_upper = ctx_sanity.real_val(upper.c_str());
            // get_sort() throws error in case a non-convertible string was specified
            z3_lower.get_sort();
            z3_upper.get_sort();
        }
        catch(...)
        {
            throw not_a_number();
        }

        if ( (lower=="") || (upper=="") )
        {
            throw boundary_missing();
        }
        
        // check whether lower bound <= upper bound
        z3::expr comparison = (z3_lower <= z3_upper);
        z3::solver s_sanity(ctx_sanity);
        s_sanity.add(comparison);
        if (!s_sanity.check())
        {
            throw not_an_interval();
        }
    }
}