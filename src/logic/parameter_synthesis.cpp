#include "parameter_synthesis.hpp"
#include "polytope.hpp"
#include "orthotope.hpp"

void synthesis::print_deque(std::deque<std::unique_ptr<polytope>> &my_deque)
{
    for (const auto &p : my_deque)
    {
        p->print();
    }
}

synthesis::synthesis(options o)
  : formula(ctx)
  , formula_neg(ctx)
  , variable_names(ctx)
  , solver_pos(ctx)
  , solver_neg(ctx)
  , total_volume(ctx)
  , max_depth(o.max_depth)
  , use_save_model(o.use_save_model)
  , use_split_samples(o.use_split_samples)
  , use_execute_2in1(o.use_execute_2in1)
  , sampling_h(o.sampling_h)
  , splitting_h(o.splitting_h)
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
    
    synthesis_areas.unknown_areas.push_back(std::unique_ptr<polytope>(new orthotope(boundaries, 0, this)));

    // get total volume of the inital polytope
    total_volume = synthesis_areas.unknown_areas.back()->get_volume();
    total_volume = total_volume.simplify();

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
    // formula_neg = (!formula);

    // initialize solvers
    solver_pos.add(formula);
    solver_neg.add(!formula);

    // do initial check to make use of learned lemmas (-> incremental solving)
    solver_pos.check();
    solver_neg.check();

    // create stamps for incremental solving
    solver_pos.push();
    solver_neg.push();

    bitmasks_flipped = generate_bitmasks(splitting_h);
    bitmasks = flip_bitmasks(bitmasks_flipped);

#ifndef SAFE
    std::cout << "WARNING: SAFETY DEACTIVATED! THIS MAY LEAD TO INCOORECT OUTPUT! Only deactivate this feature if you know what you are doing. To enable safty, recompile with setting the 'SAFE'-compile-definition" << std::endl << std::endl;
