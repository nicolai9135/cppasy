#include "polytope.hpp"

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

z3::expr_vector polytope::get_boundaries_z3(z3::context &ctx, z3::expr_vector &variable_names)
{
    return get_boundaries_z3_sub(ctx, variable_names);
}

void polytope::print()
{
    print_sub();
}

void polytope::draw_wxWidgets(wxDC *dc, axis x_axis, axis y_axis)
{
    draw_wxWidgets_sub(dc, x_axis, y_axis);
}

std::deque<std::unique_ptr<polytope>> polytope::split(splitting_heuristic splitting_h, bool use_split_samples)
{
    return split_sub(splitting_h, use_split_samples);
}

void polytope::sample(sampling_heuristic sampling_h, z3::context &ctx, z3::expr &formula, z3::expr_vector &variable_names, splitting_heuristic splitting_h, bool use_split_samples)
{
#if EVAL > 0
    auto sampling_time_begin = std::chrono::steady_clock::now();
#endif
    sample_sub(sampling_h, ctx, formula, variable_names, splitting_h, use_split_samples);
#if EVAL > 0
    auto sampling_time_end = std::chrono::steady_clock::now();
    eval->sampling_time += (sampling_time_end - sampling_time_begin);
#endif
}

void polytope::save_model(z3::model m, area_class ac, z3::expr_vector &variable_names)
{
    coordinate c;
    for (const auto &var : variable_names)
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

bool polytope::coordinate_exists(z3::solver &s, area_class ac, bool use_save_model, z3::expr_vector &variable_names)
{
    bool no_sample_exists;
    bool no_clever_sample_exists;
    if(ac == area_class::safe)
    {
        no_sample_exists = safe_coordinates.empty();
        no_clever_sample_exists = !has_safe_sample;
    }
    else
    {
        no_sample_exists = unsafe_coordinates.empty();
        no_clever_sample_exists = !has_unsafe_sample;
    }

    bool solver_result = false;

    // use solver if no positive coordinates available
    if (no_clever_sample_exists && no_sample_exists)
    {
#if EVAL > 0
        eval->solver_count++;
        auto solving_time_begin = std::chrono::steady_clock::now();
#endif
        solver_result = s.check();
#if EVAL > 0
        auto solving_time_end = std::chrono::steady_clock::now();
        eval->solving_time += (solving_time_end - solving_time_begin);
        auto model_saving_time_begin = std::chrono::steady_clock::now();
#endif
        if(use_save_model)
        {
            if(solver_result)
            {
                save_model(s.get_model(), ac, variable_names);
            }
        }
#if EVAL > 0
        auto model_saving_time_end = std::chrono::steady_clock::now();
        eval->model_saving_time += (model_saving_time_end - model_saving_time_begin);
        if(!solver_result)
        {
            eval->solver_count_necessary++;
        }
#endif
    }
#if EVAL > 0
    else
    {
        eval->skip_solver_count++;
    }
#endif

    return (!no_clever_sample_exists || !no_sample_exists || solver_result);
}

z3::expr polytope::get_volume(z3::context &ctx)
{
    return get_volume_sub(ctx);
}