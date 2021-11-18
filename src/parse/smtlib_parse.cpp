#include "smtlib_parse.hpp"

std::set<std::string> get_variable_names(std::string input)
{
    z3::context ctx;
    z3::expr_vector file_vector = ctx.parse_string(input.c_str());

    std::set<std::string> variable_names;

    for(const auto &expr : file_vector)
    {
        get_variable_names_recursion(expr, variable_names);
    }

    return variable_names;
}

void get_variable_names_recursion(z3::expr const & e, std::set<std::string> &variable_names)
{
    if (e.is_app())
    {
        unsigned num = e.num_args();
        for (unsigned i = 0; i < num; i++)
        {
            get_variable_names_recursion(e.arg(i), variable_names);
        }
        z3::func_decl f = e.decl();
        if( f.decl_kind() == Z3_OP_UNINTERPRETED )
        {
            variable_names.insert(f.name().str());
        }
    }
}
