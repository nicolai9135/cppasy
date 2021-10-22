#ifndef POLYTOPE
#define POLYTOPE

#include <vector>
#include <deque>
#include <memory>
#include <z3++.h>

using coordinate = std::vector<z3::expr>;
using interval = std::pair<z3::expr, z3::expr>;
using intervals = std::vector<interval>;
enum splitting_heuristic { bisect_all, bisect_single };
enum sampling_heuristic { corners };

class polytope
{
protected:
    int depth;
    std::vector<coordinate> safe_coordinates;
    std::vector<coordinate> unsafe_coordinates;
public:
    int get_depth();
    void set_depth(int d);
    std::vector<coordinate> get_safe_coordinates();
    std::vector<coordinate> get_unsafe_coordinates();
    virtual z3::expr_vector get_boundaries_z3(z3::context &ctx, z3::expr_vector &variable_names) = 0;
    virtual void print() = 0;
    virtual std::deque<std::unique_ptr<polytope>> split(splitting_heuristic splitting_h) = 0;
    virtual void sample(sampling_heuristic sampling_h) = 0;
};

#endif