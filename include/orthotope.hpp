#ifndef ORTHOTOPE
#define ORTHOTOPE

#include "polytope.hpp"

/**
 * Subclass of #polytope, represents all polytopes with orthogonal edges only.
 * Formally, an #orthotope (or hyperrectangle or box) is the cartesian product
 * of orthogonal intervals. Therefore it is possible to describe its surface
 * solely through providing the corresponding ::intervals .
 */
class orthotope: public polytope 
{
private:
    /**
     * Set of ::intervals defining the #orthotope.
     */
    intervals boundaries;

    /**
     * Transforms #boundaries into `z3` constraints.
     * @param ctx `z3::context` in which the expressions should be defined
     * @param variable_names list of names of the free variables
     * @return `z3` representation of #boundaries
     */
    z3::expr_vector get_boundaries_z3_sub(z3::context &ctx, z3::expr_vector &variable_names) override;

    void print_sub() override;

    std::deque<std::unique_ptr<polytope>> split_bisect_all() override;

    /**
     * Recursive helper function for #cartesian_product
     * @param accum Current version of the cartesian product to return in the end
     * @param stack Current version of the current element in the cartesian product
     * @param intervals_bisected Sets (pairs) of which the cartesian product should be taken
     * @param index used to access a certain "factor" of the cartesian product
     */
    void cartesian_recursion(std::vector<intervals>& accum, intervals stack, std::vector<std::pair<interval, interval>> intervals_bisected, int index);

    /**
     * Takes the cartesian product over pairs of ::interval s. Used by #split .
     * @param intervals_bisected pairs over which the cartesian product should be taken
     * @return `vector` of ::intervals (#boundaries of the new #orthotope s)
     */
    std::vector<intervals> cartesian_product(std::vector<std::pair<interval, interval>> intervals_bisected);

    /**
     * Takes ::intervals and bisects them. Used by #split .
     * @param intervals_in ::intervals to split
     * @return `pair`s of two equally sized ::interval s for every input ::interval
     */
    std::vector<std::pair<interval, interval>> bisect_all_intervals(intervals intervals_in);

    std::deque<std::unique_ptr<polytope>> intervals_to_orthotopes(std::vector<intervals> intervals_list);

public:
    /**
     * Constructor.
     * @param bs ::intervals defining the new #boundaries of the #orthotope
     * @param d depth of the #orthotope to create
     * @param sc #safe_coordinates within the new #boundaries
     * @param uc #unsafe_coordinates within the new #boundaries
     */
    orthotope(intervals bs, int d, std::vector<coordinate> sc = {}, std::vector<coordinate> uc = {});

    intervals get_boundaries();
};

#endif