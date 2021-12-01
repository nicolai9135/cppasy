#include "polytope.hpp"
#include "orthotope.hpp"
#include <iostream>
#include <algorithm>
#include <cmath>

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
        break;
    }
    return generate_orthotopes(cuts);
}

std::deque<std::unique_ptr<polytope>> orthotope::generate_orthotopes(cut_list cuts)
{
    // generate list of new boundaries through cutting current intervals
    std::vector<intervals> new_boundaries = generate_new_boundaries(cuts);

    std::vector<boost::dynamic_bitset<>> bitmasks = generate_bitmasks(cuts.size());
    std::vector<boost::dynamic_bitset<>> bitmasks_flipped = flip_bitmasks(bitmasks);
    /*
    for(const auto &mask : bitmasks)
    {
        std::cout << mask << std::endl;
    }
    for(const auto &mask : bitmasks_flipped)
    {
        std::cout << mask << std::endl;
    }
    */
    
    std::vector<std::vector<coordinate>> new_safe_coordinates = split_coordinates(cuts, bitmasks, bitmasks_flipped, safe_coordinates);
    std::vector<std::vector<coordinate>> new_unsafe_coordinates = split_coordinates(cuts, bitmasks, bitmasks_flipped, unsafe_coordinates);


    // generate new orthotopes
    std::deque<std::unique_ptr<polytope>> new_orthopes;
    for (const auto &new_boundary : new_boundaries)
    {
        new_orthopes.push_back(std::unique_ptr<polytope>(new orthotope(new_boundary, this->depth + 1)));
    }

    return new_orthopes;
}

std::vector<std::vector<coordinate>> orthotope::split_coordinates(cut_list &cuts, std::vector<boost::dynamic_bitset<>> &bitmasks, std::vector<boost::dynamic_bitset<>> &bitmasks_flipped, std::vector<coordinate> &coordinates)
{
    unsigned int cut_count = cuts.size();
    unsigned int new_orthotopes_count = pow(2, cuts.size());
    std::vector<std::vector<coordinate>> res(new_orthotopes_count);

    for(const auto &coord : coordinates)
    {
        // orthotope_membership[i] indicats whether coord is within the
        // boundaries of the ith new orthotope 
        boost::dynamic_bitset<> orthotope_membership(new_orthotopes_count);
        orthotope_membership.set();

        for (unsigned int current_cut; current_cut<cut_count; current_cut++)
        {
            z3::expr comp_ss = (coord[cuts[current_cut].first] < cuts[current_cut].second);
            z3::expr comp_eq = (coord[cuts[current_cut].first] == cuts[current_cut].second);
            z3::expr simple_comp_ss = comp_ss.simplify();
            z3::expr simple_comp_eq = comp_eq.simplify();

            // sanity check simplification:
            z3::expr comp_ss_neg = (!comp_ss);
            z3::expr comp_eq_neg = (!comp_eq);
            z3::expr simple_comp_ss_neg = comp_ss_neg.simplify();
            z3::expr simple_comp_eq_neg = comp_eq_neg.simplify();
            if ((simple_comp_ss.is_true() == simple_comp_ss_neg.is_true()) || (simple_comp_eq.is_true() == simple_comp_eq_neg.is_true()))
            {
                std::cout << "simplify did not work on this coordinate :(. Implement solver handling here!" << std::endl;
            }
            
            // actual comparison
            if (simple_comp_ss)
            {
                orthotope_membership = (orthotope_membership & bitmasks[current_cut]);
            }
            else if (!simple_comp_eq)
            {
                
                orthotope_membership = (orthotope_membership & bitmasks_flipped[current_cut]);
            }
        }
    }
    return {};
}


std::vector<boost::dynamic_bitset<>> orthotope::flip_bitmasks(std::vector<boost::dynamic_bitset<>> &bitmasks)
{
    std::vector<boost::dynamic_bitset<>> res = bitmasks;
    for(auto &mask : res)
    {
        mask.flip();
    }
    return res;
}

std::vector<boost::dynamic_bitset<>> orthotope::generate_bitmasks(unsigned int cut_count)
{
    // Magic is happening here...
    unsigned int new_orthotopes_count = pow(2, cut_count);
    std::vector<boost::dynamic_bitset<>> bitmasks(cut_count, boost::dynamic_bitset<>(new_orthotopes_count));

    for(int i = 0; i < new_orthotopes_count; i++)
    {
        boost::dynamic_bitset<> current(cut_count, i);
        for(int j = 0; j < cut_count; j++)
        {
            bitmasks[j][i] = current[j];
        }
    }

    std::reverse(bitmasks.begin(), bitmasks.end());
    return bitmasks;
}

std::vector<intervals> orthotope::generate_new_boundaries(cut_list cuts)
{
    // initialize new boundaries with the old ones
    std::vector<intervals> new_boundaries_per_dimension;
    for(const auto &b : boundaries)
    {
        new_boundaries_per_dimension.push_back({b});
    }

    // make cuts in the specified dimensions
    for(const auto &cut : cuts)
    {
        unsigned int dimension = cut.first;
        interval front = { boundaries[dimension].first, cut.second };
        interval back = { cut.second, boundaries[dimension].second };
        new_boundaries_per_dimension[dimension] = {front, back};
    }

    // build cartesian product over the new boundaries to get list of all new
    // orthotope boundaries
    return cartesian_product(new_boundaries_per_dimension);
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