#ifndef EVALUATION
#define EVALUATION

#include <chrono>

struct evaluation
{
#if EVAL > 0
    unsigned int solver_count = 0;
    unsigned int solver_count_necessary = 0;
    unsigned int skip_solver_count = 0;

    std::chrono::duration<long, std::ratio<1, 1000000000>> total_time = std::chrono::seconds(0);
    std::chrono::duration<long, std::ratio<1, 1000000000>> solving_time = std::chrono::seconds(0);
    std::chrono::duration<long, std::ratio<1, 1000000000>> sampling_time = std::chrono::seconds(0);
    std::chrono::duration<long, std::ratio<1, 1000000000>> polytope_splitting_cut_time = std::chrono::seconds(0);
    std::chrono::duration<long, std::ratio<1, 1000000000>> polytope_splitting_generate_time = std::chrono::seconds(0);
    std::chrono::duration<long, std::ratio<1, 1000000000>> model_saving_time = std::chrono::seconds(0);


    void print();
#endif
#if EVAL > 1
    std::chrono::duration<long, std::ratio<1, 1000000000>> sampling_creation_time = std::chrono::seconds(0);
    std::chrono::duration<long, std::ratio<1, 1000000000>> sampling_substitution_time = std::chrono::seconds(0);
    std::chrono::duration<long, std::ratio<1, 1000000000>> sampling_simplification_time = std::chrono::seconds(0);
    std::chrono::duration<long, std::ratio<1, 1000000000>> sampling_sanity_time = std::chrono::seconds(0);
    std::chrono::duration<long, std::ratio<1, 1000000000>> sampling_insertion_time = std::chrono::seconds(0);
    std::chrono::duration<long, std::ratio<1, 1000000000>> polytope_splitting_generate_boundaries_time = std::chrono::seconds(0);
    std::chrono::duration<long, std::ratio<1, 1000000000>> polytope_splitting_generate_sc_time = std::chrono::seconds(0);
    std::chrono::duration<long, std::ratio<1, 1000000000>> polytope_splitting_generate_sc_bm_time = std::chrono::seconds(0);
    std::chrono::duration<long, std::ratio<1, 1000000000>> polytope_splitting_generate_sc_simp_time = std::chrono::seconds(0);
    std::chrono::duration<long, std::ratio<1, 1000000000>> polytope_splitting_generate_sc_sanity_time = std::chrono::seconds(0);
    std::chrono::duration<long, std::ratio<1, 1000000000>> polytope_splitting_generate_sc_comp_time = std::chrono::seconds(0);
    std::chrono::duration<long, std::ratio<1, 1000000000>> polytope_splitting_generate_sc_insert_time = std::chrono::seconds(0);
    std::chrono::duration<long, std::ratio<1, 1000000000>> polytope_splitting_generate_newq_time = std::chrono::seconds(0);
#endif

};
#endif