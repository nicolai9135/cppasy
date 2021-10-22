#ifndef POLYTOPE
#define POLYTOPE

#include "global.hpp"
#include <vector>
#include <deque>
#include <memory>

class polytope
{
protected:
    int depth;
    std::vector<coordinate> safe_coordinates;
    std::vector<coordinate> unsafe_coordinates;
public:
    int get_depth();
    void set_depth(int new_depth);
    std::vector<coordinate> get_safe_coordinates();
    std::vector<coordinate> get_unsafe_coordinates();
    virtual std::deque<std::unique_ptr<polytope>> split(splitting_heuristic) = 0;
    virtual void sample(sampling_heuristic) = 0;
    virtual z3::expr_vector get_boundaries_z3(z3::context &ctx, z3::expr_vector &variable_names) = 0;
    virtual void print() = 0;
};

class orthotope: public polytope 
{
private:
    intervals boundaries;
public:
    intervals get_boundaries();
    orthotope(intervals boundaries_in, int depth_in, std::vector<coordinate> safe_coordinates_in = {}, std::vector<coordinate> unsafe_coordinates_in = {});
    std::deque<std::unique_ptr<polytope>> split(splitting_heuristic) override;
    void sample(sampling_heuristic) override;
    virtual z3::expr_vector get_boundaries_z3(z3::context &ctx, z3::expr_vector &variable_names) override;
    void print() override;
};

void cartesian_recursion(std::vector<intervals>&, intervals, std::vector<std::pair<interval, interval>>, int);
std::vector<intervals> cartesian_product(std::vector<std::pair<interval, interval>>);
std::vector<std::pair<interval, interval>> bisect_all_intervals(intervals);

#endif