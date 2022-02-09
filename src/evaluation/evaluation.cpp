#include "evaluation.hpp"
#include <iostream>

#if EVAL > 0
void evaluation::print()
{
    std::cout <<   "Evaluation" << std::endl;
    std::cout <<   "==========" << std::endl;
    std::cout << std::endl;
    std::cout <<   "    Execution Time" << std::endl;
    std::cout <<   "    --------------" << std::endl;
    // std::cout <<   "      Total                      = " << std::chrono::duration_cast<std::chrono::seconds>(total_time).count() << "[s]" << std::endl;
    std::cout <<   "      Total                      = " << std::chrono::duration_cast<std::chrono::milliseconds>(total_time).count() << "[ms]" << std::endl;
    // std::cout <<   "        Solving                    = " << std::chrono::duration_cast<std::chrono::seconds>(solving_time).count() << "[s]" << std::endl;
    std::cout <<   "        * Solving                  | " << std::chrono::duration_cast<std::chrono::milliseconds>(solving_time).count() << "[ms]" << std::endl;
    // std::cout <<   "        Model Saving               = " << std::chrono::duration_cast<std::chrono::seconds>(model_saving_time).count() << "[s]" << std::endl;
    std::cout <<   "        * Model Saving             | " << std::chrono::duration_cast<std::chrono::milliseconds>(model_saving_time).count() << "[ms]" << std::endl;
    // std::cout <<   "        Sampling                   = " << std::chrono::duration_cast<std::chrono::seconds>(sampling_time).count() << "[s]" << std::endl;
    std::cout <<   "        * Sampling                 | " << std::chrono::duration_cast<std::chrono::milliseconds>(sampling_time).count() << "[ms]" << std::endl;
#if EVAL > 1
    std::cout <<   "          - Creation                 | " << std::chrono::duration_cast<std::chrono::milliseconds>(sampling_creation_time).count() << "[ms]" << std::endl;
    std::cout <<   "          - Substitution             | " << std::chrono::duration_cast<std::chrono::milliseconds>(sampling_substitution_time).count() << "[ms]" << std::endl;
    std::cout <<   "          - Simplification           | " << std::chrono::duration_cast<std::chrono::milliseconds>(sampling_simplification_time).count() << "[ms]" << "    (for " << simplify_sample_count << " '.simplify()' calls)" << std::endl;
    std::cout <<   "          - Sanity                   | " << std::chrono::duration_cast<std::chrono::milliseconds>(sampling_sanity_time).count() << "[ms]" << std::endl;
    std::cout <<   "          - Insertion                | " << std::chrono::duration_cast<std::chrono::milliseconds>(sampling_insertion_time).count() << "[ms]" << std::endl;
#endif
    // std::cout <<   "        Polytope Splitting      = " << std::chrono::duration_cast<std::chrono::seconds>(polytope_splitting_time).count() << "[s]" << std::endl;
    std::cout <<   "        * Polytope Splitting       | " << std::chrono::duration_cast<std::chrono::milliseconds>(polytope_splitting_cut_time + polytope_splitting_generate_time).count() << "[ms]" << std::endl;
#if EVAL > 1
    std::cout <<   "          - Create Cuts              | " << std::chrono::duration_cast<std::chrono::milliseconds>(polytope_splitting_cut_time).count() << "[ms]" << std::endl;
    std::cout <<   "          - Generate Polytopes       | " << std::chrono::duration_cast<std::chrono::milliseconds>(polytope_splitting_generate_time).count() << "[ms]" << std::endl;
    std::cout <<   "            > Generate Boundaries      | " << std::chrono::duration_cast<std::chrono::milliseconds>(polytope_splitting_generate_boundaries_time).count() << "[ms]" << std::endl;
    std::cout <<   "            > Split Coordinates        | " << std::chrono::duration_cast<std::chrono::milliseconds>(polytope_splitting_generate_sc_time).count() << "[ms]" << std::endl;
    // std::cout <<   "              + Generate Bitmap          | " << std::chrono::duration_cast<std::chrono::milliseconds>(polytope_splitting_generate_sc_bm_time).count() << "[ms]" << std::endl;
    std::cout <<   "              + Simplification           | " << std::chrono::duration_cast<std::chrono::milliseconds>(polytope_splitting_generate_sc_simp_time).count() << "[ms]" << "    (for " << simplify_split_count << " '.simplify()' calls)" << std::endl;
    std::cout <<   "              + Sanity                   | " << std::chrono::duration_cast<std::chrono::milliseconds>(polytope_splitting_generate_sc_sanity_time).count() << "[ms]" << std::endl;
    std::cout <<   "              + Comparison               | " << std::chrono::duration_cast<std::chrono::milliseconds>(polytope_splitting_generate_sc_comp_time).count() << "[ms]" << std::endl;
    std::cout <<   "              + Insertion                | " << std::chrono::duration_cast<std::chrono::milliseconds>(polytope_splitting_generate_sc_insert_time).count() << "[ms]" << std::endl;
    std::cout <<   "            > Create New Queue         | " << std::chrono::duration_cast<std::chrono::milliseconds>(polytope_splitting_generate_newq_time).count() << "[ms]" << std::endl;
#endif
    std::cout << std::endl;
    std::cout <<   "    Solver Skipping" << std::endl;
    std::cout <<   "    ---------------" << std::endl;
    std::cout <<   "      Solver Activation Count:  " << solver_count << std::endl;
    std::cout <<   "        * Necessary:            | " << solver_count_necessary << std::endl;
    std::cout <<   "        * Preventable:          | " << solver_count-solver_count_necessary << std::endl;
    std::cout <<   "      Solver Skip Count:        " << skip_solver_count << std::endl;
}
#endif

