#ifndef ORTHOTOPE
#define ORTHOTOPE

#include "polytope.hpp"

class orthotope: public polytope 
{
private:
    intervals boundaries;
public:
    orthotope(intervals bs, int d, std::vector<coordinate> sc = {}, std::vector<coordinate> uc = {});
    intervals get_boundaries();
    virtual z3::expr_vector get_boundaries_z3(z3::context &ctx, z3::expr_vector &variable_names) override;
    void print() override;
    std::deque<std::unique_ptr<polytope>> split(splitting_heuristic splitting_heuristic_in) override;
    void sample(sampling_heuristic sampling_h) override;
};

void cartesian_recursion(std::vector<intervals>&, intervals, std::vector<std::pair<interval, interval>>, int);
std::vector<intervals> cartesian_product(std::vector<std::pair<interval, interval>>);
std::vector<std::pair<interval, interval>> bisect_all_intervals(intervals);

#endif