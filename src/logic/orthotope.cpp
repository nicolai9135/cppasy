#include "polytope.hpp"
#include "orthotope.hpp"
#include <iostream>
#include <algorithm>

orthotope::orthotope(intervals bs, unsigned int d, std::vector<coordinate> sc, std::vector<coordinate> uc)
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

void orthotope::print_sub()
{
    std::cout << "Printing Orthotope" << std::endl;

    // print depth
    std::cout << "    Depth: " << depth << std::endl; 
    
    // print boundaries
    std::cout << "    Boundaries:" << std::endl;
    int i = 0;

    for(const auto &it : boundaries)
    {
        i++;
        std::cout << "        Dimension " << i << ":   " << "[" << it.first << ", " << it.second << "]" << std::endl;
    }

    // print safe samples

    // print unsafe samples
}

z3::expr_vector orthotope::get_boundaries_z3_sub(z3::context &ctx, z3::expr_vector &variable_names)
{
    z3::expr_vector res(ctx);
    for (unsigned int i = 0; i < variable_names.size(); i++)
    {
        // WARNING: conversion from unsigned int to int dangerous if #vars to big!!!
        res.push_back(variable_names[(int) i] >= this->get_boundaries()[i].first);
        res.push_back(variable_names[(int) i] <= this->get_boundaries()[i].second);
    }
    return res;
}


std::deque<std::unique_ptr<polytope>> orthotope::intervals_to_orthotopes(std::vector<intervals> intervals_list)
{
    // TODO: go through samples and add them to the resp. polytope
    std::deque<std::unique_ptr<polytope>> new_orthopes;
    for (const auto &it : intervals_list)
    {
        new_orthopes.push_back(std::unique_ptr<polytope>(new orthotope(it, this->depth + 1)));
    }

    return new_orthopes;
}

std::deque<std::unique_ptr<polytope>> orthotope::split_bisect_all()
{
    // bisect all intervals
    std::vector<std::pair<interval, interval>> intervals_bisected = bisect_all_intervals(this->boundaries);

    // list of intervals for new orthotopes
    std::vector<intervals> new_intervals_list = cartesian_product(intervals_bisected);

    return intervals_to_orthotopes(new_intervals_list);
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

void orthotope::cartesian_recursion(std::vector<intervals> &accum, intervals stack, std::vector<std::pair<interval, interval>>  intervals_bisected, long unsigned int index)
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
    // unfortunately the cartesian product function I use does the product the other way around...
    std::reverse(intervals_bisected.begin(), intervals_bisected.end());
    std::vector<intervals> accum;

    intervals stack;

    if (intervals_bisected.size() > 0)
        cartesian_recursion(accum, stack, intervals_bisected, intervals_bisected.size() - 1);
    // for(std::vector<intervals>::iterator it = accum.begin(); it != accum.end(); ++it)
    //     std::reverse(it->begin(), it->end());
    return accum;
}

void orthotope::draw_wxWidgets_sub(wxDC *dc, axis x_axis, axis y_axis)
{
    double x_begin_scaled = boundaries[x_axis.index].first.as_double() * x_axis.scalar;
    double x_end_scaled = boundaries[x_axis.index].second.as_double() * x_axis.scalar;
    double y_begin_scaled = boundaries[y_axis.index].first.as_double() * y_axis.scalar;
    double y_end_scaled = boundaries[y_axis.index].second.as_double() * y_axis.scalar;

    const wxRealPoint top_left_real = wxRealPoint(x_begin_scaled, -y_end_scaled);
    const wxRealPoint bottom_right_real = wxRealPoint(x_end_scaled, -y_begin_scaled);

    const wxPoint top_left = wxPoint(top_left_real);
    const wxPoint bottom_right = wxPoint(bottom_right_real);

    const wxRect rectangle = wxRect(top_left, bottom_right);

    dc->DrawRectangle(rectangle);
}