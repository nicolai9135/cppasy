#ifndef POLYTOPE
#define POLYTOPE

#include <vector>
#include <deque>
#include <memory>
#include <z3++.h>

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

/**
 * List of heuristics that can be used to split one #polytope into multiple
 * new ones.
 */
enum splitting_heuristic 
{
    /**
     * Bisects all a #polytope in all dimensions. Using this heuristic on an
     * \f$n\f$-dimensional #polytope will thus result in \f$2^n\f$ new 
     * #polytope s.
     */
    bisect_all
    // bisect_single
};

/**
 * List of heuristics that can be used to take samples within a #polytope .
 */
enum sampling_heuristic 
{
    /**
     * Samples are taken on the vertices of the #polytope
     */
    vertices
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
    virtual z3::expr_vector get_boundaries_z3_sub(z3::context &ctx, z3::expr_vector &variable_names) = 0;
    virtual void print_sub() = 0;

    /**
     * Calles by #split if #splitting_heuristic `bisect_all` passed. See 
     * #splitting_heuristic for details.
     */
    virtual std::deque<std::unique_ptr<polytope>> split_bisect_all() = 0;

protected:
    /**
     * This number represents the progress of the algorithm and is used to abort 
     * calculation at some point. The higher #depth, the more steps have been 
     * taken to come to this point. What #depth is actually representing is 
     * dependent (1) the concrete #polytope used (2) the splitting heuristic. 
     */
    int depth; 

    /**
     * Vector of ::coordinate s within the #polytope which have already 
     * been identified as safe. If this list is not empty, calling 
     * ``solver_pos.check()`` on this #polytope is not necessary anymore.
     */
    std::vector<coordinate> safe_coordinates;

    /**
     * Vector of ::coordinate s within the #polytope which have already 
     * been identified as unsafe. If this list is not empty, calling 
     * ``solver_neg.check()`` on this #polytope is not necessary anymore.
     */
    std::vector<coordinate> unsafe_coordinates;

public:
    virtual ~polytope() = default;
    int get_depth();
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
    z3::expr_vector get_boundaries_z3(z3::context &ctx, z3::expr_vector &variable_names);

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
    std::deque<std::unique_ptr<polytope>> split(splitting_heuristic splitting_h);

    /**
     * Takes samples at different ::coordinate s within the #polytope and stores
     * them in #safe_coordinates for #unsafe_coordinates
     * @sideeffect extends #safe_coordinates and/or #unsafe_coordinates
     * @param sampling_h #sampling_heuristic to be used
     */
    void sample(sampling_heuristic sampling_h);
};

#endif