#if EVAL > 0
void evaluation::print_parseable()
{
    std::cout << std::endl;
    std::cout <<   "time_total                          = " << std::chrono::duration_cast<std::chrono::milliseconds>(total_time).count() << "[ms]" << std::endl;
    std::cout <<   "time_solving                        | " << std::chrono::duration_cast<std::chrono::milliseconds>(solving_time).count() << "[ms]" << std::endl;
    std::cout <<   "time_model_saving                   | " << std::chrono::duration_cast<std::chrono::milliseconds>(model_saving_time).count() << "[ms]" << std::endl;
    std::cout <<   "time_sampling_total                 | " << std::chrono::duration_cast<std::chrono::milliseconds>(sampling_time).count() << "[ms]" << std::endl;
#if EVAL > 1
    std::cout <<   "time_sampling_creation                | " << std::chrono::duration_cast<std::chrono::milliseconds>(sampling_creation_time).count() << "[ms]" << std::endl;
    std::cout <<   "time_sampling_substitution            | " << std::chrono::duration_cast<std::chrono::milliseconds>(sampling_substitution_time).count() << "[ms]" << std::endl;
    std::cout <<   "time_sampling_simplify                | " << std::chrono::duration_cast<std::chrono::milliseconds>(sampling_simplification_time).count() << "[ms]" << "    (for " << simplify_sample_count << " '.simplify()' calls)" << std::endl;
    std::cout <<   "time_sampling_sanity                  | " << std::chrono::duration_cast<std::chrono::milliseconds>(sampling_sanity_time).count() << "[ms]" << std::endl;
    std::cout <<   "time_sampling_insertion               | " << std::chrono::duration_cast<std::chrono::milliseconds>(sampling_insertion_time).count() << "[ms]" << std::endl;
#endif
    std::cout <<   "time_splitting_total                | " << std::chrono::duration_cast<std::chrono::milliseconds>(polytope_splitting_cut_time + polytope_splitting_generate_time).count() << "[ms]" << std::endl;
#if EVAL > 1
    std::cout <<   "time_splitting_cut                    | " << std::chrono::duration_cast<std::chrono::milliseconds>(polytope_splitting_cut_time).count() << "[ms]" << std::endl;
    std::cout <<   "time_splitting_gen_total              | " << std::chrono::duration_cast<std::chrono::milliseconds>(polytope_splitting_generate_time).count() << "[ms]" << std::endl;
    std::cout <<   "time_splitting_gen_boundaries           | " << std::chrono::duration_cast<std::chrono::milliseconds>(polytope_splitting_generate_boundaries_time).count() << "[ms]" << std::endl;
    std::cout <<   "time_splitting_gen_sc_total             | " << std::chrono::duration_cast<std::chrono::milliseconds>(polytope_splitting_generate_sc_time).count() << "[ms]" << std::endl;
    std::cout <<   "time_splitting_gen_sc_simplify            | " << std::chrono::duration_cast<std::chrono::milliseconds>(polytope_splitting_generate_sc_simp_time).count() << "[ms]" << "    (for " << simplify_split_count << " '.simplify()' calls)" << std::endl;
    std::cout <<   "time_splitting_gen_sc_sanity              | " << std::chrono::duration_cast<std::chrono::milliseconds>(polytope_splitting_generate_sc_sanity_time).count() << "[ms]" << std::endl;
    std::cout <<   "time_splitting_gen_sc_comparison          | " << std::chrono::duration_cast<std::chrono::milliseconds>(polytope_splitting_generate_sc_comp_time).count() << "[ms]" << std::endl;
    std::cout <<   "time_splitting_gen_sc_insertion           | " << std::chrono::duration_cast<std::chrono::milliseconds>(polytope_splitting_generate_sc_insert_time).count() << "[ms]" << std::endl;
    std::cout <<   "time_splitting_gen_new                  | " << std::chrono::duration_cast<std::chrono::milliseconds>(polytope_splitting_generate_newq_time).count() << "[ms]" << std::endl;
#endif
    std::cout << std::endl;
    std::cout <<   "solver_activation_count     " << solver_count << std::endl;
    std::cout <<   "solver_necessary_count      | " << solver_count_necessary << std::endl;
    std::cout <<   "solver_preventable_count    | " << solver_count-solver_count_necessary << std::endl;
    std::cout <<   "solver_skip_count           " << skip_solver_count << std::endl;
}
#endif