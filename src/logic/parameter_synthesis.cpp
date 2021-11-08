#include "parameter_synthesis.hpp"

void synthesis::print_deque(std::deque<std::unique_ptr<polytope>> &my_deque)
{
    for (const auto &p : my_deque)
    {
        p->print();
    }
}

synthesis::synthesis(options o)
{
    this->synthesis_options = o;
}

void synthesis::print_all_areas()
{
    // print queues
    std::cout << "SAFE areas" << std::endl;
    std::cout << "==========" << std::endl;
    print_deque(this->safe_areas);

    std::cout << "UNSAFE areas" << std::endl;
    std::cout << "============" << std::endl;
    print_deque(this->unsafe_areas);

    std::cout << "UNKNOWN areas" << std::endl;
    std::cout << "=============" << std::endl;
    print_deque(this->unknown_areas);
}

void synthesis::execute()
{
    // TODO: handle user_input.variable_boundaries
    z3::expr_vector variable_names(this->ctx);
    variable_names.push_back(this->ctx.real_const("x"));
    variable_names.push_back(this->ctx.real_const("y"));
    intervals boundaries = {{this->ctx.real_val(0),this->ctx.real_val(2)}, {this->ctx.real_val(0),this->ctx.real_val(2)}};

    // read formula
    z3::expr_vector formula_vector = this->ctx.parse_file(synthesis_options.formula_file.c_str());

    // transform vector into expression
    z3::expr formula = mk_and(formula_vector);

    // create solvers for checking (negated) formula
    z3::solver solver_pos(this->ctx);
    solver_pos.add(formula);
    solver_pos.push();
    z3::solver solver_neg(this->ctx);
    solver_neg.add(!formula);
    solver_neg.push();

    // initial depth: 1
    this->unknown_areas.push_back(std::unique_ptr<polytope>(new orthotope(boundaries, 1)));

    // main loop
    while (!this->unknown_areas.empty())
    {
        // get first polytope in queue
        std::unique_ptr<polytope>& current_polytope = this->unknown_areas.front();

        // end calculation if maximal depth is reached
        if (current_polytope->get_depth() >= synthesis_options.max_depth) break;

        // get boundaries of the polytope in Z3 format
        z3::expr_vector boundaries_z3 = current_polytope->get_boundaries_z3(this->ctx, variable_names);

        // add boundaries of current polytope to pos solver
        solver_pos.add(boundaries_z3);

        if (solver_pos.check())
        {
            // add boundaries of current box to neg solver
            solver_neg.add(boundaries_z3);
            if (solver_neg.check())
            {
                std::deque<std::unique_ptr<polytope>> new_polytopes = current_polytope->split(bisect_all);
                this->unknown_areas.insert(this->unknown_areas.end(), std::make_move_iterator(new_polytopes.begin()), std::make_move_iterator(new_polytopes.end()));
            }
            else
            {
                this->safe_areas.push_back(std::move(current_polytope));
            }
            solver_neg.pop();
            solver_neg.push();
        }
        else
        {
            this->unsafe_areas.push_back(std::move(current_polytope));
        }
        solver_pos.pop();
        solver_pos.push();
        
        // delete processed element
        this->unknown_areas.pop_front();
    }
}