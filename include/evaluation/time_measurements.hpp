#ifndef TIME_MEASUREMENTS
#define TIME_MEASUREMENTS

#include <chrono>

struct execution_time
{

#if EVAL == 1
    std::chrono::duration<long, std::ratio<1, 1000000000>> total = std::chrono::seconds(0);
    std::chrono::duration<long, std::ratio<1, 1000000000>> solving = std::chrono::seconds(0);
    std::chrono::duration<long, std::ratio<1, 1000000000>> sampling = std::chrono::seconds(0);
    std::chrono::duration<long, std::ratio<1, 1000000000>> polytope_splitting = std::chrono::seconds(0);
    std::chrono::duration<long, std::ratio<1, 1000000000>> model_saving = std::chrono::seconds(0);
#elif EVAL == 2
    std::chrono::duration<long, std::ratio<1, 1000000000>> sampling_creation = std::chrono::seconds(0);
    std::chrono::duration<long, std::ratio<1, 1000000000>> sampling_substitution = std::chrono::seconds(0);
    std::chrono::duration<long, std::ratio<1, 1000000000>> sampling_evaluation = std::chrono::seconds(0);
    std::chrono::duration<long, std::ratio<1, 1000000000>> sampling_insertion = std::chrono::seconds(0);
    std::chrono::duration<long, std::ratio<1, 1000000000>> coordinate_splitting = std::chrono::seconds(0);
    std::chrono::duration<long, std::ratio<1, 1000000000>> coordinate_splitting_bitmask = std::chrono::seconds(0);
    std::chrono::duration<long, std::ratio<1, 1000000000>> coordinate_splitting_comparison = std::chrono::seconds(0);
    std::chrono::duration<long, std::ratio<1, 1000000000>> coordinate_splitting_condition = std::chrono::seconds(0);
    std::chrono::duration<long, std::ratio<1, 1000000000>> coordinate_splitting_insertion = std::chrono::seconds(0);
#endif

};
#endif