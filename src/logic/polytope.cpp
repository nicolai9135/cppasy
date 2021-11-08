#include "polytope.hpp"

#include <iostream>

int polytope::get_depth()
{
    return this->depth;
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
    return this->get_boundaries_z3_sub(ctx, variable_names);
}

void polytope::print()
{
    this->print_sub();
}

std::deque<std::unique_ptr<polytope>> polytope::split(splitting_heuristic splitting_h)
{
    switch (splitting_h)
    {
    case splitting_heuristic::bisect_all:
        return this->split_bisect_all();
        break;
    default:
        return {};
        break;
    }
}

void polytope::sample(sampling_heuristic sampling_h)
{
    switch (sampling_h)
    {
    case sampling_heuristic::vertices:
        break;
    default:
        break;
    }
}