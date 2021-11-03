#include "parameter_synthesis.hpp"

void print_deque(std::deque<std::unique_ptr<polytope>> &my_deque)
{
    for (const auto &p : my_deque)
    {
        p->print();
    }
}

synthesis_queues parameter_synthesis(options user_input, z3::context &ctx)
{
    synthesis_queues sq;

    // init z3 "framework"
    
    // TODO: handle user_input.variable_boundaries
    z3::expr_vector variable_names(ctx);
    variable_names.push_back(ctx.real_const("x"));
    variable_names.push_back(ctx.real_const("y"));
    intervals boundaries = {{ctx.real_val(0),ctx.real_val(2)}, {ctx.real_val(0),ctx.real_val(2)}};

    // read formula
    z3::expr_vector formula_vector = ctx.parse_file(user_input.formula_file.c_str());

    // transform vector into expression
    z3::expr formula = mk_and(formula_vector);

    // create solvers for checking (negated) formula
    z3::solver solver_pos(ctx);
    solver_pos.add(formula);
    solver_pos.push();
    z3::solver solver_neg(ctx);
    solver_neg.add(!formula);
    solver_neg.push();

    // initial depth: 1
    sq.unknown_areas.push_back(std::unique_ptr<polytope>(new orthotope(boundaries, 1)));

    // main loop
    while (!sq.unknown_areas.empty())
    {
        // get first polytope in queue
        std::unique_ptr<polytope>& current_polytope = sq.unknown_areas.front();

        // end calculation if maximal depth is reached
        if (current_polytope->get_depth() >= user_input.max_depth) break;

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
                std::deque<std::unique_ptr<polytope>> new_polytopes = current_polytope->split(bisect_all);
                sq.unknown_areas.insert(sq.unknown_areas.end(), std::make_move_iterator(new_polytopes.begin()), std::make_move_iterator(new_polytopes.end()));
            }
            else
            {
                sq.safe_areas.push_back(std::move(current_polytope));
            }
            solver_neg.pop();
            solver_neg.push();
        }
        else
        {
            sq.unsafe_areas.push_back(std::move(current_polytope));
        }
        solver_pos.pop();
        solver_pos.push();
        
        // delete processed element
        sq.unknown_areas.pop_front();
    }

    return sq;
}