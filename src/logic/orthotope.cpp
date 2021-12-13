#include "polytope.hpp"
#include "orthotope.hpp"
#include <iostream>
#include <algorithm>
#include <cmath>

orthotope::orthotope(intervals bs, unsigned int d, evaluation *eval_synthesis, bool has_s_s, bool has_u_s, std::vector<coordinate> sc, std::vector<coordinate> uc)
{
    boundaries = bs;
    depth = d;
    eval = eval_synthesis;
    has_safe_sample = has_s_s;
    has_unsafe_sample = has_u_s;
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
    std::cout << "    Safe Coordinates:" << std::endl;
    for(const auto &coord : safe_coordinates)
    {
        std::cout << "        [";
        for (const auto &val : coord)
        {
            std::cout << val << ", ";
        }
        std::cout << "]" << std::endl;
    }

    // print unsafe samples
    std::cout << "    Unsafe Coordinates:" << std::endl;
    for(const auto &coord : unsafe_coordinates)
    {
        std::cout << "        [";
        for (const auto &val : coord)
        {
            std::cout << val << ", ";
        }
        std::cout << "]" << std::endl;
    }
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

std::deque<std::unique_ptr<polytope>> orthotope::split_sub(splitting_heuristic splitting_h, bool use_split_samples, std::vector<boost::dynamic_bitset<>> &bitmasks, std::vector<boost::dynamic_bitset<>> &bitmasks_flipped)
{
#if EVAL > 0
    auto polytope_splitting_cut_time_begin = std::chrono::steady_clock::now();
#endif
    cut_list cuts;
    switch (splitting_h)
    {
    case splitting_heuristic::bisect_all:
        cuts = split_bisect_all();
        break;
    case splitting_heuristic::bisect_single:
        cuts = split_bisect_single();
        break;
    default:
        break;
    }
#if EVAL > 0
    auto polytope_splitting_cut_time_end = std::chrono::steady_clock::now();
    eval->polytope_splitting_cut_time += (polytope_splitting_cut_time_end - polytope_splitting_cut_time_begin);
#endif
    return generate_orthotopes(cuts, use_split_samples, bitmasks, bitmasks_flipped);
}

std::deque<std::unique_ptr<polytope>> orthotope::generate_orthotopes(cut_list cuts, bool use_split_samples, std::vector<boost::dynamic_bitset<>> &bitmasks, std::vector<boost::dynamic_bitset<>> &bitmasks_flipped)
{
#if EVAL > 0
    auto polytope_splitting_generate_time_begin = std::chrono::steady_clock::now();
#endif
#if EVAL > 1
    auto polytope_splitting_generate_boundaries_time_begin = std::chrono::steady_clock::now();
#endif
    // generate list of new boundaries through cutting current intervals
    std::vector<intervals> new_boundaries = generate_new_boundaries(cuts);
#if EVAL > 1
    auto polytope_splitting_generate_boundaries_time_end = std::chrono::steady_clock::now();
    eval->polytope_splitting_generate_boundaries_time += (polytope_splitting_generate_boundaries_time_end - polytope_splitting_generate_boundaries_time_begin);
#endif
    std::deque<std::unique_ptr<polytope>> new_orthotopes;

    if(use_split_samples)
    {
#if EVAL > 1
        auto polytope_splitting_generate_sc_time_begin = std::chrono::steady_clock::now();
#endif
        std::vector<std::vector<coordinate>> new_safe_coordinates = split_coordinates(cuts, bitmasks, bitmasks_flipped, safe_coordinates);
        std::vector<std::vector<coordinate>> new_unsafe_coordinates = split_coordinates(cuts, bitmasks, bitmasks_flipped, unsafe_coordinates);
#if EVAL > 1
        auto polytope_splitting_generate_sc_time_end = std::chrono::steady_clock::now();
        eval->polytope_splitting_generate_sc_time += (polytope_splitting_generate_sc_time_end - polytope_splitting_generate_sc_time_begin);
#endif
#if EVAL > 1
        auto polytope_splitting_generate_newq_time_begin = std::chrono::steady_clock::now();
#endif
        for (unsigned int new_orthotope_index = 0; new_orthotope_index<new_boundaries.size(); new_orthotope_index++)
        {
            new_orthotopes.push_back(std::unique_ptr<polytope>(new orthotope(new_boundaries[new_orthotope_index], depth + 1, eval, has_safe_sample, has_unsafe_sample, new_safe_coordinates[new_orthotope_index], new_unsafe_coordinates[new_orthotope_index])));
        }
#if EVAL > 1
        auto polytope_splitting_generate_newq_time_end = std::chrono::steady_clock::now();
        eval->polytope_splitting_generate_newq_time += (polytope_splitting_generate_newq_time_end - polytope_splitting_generate_newq_time_begin);
#endif
    }
    else
    {
#if EVAL > 1
        auto polytope_splitting_generate_newq_time_begin = std::chrono::steady_clock::now();
#endif
        for (unsigned int new_orthotope_index = 0; new_orthotope_index<new_boundaries.size(); new_orthotope_index++)
        {
            new_orthotopes.push_back(std::unique_ptr<polytope>(new orthotope(new_boundaries[new_orthotope_index], depth + 1, eval, has_safe_sample, has_unsafe_sample)));
        }
#if EVAL > 1
        auto polytope_splitting_generate_newq_time_end = std::chrono::steady_clock::now();
        eval->polytope_splitting_generate_newq_time += (polytope_splitting_generate_newq_time_end - polytope_splitting_generate_newq_time_begin);
#endif
    }

#if EVAL > 0
    auto polytope_splitting_generate_time_end = std::chrono::steady_clock::now();
    eval->polytope_splitting_generate_time += (polytope_splitting_generate_time_end - polytope_splitting_generate_time_begin);
#endif
    return new_orthotopes;
}

std::vector<std::vector<coordinate>> orthotope::split_coordinates(cut_list &cuts, std::vector<boost::dynamic_bitset<>> &bitmasks, std::vector<boost::dynamic_bitset<>> &bitmasks_flipped, std::vector<coordinate> &coordinates)
{
    unsigned int cut_count = cuts.size();
    unsigned int new_orthotopes_count = pow(2, cuts.size());
    std::vector<std::vector<coordinate>> res(new_orthotopes_count);

    // iterate over given list of coordinates
    for(const auto &coord : coordinates)
    {

        // orthotope_membership[i] indicats whether coord is within the
        // boundaries of the ith new orthotope 
        boost::dynamic_bitset<> orthotope_membership(new_orthotopes_count);

        // initially assume coordinate is in all orthotopes
        orthotope_membership.set();

        // iterate through cuts and filter out unsuitable orthotopes
        for (unsigned int current_cut = 0; current_cut<cut_count; current_cut++)
        {
#if EVAL > 1
            auto polytope_splitting_generate_sc_simp_time_begin = std::chrono::steady_clock::now();
#endif
            z3::expr comp_ss = (coord[cuts[current_cut].first] < cuts[current_cut].second);
            z3::expr comp_eq = (coord[cuts[current_cut].first] == cuts[current_cut].second);
            z3::expr simple_comp_ss = comp_ss.simplify();
            z3::expr simple_comp_eq = comp_eq.simplify();
#if EVAL > 1
            auto polytope_splitting_generate_sc_simp_time_end = std::chrono::steady_clock::now();
            eval->polytope_splitting_generate_sc_simp_time += (polytope_splitting_generate_sc_simp_time_end - polytope_splitting_generate_sc_simp_time_begin);
            eval->simplify_split_count += 2;
#endif
#ifdef SAFE
#if EVAL > 1
            auto polytope_splitting_generate_sc_sanity_time_begin = std::chrono::steady_clock::now();
#endif
            // sanity check simplification:
            z3::expr comp_ss_neg = (!comp_ss);
            z3::expr comp_eq_neg = (!comp_eq);
            z3::expr simple_comp_ss_neg = comp_ss_neg.simplify();
            z3::expr simple_comp_eq_neg = comp_eq_neg.simplify();
            if ((simple_comp_ss.is_true() == simple_comp_ss_neg.is_true()) || (simple_comp_eq.is_true() == simple_comp_eq_neg.is_true()))
            {
                throw simplification_impossible();
            }
#if EVAL > 1
            auto polytope_splitting_generate_sc_sanity_time_end = std::chrono::steady_clock::now();
            eval->polytope_splitting_generate_sc_sanity_time += (polytope_splitting_generate_sc_sanity_time_end - polytope_splitting_generate_sc_sanity_time_begin);
#endif
#endif
#if EVAL > 1
            auto polytope_splitting_generate_sc_comp_time_begin = std::chrono::steady_clock::now();
#endif
            // actual comparison to filter unsuitable orthotopes
            if (simple_comp_ss.is_true())
            {
                orthotope_membership = (orthotope_membership & bitmasks[current_cut]);
            }
            else if (!simple_comp_eq.is_true())
            {
                orthotope_membership = (orthotope_membership & bitmasks_flipped[current_cut]);
            }
#if EVAL > 1
            auto polytope_splitting_generate_sc_comp_time_end = std::chrono::steady_clock::now();
            eval->polytope_splitting_generate_sc_comp_time += (polytope_splitting_generate_sc_comp_time_end - polytope_splitting_generate_sc_comp_time_begin);
#endif
        }
#if EVAL > 1
        auto polytope_splitting_generate_sc_insert_time_begin = std::chrono::steady_clock::now();
#endif
        // Add coordinate to suitbale orthotopes
        for (unsigned int current_orthotope = 0; current_orthotope<new_orthotopes_count; current_orthotope++)
        {
            if (orthotope_membership[current_orthotope])
            {
                res[current_orthotope].push_back(coord);
            }
        }
#if EVAL > 1
        auto polytope_splitting_generate_sc_insert_time_end = std::chrono::steady_clock::now();
        eval->polytope_splitting_generate_sc_insert_time += (polytope_splitting_generate_sc_insert_time_end - polytope_splitting_generate_sc_insert_time_begin);
#endif
    }
    return res;
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

cut_list orthotope::split_bisect_single()
{
    cut_list cuts;
    unsigned int dim = boundaries.size();
    unsigned int cut_dim = depth % dim;
    z3::expr mid = (boundaries[cut_dim].first + boundaries[cut_dim].second)/2;
    cuts.push_back({cut_dim, mid.simplify()});
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

void orthotope::sample_sub(sampling_heuristic sampling_h, z3::context &ctx, z3::expr &formula, z3::expr_vector &variable_names, splitting_heuristic splitting_h, bool use_split_samples)
{
    switch (sampling_h)
    {
    case sampling_heuristic::center:
        sample_center(ctx, formula, variable_names, use_split_samples);
        break;
    case sampling_heuristic::clever:
        sample_clever(ctx, formula, variable_names, splitting_h);
        break;
    default:
        break;
    }
}

void orthotope::sample_center(z3::context &ctx, z3::expr &formula, z3::expr_vector &variable_names, bool use_split_samples)
{

#if EVAL > 1
    auto sampling_creation_time_begin = std::chrono::steady_clock::now();
#endif
    z3::expr_vector dest(ctx);
    for (const auto &boundary : boundaries)
    {
        dest.push_back((boundary.first+boundary.second)/2);
    }
#if EVAL > 1
    auto sampling_creation_time_end = std::chrono::steady_clock::now();
    eval->sampling_creation_time += (sampling_creation_time_end - sampling_creation_time_begin);
#endif

    // substitution
#if EVAL > 1
    auto sampling_substitution_time_begin = std::chrono::steady_clock::now();
#endif
    z3::expr formula_substituted = formula.substitute(variable_names, dest);
#if EVAL > 1
    auto sampling_substitution_time_end = std::chrono::steady_clock::now();
    eval->sampling_substitution_time += (sampling_substitution_time_end - sampling_substitution_time_begin);
#endif

    // simplification
#if EVAL > 1
    auto sampling_simplification_time_begin = std::chrono::steady_clock::now();
#endif
    z3::expr simple = formula_substituted.simplify();
#if EVAL > 1
    auto sampling_simplification_time_end = std::chrono::steady_clock::now();
    eval->sampling_simplification_time += (sampling_simplification_time_end - sampling_simplification_time_begin);
    eval->simplify_sample_count++;
#endif

    // sanity check!
#ifdef SAFE
#if EVAL > 1
    auto sampling_sanity_time_begin = std::chrono::steady_clock::now();
#endif
    z3::expr formula_neg = (!formula_substituted);
    z3::expr simple_neg = formula_neg.simplify();
    if (simple.is_true() == simple_neg.is_true())
    {
        throw simplification_impossible();
    }
#if EVAL > 1
    auto sampling_sanity_time_end = std::chrono::steady_clock::now();
    eval->sampling_sanity_time += (sampling_sanity_time_end - sampling_sanity_time_begin);
#endif
#endif

    // insertion
#if EVAL > 1
    auto sampling_insertion_time_begin = std::chrono::steady_clock::now();
#endif
    if(use_split_samples)
    {
        coordinate c;
        for(const auto &val : dest)
        {
            c.push_back(val);
        }
        if (simple.is_true())
        {
            safe_coordinates.push_back(c);
        }
        else
        {
            unsafe_coordinates.push_back(c);
        }
    }
    else
    {
        if (simple.is_true())
        {
            has_safe_sample = true;
            has_unsafe_sample = false;
        }
        else
        {
            has_safe_sample = false;
            has_unsafe_sample = true;
        }
    }
#if EVAL > 1
    auto sampling_insertion_time_end = std::chrono::steady_clock::now();
    eval->sampling_insertion_time += (sampling_insertion_time_end - sampling_insertion_time_begin);
#endif
}

void orthotope::sample_clever(z3::context &ctx, z3::expr &formula, z3::expr_vector &variable_names, splitting_heuristic splitting_h)
{
    switch (splitting_h)
    {
    case splitting_heuristic::bisect_all:
        if(!( depth % 2 == 0))
        {
            return;
        }
        break;
    case splitting_heuristic::bisect_single:
        {
            unsigned int dim = boundaries.size();
            if(!( depth % (dim+1) == 0))
            {
                return;
            }
            break;
        }
    default:
        throw clever_sampling_not_supported();
        break;
    }
#if EVAL > 1
    auto sampling_creation_time_begin = std::chrono::steady_clock::now();
#endif
    z3::expr_vector dest(ctx);
    for (const auto &boundary : boundaries)
    {
        dest.push_back((boundary.first+boundary.second)/2);
    }
#if EVAL > 1
    auto sampling_creation_time_end = std::chrono::steady_clock::now();
    eval->sampling_creation_time += (sampling_creation_time_end - sampling_creation_time_begin);
#endif

    // substitution
#if EVAL > 1
    auto sampling_substitution_time_begin = std::chrono::steady_clock::now();
#endif
    z3::expr formula_substituted = formula.substitute(variable_names, dest);
#if EVAL > 1
    auto sampling_substitution_time_end = std::chrono::steady_clock::now();
    eval->sampling_substitution_time += (sampling_substitution_time_end - sampling_substitution_time_begin);
#endif

    // simplification
#if EVAL > 1
    auto sampling_simplification_time_begin = std::chrono::steady_clock::now();
#endif
    z3::expr simple = formula_substituted.simplify();
#if EVAL > 1
    auto sampling_simplification_time_end = std::chrono::steady_clock::now();
    eval->sampling_simplification_time += (sampling_simplification_time_end - sampling_simplification_time_begin);
    eval->simplify_sample_count++;
#endif

    // sanity check!
#ifdef SAFE
#if EVAL > 1
    auto sampling_sanity_time_begin = std::chrono::steady_clock::now();
#endif
    z3::expr formula_neg = (!formula_substituted);
    z3::expr simple_neg = formula_neg.simplify();
    if (simple.is_true() == simple_neg.is_true())
    {
        throw simplification_impossible();
    }
#if EVAL > 1
    auto sampling_sanity_time_end = std::chrono::steady_clock::now();
    eval->sampling_sanity_time += (sampling_sanity_time_end - sampling_sanity_time_begin);
#endif
#endif
    // set val
    if (simple.is_true())
    {
        has_safe_sample = true;
        has_unsafe_sample = false;
    }
    else
    {
        has_safe_sample = false;
        has_unsafe_sample = true;
    }
}

z3::expr orthotope::get_volume_sub(z3::context &ctx)
{
    z3::expr res = ctx.real_val("1");
    for (const auto &interval : boundaries)
    {
        z3::expr interval_length = interval.second-interval.first;
        res = res * interval_length;
    }
    return res;
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
