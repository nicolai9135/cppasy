/**
 * @file    cli.cpp
 */

#include "argument_parser.hpp"
#include "polytope.hpp"
#include "orthotope.hpp"
#include "parameter_synthesis.hpp"

int main(int argc, char * argv[])
{
    // struct to store user input
    cli_options user_input;

    // try to parse command line arguments
    try
    {
        user_input = parse_arguments(argc, argv);
    }
    // if --help is passed, abort execution
    catch(const help &e)
    {
        return 1;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        std::cout << "execute '$ cppasy --help' for usage information" << std::endl;
        return 1;
    }
    catch(...)
    {
        std::cerr << "Exception of unknown type!" << std::endl;
        return 1;
    }

    synthesis* s = new synthesis(user_input);
    s->execute();
    s->print_all_areas();
    s->continue_synthesis(1);
    s->print_all_areas();
    s->continue_synthesis(1);
    s->print_all_areas();

    return 0;
}