/**
 * @file    main.cpp
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
        std::cout << "execute '$ cppasy --help' for usage information" << std::endl;
        return 1;
    }
    catch(...)
    {
        std::cerr << "Exception of unknown type!" << std::endl;
        return 1;
    }

    synthesis_queues res = parameter_synthesis(user_input);

    /*
    // print queues
    std::cout << "SAFE areas" << std::endl;
    std::cout << "==========" << std::endl;
    print_and_delete(res.safe_areas);

    std::cout << "UNSAFE areas" << std::endl;
    std::cout << "============" << std::endl;
    print_and_delete(res.unsafe_areas);

    std::cout << "UNKNOWN areas" << std::endl;
    std::cout << "=============" << std::endl;
    print_and_delete(res.unknown_areas);
    */

    
    return 0;
}