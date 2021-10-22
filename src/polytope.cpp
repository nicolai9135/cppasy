#include "polytope.hpp"

#include <iostream>

int polytope::get_depth()
{
    return this->depth;
}

void polytope::set_depth(int d)
{
    this->depth = depth;
}

std::vector<coordinate> polytope::get_safe_coordinates()
{
    return polytope::safe_coordinates;
}

std::vector<coordinate> polytope::get_unsafe_coordinates()
{
    return polytope::unsafe_coordinates;
}

