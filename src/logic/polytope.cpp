#include "polytope.hpp"
#include "parameter_synthesis.hpp"

#include <iostream>

unsigned int polytope::get_depth()
{
    return depth;
}

std::vector<coordinate> polytope::get_safe_coordinates()
{
    return polytope::safe_coordinates;
}

std::vector<coordinate> polytope::get_unsafe_coordinates()
{
    return polytope::unsafe_coordinates;
}

z3::expr_vector polytope::get_boundaries_z3()
{
    return get_boundaries_z3_sub();
}

void polytope::print()
{
    print_sub();
}

std::deque<std::unique_ptr<polytope>> polytope::split()
{
    return split_sub();
}

void polytope::sample()
{
#if EVAL > 0
    auto sampling_time_begin = std::chrono::steady_clock::now();
#endif
    sample_sub();
#if EVAL > 0
    auto sampling_time_end = std::chrono::steady_clock::now();
    s->eval.sampling_time += (sampling_time_end - sampling_time_begin);
#endif
}

void polytope::save_model(const z3::model &m, area_class ac)
{
    coordinate c;
    for (const auto &var : s->get_variable_names())
    {
        c.push_back(m.get_const_interp(var.decl()));
    }
    if (ac == area_class::safe)
    {
        safe_coordinates.push_back(c);
    }
    else
    {
        unsafe_coordinates.push_back(c);
    }
}

bool polytope::coordinate_exists(area_class ac)
{
    z3::solver *solv;
    bool no_sample_exists;
    bool no_clever_sample_exists;
    if(ac == area_class::safe)
    {
        solv = &s->get_solver_pos();
        no_sample_exists = safe_coordinates.empty();
        no_clever_sample_exists = !has_safe_sample;
    }
    else
    {
        solv = &s->get_solver_neg();
        no_sample_exists = unsafe_coordinates.empty();
        no_clever_sample_exists = !has_unsafe_sample;
    }

    bool solver_result = false;

    // use solver if no positive coordinates available
    if (no_clever_sample_exists && no_sample_exists)
    {
#if EVAL > 0
        s->eval.solver_count++;
        auto solving_time_begin = std::chrono::steady_clock::now();
#endif
        solver_result = solv->check();
#if EVAL > 0
        auto solving_time_end = std::chrono::steady_clock::now();
        s->eval.solving_time += (solving_time_end - solving_time_begin);
        auto model_saving_time_begin = std::chrono::steady_clock::now();
#endif
        if(s->get_use_save_model())
        {
            if(solver_result)
            {
                save_model(solv->get_model(), ac);
            }
        }
#if EVAL > 0
        auto model_saving_time_end = std::chrono::steady_clock::now();
        s->eval.model_saving_time += (model_saving_time_end - model_saving_time_begin);
        if(!solver_result)
        {
            s->eval.solver_count_necessary++;
        }
#endif
    }
#if EVAL > 0
    else
    {
        s->eval.skip_solver_count++;
    }
#endif

    return (!no_clever_sample_exists || !no_sample_exists || solver_result);
}

z3::expr polytope::get_volume()
{
    return get_volume_sub();
}