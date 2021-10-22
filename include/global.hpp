#ifndef GLOBAL
#define GLOBAL

#include<vector>
#include<z3++.h>

using coordinate = std::vector<z3::expr>;
using interval = std::pair<z3::expr, z3::expr>;
using intervals = std::vector<interval>;
enum splitting_heuristic { bisect_all, bisect_single };
enum sampling_heuristic { corners };

#endif