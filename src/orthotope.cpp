#include "polytope.hpp"
#include "orthotope.hpp"
#include <iostream>
#include <algorithm>

orthotope::orthotope(intervals bs, int d, std::vector<coordinate> sc, std::vector<coordinate> uc)
{
    boundaries = bs;
    depth = d;
    safe_coordinates = sc;
    unsafe_coordinates = uc;
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

std::deque<std::unique_ptr<polytope>> orthotope::split(splitting_heuristic splitting_h)
{
    std::vector<intervals> new_intervals_list;
    std::vector<std::pair<interval, interval>> intervals_bisected;

    switch (splitting_h)
    {
    case splitting_heuristic::bisect_all:
        intervals_bisected = bisect_all_intervals(this->boundaries);
        new_intervals_list = cartesian_product(intervals_bisected);
        break;
    // case splitting_heuristic::bisect_single:
        // break;
    default:
        break;
    }

    std::deque<std::unique_ptr<polytope>> new_orthopes;

    for (std::vector<intervals>::iterator it = new_intervals_list.begin(); it != new_intervals_list.end(); it++)
    {
        new_orthopes.push_back(std::unique_ptr<polytope>(new orthotope(*it, this->depth + 1)));
    }

    return new_orthopes;
}

void orthotope::sample(sampling_heuristic sampling_h)
{

}

std::vector<std::pair<interval, interval>> orthotope::bisect_all_intervals(intervals intervals_in)
{
    std::vector<std::pair<interval, interval>> intervals_bisected;
    for(intervals::iterator it = intervals_in.begin(); it != intervals_in.end(); ++it)
    {
        z3::expr mid = (it->second + it->first)/2;
        z3::expr mid_simplified = mid.simplify();
        interval front = { it->first, mid_simplified };
        interval back = { mid_simplified, it->second };
        intervals_bisected.push_back({front, back});
    }
    return intervals_bisected;
}

void orthotope::cartesian_recursion(std::vector<intervals> &accum, intervals stack, std::vector<std::pair<interval, interval>>  intervals_bisected, int index)
{
    std::pair<interval, interval> interval_bisected = intervals_bisected[index];

    // for first element
    stack.push_back(interval_bisected.first);
    if (index == 0)
        accum.push_back(stack);
    else
        cartesian_recursion(accum, stack, intervals_bisected, index - 1);
    stack.pop_back();

    // for second element
    stack.push_back(interval_bisected.second);
    if (index == 0)
    {
        accum.push_back(stack);
    }
    else
        cartesian_recursion(accum, stack, intervals_bisected, index - 1);
    stack.pop_back();
}

std::vector<intervals> orthotope::cartesian_product(std::vector<std::pair<interval, interval>> intervals_bisected)
{
    std::vector<intervals> accum;

    intervals stack;

    if (intervals_bisected.size() > 0)
        cartesian_recursion(accum, stack, intervals_bisected, intervals_bisected.size() - 1);
    for(std::vector<intervals>::iterator it = accum.begin(); it != accum.end(); ++it)
        std::reverse(it->begin(), it->end());
    return accum;
}