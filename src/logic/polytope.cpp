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


std::deque<std::unique_ptr<polytope>> polytope::split(splitting_heuristic splitting_h)
{
    return split_sub(splitting_h);
}

void polytope::sample(sampling_heuristic sampling_h, z3::context &ctx, z3::expr &formula, z3::expr_vector &variable_names)
{
    sample_sub(sampling_h, ctx, formula, variable_names);
}

void polytope::save_model(z3::model m, bool is_safe, z3::expr_vector &variable_names)
{
    coordinate c;
    for (const auto &var : variable_names)
    {
        c.push_back(m.get_const_interp(var.decl()));
    }
    if (is_safe)
    {
        safe_coordinates.push_back(c);
    }
    else
    {
        unsafe_coordinates.push_back(c);
    }
}