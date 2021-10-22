#include "global.hpp"
#include "polytope.hpp"

#include <iostream>

int polytope::get_depth()
{
    return this->depth;
}

void polytope::set_depth(int new_depth)
{
    this->depth = new_depth;
}

std::vector<coordinate> polytope::get_safe_coordinates()
{
    return polytope::safe_coordinates;
}

std::vector<coordinate> polytope::get_unsafe_coordinates()
{
    return polytope::unsafe_coordinates;
}

orthotope::orthotope(intervals boundaries_in, int depth_in, std::vector<coordinate> safe_coordinates_in, std::vector<coordinate> unsafe_coordinates_in)
{
    depth = depth_in;
    boundaries = boundaries_in;
    safe_coordinates = safe_coordinates_in;
    unsafe_coordinates = unsafe_coordinates_in;
}

intervals orthotope::get_boundaries()
{
    return boundaries;
}

void orthotope::print()
{
    std::cout << "Printing Orthotope" << std::endl;

    // print depth
    std::cout << "    Depth: " << depth << std::endl; 
    
    // print boundaries
    std::cout << "    Boundaries:" << std::endl;
    int i = 0;
    for(intervals::iterator it = boundaries.begin(); it != boundaries.end(); ++it)
    {
        i++;
        std::cout << "        Dimension " << i << ":   " << "[" << it->first << ", " << it->second << "]" << std::endl;
    }

    // print safe samples

    // print unsafe samples
}

z3::expr_vector orthotope::get_boundaries_z3(z3::context &ctx, z3::expr_vector &variable_names)
{
    z3::expr_vector res(ctx);
    for (int i = 0; i < variable_names.size(); i++)
    {
        res.push_back(variable_names[i] >= this->get_boundaries()[i].first);
        res.push_back(variable_names[i] <= this->get_boundaries()[i].second);
    }
    return res;
}