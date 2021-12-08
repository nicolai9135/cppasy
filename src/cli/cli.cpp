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
    options user_input;

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
        std::cout << "execute '$ ./cli --help' for usage information" << std::endl;
    
        return 1;
    }
    catch(...)
    {
        std::cerr << "Exception of unknown type!" << std::endl;
        return 1;
    }

    synthesis* s = new synthesis(user_input);

#if EVAL > 0
    auto total_time_begin = std::chrono::steady_clock::now();
#endif
    s->execute();
#if EVAL > 0
    auto total_time_end = std::chrono::steady_clock::now();
    s->eval.total_time += (total_time_end - total_time_begin);
    s->eval.print();
#endif
    // s->print_all_areas();

    return 0;
}