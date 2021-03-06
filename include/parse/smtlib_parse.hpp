#ifndef SMTLIB_PARSE
#define SMTLIB_PARSE

#include <z3++.h>
#include <string>
#include <set>

/**
 * Takes an smtlib formula and returns a list of all variables used. Note, that
 * this function is not very clean and may fail at certain inputs: It assumes
 * that the expressions `e` in the input formula with 
 * `e.decl().decl_kind() == Z3_OP_UNINTERPRETED` are exaclty the free variables.
 * If this function fails it may be necessary to write a parser or import an 
 * existing one. `z3`'s parser is apperently not able to give a list of the
 * declared/used variables.
 * @param input either file name or the formula itself
 * @param formula_as_file indicates whether input is the formula itself or a 
 * path to the smtlib file containing the formula
 */ 
std::set<std::string> find_variable_names(std::string input, bool formula_as_file);

/**
 * recursive helper for #find_variable_names
 */
void find_variable_names_recursion(z3::expr const & e, std::set<std::string> &variable_names);

#endif