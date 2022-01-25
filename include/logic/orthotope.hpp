#ifndef ORTHOTOPE
#define ORTHOTOPE

#include "polytope.hpp"

// For compilers that support precompilation, includes "wx/wx.h".

#include <boost/dynamic_bitset.hpp>

/**
 * split_info contains the list of cuts wich are done to create the new 
 * orthotopes. The first element of the pair is the dimension, the second is
 * the value at which the orthotope is cut.
 */
using cut_list = std::vector<std::pair<unsigned int, z3::expr>>;

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

    // implementations of virtual #polytope functions
    z3::expr_vector get_boundaries_z3_sub() override;
    void print_sub() override;
    std::deque<std::unique_ptr<polytope>> split_sub() override;
    void sample_sub() override;
    z3::expr get_volume_sub() override;


    /** see ::splitting_heuristic for details */ 
    cut_list split_bisect_all();

    /** see ::splitting_heuristic for details */ 
    cut_list split_bisect_single();

    /** see ::sampling_heuristic for details */ 
    void sample_center();

    /** see ::sampling_heuristic for details */ 
    void sample_clever();

    /**
     * Recursive helper function for #cartesian_product
     * @param accum Current version of the cartesian product to return in the end
     * @param stack Current version of the current element in the cartesian product
     * @param sequences Sets of which the cartesian product should be taken
     * @param index used to access a certain "factor" of the cartesian product
     */
    void cartesian_recursion(std::vector<intervals> &accum, intervals stack, std::vector<intervals> sequences, long unsigned int index);

    /**
     * Takes the cartesian product over pairs of ::interval s. Used by #split .
     * @param intervals_bisected pairs over which the cartesian product should be taken
     * @return `vector` of ::intervals (#boundaries of the new #orthotope s)
     */
    std::vector<intervals> cartesian_product(std::vector<intervals> sequences);

    /**
     * Generates a list of intervals. each element of this list contains the
     * boundaries for exactely one new orthotope.
     * @param cuts specifies where to cut/split the current polytope
     * @return new boundaries
     */
    std::vector<intervals> generate_new_boundaries(cut_list cuts);

    /**
     * Takes the coordinates and splits them according to cuts. Thereby it
     * assigns them to their new #orthotope s
     * @param cuts specifies where the current polytope is going to be split
     * @param coordinates list of coordinates to be split
     * @return list of coordinates for every new #orthotope
     */
    std::vector<std::vector<coordinate>> split_coordinates(cut_list &cuts, std::vector<coordinate> &coordinates);


    /**
     * Takes the list of cuts and uses uses it to cut #this into 
     * 2^(cuts.size()) new #orthotope s. Uses #generate_new_boundaries
     * @param cuts defines which dimensions to cut at which positions
     * @return new #orthotope s 
     */
    std::deque<std::unique_ptr<polytope>> generate_orthotopes(cut_list cuts);

public:
    /**
     * Constructor.
     * @param bs ::intervals defining the new #boundaries of the #orthotope
     * @param d depth of the #orthotope to create
     * @param sc #safe_coordinates within the new #boundaries
     * @param uc #unsafe_coordinates within the new #boundaries
     */
    orthotope(intervals bs, unsigned int d, synthesis *s_in, bool has_s_s = false, bool has_u_s = false, std::vector<coordinate> sc = {}, std::vector<coordinate> uc = {});
    intervals get_boundaries();
};

#endif