#endif
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
    if(use_execute_2in1)
    {
        execute_2in1();
    }
    else
    {
        execute_default();
    }
}
void synthesis::execute_default()
{
    std::cout << "using dafault" << std::endl;

    // main loop
    while (!synthesis_areas.unknown_areas.empty())
    {
        // get first polytope in queue
        std::unique_ptr<polytope>& current_polytope = synthesis_areas.unknown_areas.front();

        // end calculation if maximal depth is reached
        if (current_polytope->get_depth() >= max_depth) break;

        // do sampling
        current_polytope->sample();

        // prepare solver
        z3::expr_vector boundaries_z3 = current_polytope->get_boundaries_z3();
        solver_pos.add(boundaries_z3);

        // check whether there is a unsafe coordinate whithin the current polytope
        bool safe_coordinate_exists = current_polytope->coordinate_exists(safe);

        if (safe_coordinate_exists)
        {
            // add boundaries of current box to neg solver
            solver_neg.add(boundaries_z3);

            // check whether there is a safe coordinate whithin the current polytope
            bool unsafe_coordinate_exists = current_polytope->coordinate_exists(unsafe);

            if (unsafe_coordinate_exists)
            {
                // split current polytope
                std::deque<std::unique_ptr<polytope>> new_polytopes = current_polytope->split();

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

void synthesis::execute_2in1()
{
    unsigned int current_depth;
    std::cout << "using 2in1" << std::endl;

    // main loop
    while (!synthesis_areas.unknown_areas.empty())
    {
        // get first polytope in queue
        std::unique_ptr<polytope>& current_polytope = synthesis_areas.unknown_areas.front();
        current_depth = current_polytope->get_depth();

        // end calculation if maximal depth is reached
        if (current_depth >= max_depth) break;

        // extra pop necessary when switching from odds to evens (note that an even ot odd switch is impossible because evens are always inserted at the end)
        if (current_depth%2 == 0)
        {
            solver_neg.pop();
            solver_pos.pop();
        }

        // do sampling
        current_polytope->sample();

        // prepare solver
        z3::expr_vector boundaries_z3 = current_polytope->get_boundaries_z3();
        solver_pos.push();
        solver_pos.add(boundaries_z3);
        solver_neg.push();
        solver_neg.add(boundaries_z3);

        // check whether there is a unsafe coordinate whithin the current polytope
        bool safe_coordinate_exists = current_polytope->coordinate_exists(safe);

        if (safe_coordinate_exists)
        {
            // add boundaries of current box to neg solver

            // check whether there is a safe coordinate whithin the current polytope
            bool unsafe_coordinate_exists = current_polytope->coordinate_exists(unsafe);

            if (unsafe_coordinate_exists)
            {
                // split current polytope
                std::deque<std::unique_ptr<polytope>> new_polytopes = current_polytope->split();

                // append new areas to unknown areas
                if (current_depth%3 == 0)
                {
                    // delete processed element
                    synthesis_areas.unknown_areas.pop_front();
                    synthesis_areas.unknown_areas.insert(synthesis_areas.unknown_areas.begin(), std::make_move_iterator(new_polytopes.begin()), std::make_move_iterator(new_polytopes.end()));
                }
                else
                {
                    // delete processed element
                    synthesis_areas.unknown_areas.pop_front();
                    synthesis_areas.unknown_areas.insert(synthesis_areas.unknown_areas.end(), std::make_move_iterator(new_polytopes.begin()), std::make_move_iterator(new_polytopes.end()));
                }
            }
            else
            {
                synthesis_areas.safe_areas.push_back(std::move(current_polytope));
                // delete processed element
                synthesis_areas.unknown_areas.pop_front();
            }
        }
        else
        {
            synthesis_areas.unsafe_areas.push_back(std::move(current_polytope));
            // delete processed element
            synthesis_areas.unknown_areas.pop_front();
        }
        if (!(current_depth%3 == 0))
        {
            solver_neg.pop();
            solver_pos.pop();
        }
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

// z3::expr_vector synthesis::get_variable_names()
// {
//     return variable_names;
// }

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

std::vector<boost::dynamic_bitset<>> synthesis::flip_bitmasks(std::vector<boost::dynamic_bitset<>> &bitmasks)
{
    std::vector<boost::dynamic_bitset<>> res = bitmasks;
    for(auto &mask : res)
    {
        mask.flip();
    }
    return res;
}

std::vector<boost::dynamic_bitset<>> synthesis::generate_bitmasks(splitting_heuristic splitting_h)
{
    unsigned int cut_count;

    switch (splitting_h)
    {
    case splitting_heuristic::bisect_all:
        cut_count = variable_names.size();
        break;
    case splitting_heuristic::bisect_single:
        cut_count = 1;
        break;
    default:
        throw no_split_samples_support();
        break;
    }

    // Magic is happening here...
    unsigned int new_orthotopes_count = pow(2, cut_count);
    std::vector<boost::dynamic_bitset<>> bitmasks(cut_count, boost::dynamic_bitset<>(new_orthotopes_count));

    for(long unsigned int i = 0; i < new_orthotopes_count; i++)
    {
        boost::dynamic_bitset<> current(cut_count, i);
        for(long unsigned int j = 0; j < cut_count; j++)
        {
            bitmasks[j][i] = current[j];
        }
    }
    std::reverse(bitmasks.begin(), bitmasks.end());

    return bitmasks;
}

double synthesis::get_area_percentage(std::deque<std::unique_ptr<polytope>> &my_deque)
{
    // get area of polytopes in deque
    z3::expr sum = ctx.real_val("0");
    for (const auto &p : my_deque)
    {
        sum = sum + p->get_volume();
    }
    z3::expr area_percentage = sum / total_volume;
    area_percentage = area_percentage.simplify();
    return area_percentage.as_double();
}

void synthesis::print_percentages()
{
    std::cout << std::endl;
    std::cout <<   "    Areas" << std::endl;
    std::cout <<   "    -----" << std::endl;
    std::cout <<   "      Unknown:  " << get_area_percentage(synthesis_areas.unknown_areas) << "%" << std::endl;
    std::cout <<   "      Safe:     " << get_area_percentage(synthesis_areas.safe_areas) << "%" << std::endl;
    std::cout <<   "      Unsafe:   " << get_area_percentage(synthesis_areas.unsafe_areas) << "%" << std::endl;
}

z3::expr& synthesis::get_formula()
{
    return formula;
}

z3::context& synthesis::get_ctx()
{
    return ctx;
}

const z3::expr_vector& synthesis::get_variable_names()
{
    return variable_names;
}

z3::solver& synthesis::get_solver_pos()
{
    return solver_pos;
}
z3::solver& synthesis::get_solver_neg()
{
    return solver_neg;
}

bool synthesis::get_use_save_model()
{
    return use_save_model;
}

bool synthesis::get_use_split_samples()
{
    return use_split_samples;
}

sampling_heuristic synthesis::get_sampling_h()
{
    return sampling_h;
}

splitting_heuristic synthesis::get_splitting_h()
{
    return splitting_h;
}

const std::vector<boost::dynamic_bitset<>>& synthesis::get_bitmasks()
{
    return bitmasks;
}

const std::vector<boost::dynamic_bitset<>>& synthesis::get_bitmasks_flipped()
{
    return bitmasks_flipped;
}