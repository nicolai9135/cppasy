#ifndef POLYTOPE
#define POLYTOPE

#include "evaluation.hpp"
#include <vector>
#include <deque>
#include <memory>
#include <z3++.h>
#include <boost/bimap.hpp>
#include <boost/assign.hpp>
#include <exception>
#include <boost/dynamic_bitset.hpp>

// For compilers that support precompilation, includes "wx/wx.h".

// forward declaration
class synthesis;

/**
 * Used to store points \f$x \in \mathbb{R}^n, n \in \mathbb{N}\f$. The 
 * dimension \f$n\f$ is the length of the vector. `z3::expr` allows precise
 * calculations.
 */
using coordinate = std::vector<z3::expr>;

/**
 * Used to store an interval \f$[x,y] \subset \mathbb{R}\f$. `z3::expr` allows 
 * precise calculations.
 */
using interval = std::pair<z3::expr, z3::expr>;
using intervals = std::vector<interval>;

/** Exception in case simplification is not possible*/
class simplification_impossible : public std::exception
{
    virtual const char* what() const throw()
    {
        return "Simplification on this input is not possible. Choose 'no_sampling' and turn off 'split_samples'!";
    }
};

/** Exception in case a certain splitting heuristic does not support clever sampling*/
class clever_sampling_not_supported : public std::exception
{
    virtual const char* what() const throw()
    {
        return "The splitting heuristic you chose does not support 'clever_sampling'. Chose a different sampling or splitting heuristic!";
    }
};

/**
 * List of heuristics that can be used to split one #polytope into multiple
 * new ones.
 */
enum class splitting_heuristic 
{
    /**
     * Bisects all a #polytope in all dimensions. Using this heuristic on an
     * \f$n\f$-dimensional #polytope will thus result in \f$2^n\f$ new 
     * #polytope s.
     */
    bisect_all,
    bisect_single
};

typedef boost::bimap<std::string, splitting_heuristic> splitting_bimap_type;

const splitting_bimap_type splitting_bimap =
    boost::assign::list_of< splitting_bimap_type::relation >
        ("bisect_all", splitting_heuristic::bisect_all)
        ("bisect_single", splitting_heuristic::bisect_single);

/** List of heuristics that can be used to take samples within a #polytope . */
enum class sampling_heuristic 
{
    no_sampling,
    center,
    clever
};

typedef boost::bimap<std::string, sampling_heuristic> sampling_bimap_type;

const sampling_bimap_type sampling_bimap =
    boost::assign::list_of< sampling_bimap_type::relation >
        ("no_sampling", sampling_heuristic::no_sampling)
        ("center", sampling_heuristic::center)
        ("clever", sampling_heuristic::clever);

enum area_class
{
    safe,
    unsafe
};

struct axis
{
    bool is_x_axis;
    std::string name;
    unsigned int index;
    // needed when axis itself is printed
    int extra_space;
    double width;
    double scalar;
    double offset;
};

/**
 * The purpose of this tool is to perform parameter synthesis in a user-provided
 * region. To do so, the initial region is splitted multiple times into smaller
 * regions. #polytope is the class to represent one of these regions, resp.
 */
class polytope
{
private:
    // used by public wrapper functions
    virtual z3::expr_vector get_boundaries_z3_sub() = 0;
    virtual void print_sub() = 0;
    virtual std::deque<std::unique_ptr<polytope>> split_sub() = 0;
    virtual void sample_sub() = 0;
    virtual z3::expr get_volume_sub() = 0;

protected:
    /**
     * This number represents the progress of the algorithm and is used to abort 
     * calculation at some point. The higher #depth, the more steps have been 
     * taken to come to this point. What #depth is actually representing is 
     * dependent (1) the concrete #polytope used (2) the splitting heuristic. 
     */
    unsigned int depth; 

    /**
     * Vector of ::coordinate s within the #polytope which have already 
     * been identified as safe. If this list is not empty, calling 
     * ``solver_pos.check()`` on this #polytope is not necessary anymore.
     */
    std::vector<coordinate> safe_coordinates;
    bool has_safe_sample;

    /**
     * Vector of ::coordinate s within the #polytope which have already 
     * been identified as unsafe. If this list is not empty, calling 
     * ``solver_neg.check()`` on this #polytope is not necessary anymore.
     */
    std::vector<coordinate> unsafe_coordinates;
    bool has_unsafe_sample;

public:
    virtual ~polytope() = default;
    unsigned int get_depth();
    std::vector<coordinate> get_safe_coordinates();
    std::vector<coordinate> get_unsafe_coordinates();

    /**
     * This Function is a wrapper around #get_boundaries_z3_sub .
     * The shape of a #polytope is defined through the set its boundaries. The
     * representation of a #polytope's boudaries is dependent on its shape and
     * surfaces. This function takes the #polytope dependent boundary
     * representation and puts them into `z3` format. A `z3` representation is
     * necessary for the solver to ensure that a solver only consideres
     * coordinates within the boundaries of a #polytope.
     * @param ctx `z3::context` in which the expressions should be defined
     * @param variable_names list of names of the free variables
     * @return `z3` representation of the #polytope's boundaries
     */
    z3::expr_vector get_boundaries_z3();

    /**
     * Wrapper for #print_sub . 
     * Prints all attributes of a #polytope. Mainly for develepement purposes
     */
    void print();

    /**
     * Splits `this` #polytope into multiple new polytopes depending on the
     * provided #splitting_heuristic.
     * @param splitting_h #splitting_heuristic to be used
     */
    std::deque<std::unique_ptr<polytope>> split();

    /**
     * Takes samples at different ::coordinate s within the #polytope and stores
     * them in #safe_coordinates for #unsafe_coordinates
     * @sideeffect extends #safe_coordinates and/or #unsafe_coordinates
     * @param sampling_h #sampling_heuristic to be used
     */
    void sample();

    /**
     * Draws the given #axis on the given device context
     * @param dc device context to draw the axis
     * @param x_axis x-axis to draw
     * @param y_axis y-axis to draw
     */

    /**
     * The model corresponds to a coordinate. This coordinate is appended to 
     * either #safe_coordinates or #unsafe_coordinates.
     * @sideeffect appends the coordinate to #safe_coordinates or #unsafe_coordinates
     * @param m model/coordinate to safe
     * @param ac indicator whether the m is a model for solver_pos or solver_neg
     */
    void save_model(const z3::model &m, area_class ac);

    bool coordinate_exists(area_class ac);

    z3::expr get_volume();

    synthesis *s;
};

#endif