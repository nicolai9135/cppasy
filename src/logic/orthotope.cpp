#include "polytope.hpp"
#include "orthotope.hpp"
#include <iostream>
#include <algorithm>
#include <bitset>

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

std::deque<std::unique_ptr<polytope>> orthotope::split_sub(splitting_heuristic splitting_h)
{
    cut_list cuts;
    switch (splitting_h)
    {
    case splitting_heuristic::bisect_all:
        cuts = split_bisect_all();
        break;
    default:
        return {};
        break;
    }
    return generate_orthotopes(cuts);
}

std::deque<std::unique_ptr<polytope>> orthotope::generate_orthotopes(cut_list cuts)
{
    // initialize new boundaries with the old ones
    std::vector<intervals> new_boundaries;
    for(const auto &b : boundaries)
    {
        new_boundaries.push_back({b});
    }

    // iterate over the cuts to cut the specified intervals
    for(const auto &cut : cuts)
    {
        unsigned int dimension = cut.first;
        interval front = { boundaries[dimension].first, cut.second };
        interval back = { cut.second, boundaries[dimension].second };
        new_boundaries[dimension] = {front, back};
    }

    // build cartesian product over the new boundaries to get list of all new
    // orthotope boundaries
    std::vector<intervals> new_boundaries_complete = cartesian_product(new_boundaries);

    std::deque<std::unique_ptr<polytope>> new_orthopes;
    for (const auto &new_boundary : new_boundaries_complete)
    {
        new_orthopes.push_back(std::unique_ptr<polytope>(new orthotope(new_boundary, this->depth + 1)));
    }

    return new_orthopes;
}

cut_list orthotope::split_bisect_all()
{
    cut_list cuts;
    // bisect all intervals
    for(unsigned int dim; dim < boundaries.size(); dim++)
    {
        z3::expr mid = (boundaries[dim].first + boundaries[dim].second)/2;
        cuts.push_back({dim, mid.simplify()});
    }
    return cuts;
}

void orthotope::cartesian_recursion(std::vector<intervals> &accum, intervals stack, std::vector<intervals> sequences, long unsigned int index)
{
    intervals sequence = sequences[index];
    for (interval i : sequence)
    {
        stack.push_back(i);
        if (index == 0)
            accum.push_back(stack);
        else
            cartesian_recursion(accum, stack, sequences, index - 1);
        stack.pop_back();
    }
}

std::vector<intervals> orthotope::cartesian_product(std::vector<intervals> sequences)
{
    // the cartesian product function takes the product the other way around...
    std::reverse(sequences.begin(), sequences.end());

    std::vector<intervals> accum;
    intervals stack;
    if (sequences.size() > 0)
        cartesian_recursion(accum, stack, sequences, sequences.size() - 1);
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