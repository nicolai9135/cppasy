/**
 * @file    main.cpp
 */

#include "polytope.hpp"
#include "orthotope.hpp"

// only for developement purposes
void print_and_delete(std::deque<std::unique_ptr<polytope>> &my_deque)
{
    while (!my_deque.empty())
    {
        my_deque.front()->print();
        my_deque.pop_front();
    }
}


int main()
{
    // TODO: make input dependent
    const int max_depth = 4;

    // init z3 "framework"

    // create global context
    z3::context ctx;

    
    // TODO: do this generic and input_dependent
    z3::expr_vector variable_names(ctx);
    variable_names.push_back(ctx.real_const("x"));
    variable_names.push_back(ctx.real_const("y"));


    // read formula
    z3::expr_vector formula_vector = ctx.parse_file("smt2_input/example.smt2");

    // transform vector into expression
    z3::expr formula = mk_and(formula_vector);

    // create solvers for checking (negated) formula
    z3::solver solver_pos(ctx);
    solver_pos.add(formula);
    solver_pos.push();

    z3::solver solver_neg(ctx);
    solver_neg.add(!formula);
    solver_neg.push();

    // TODO: only for developement purposes, remove later and read this through a file.
    intervals boundaries = {{ctx.real_val(0),ctx.real_val(2)}, {ctx.real_val(0),ctx.real_val(2)}};

    // queues for safe, unsafe and unknown areas
    std::deque<std::unique_ptr<polytope>> safe_areas;
    std::deque<std::unique_ptr<polytope>> unsafe_areas;
    std::deque<std::unique_ptr<polytope>> unknown_areas;

    // TODO: make generic (initial depth:1)
    unknown_areas.push_back(std::unique_ptr<polytope>(new orthotope(boundaries, 1)));


    // main loop
    while (!unknown_areas.empty())
    {
        // get first polytope in queue
        std::unique_ptr<polytope>& current_polytope = unknown_areas.front();

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
                std::deque<std::unique_ptr<polytope>> new_polytopes = current_polytope->split(bisect_all);
                unknown_areas.insert(unknown_areas.end(), std::make_move_iterator(new_polytopes.begin()), std::make_move_iterator(new_polytopes.end()));
            }
            else
            {
                safe_areas.push_back(std::move(current_polytope));
            }
            solver_neg.pop();
            solver_neg.push();
        }
        else
        {
            unsafe_areas.push_back(std::move(current_polytope));
        }
        solver_pos.pop();
        solver_pos.push();
        
        // delete processed element
        unknown_areas.pop_front();
    }

    // print queues
    std::cout << "SAFE areas" << std::endl;
    std::cout << "==========" << std::endl;
    print_and_delete(safe_areas);

    std::cout << "UNSAFE areas" << std::endl;
    std::cout << "============" << std::endl;
    print_and_delete(unsafe_areas);

    std::cout << "UNKNOWN areas" << std::endl;
    std::cout << "=============" << std::endl;
    print_and_delete(unknown_areas);

    return 0;
}