#ifndef SMTLIB_PARSE
#define SMTLIB_PARSE

#include <z3++.h>
#include <string>
#include <set>

std::set<std::string> get_variable_names(std::string input);
void get_variable_names_recursion(z3::expr const & e, std::set<std::string> &variable_names);

#endif