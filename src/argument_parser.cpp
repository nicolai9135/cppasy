#include "argument_parser.hpp"

#include <boost/program_options.hpp>
#include <iostream>

options parse_arguments(int argc, char* argv[])
{
    options res;

    /**
    boost::program_options::options_description desc("Allowed options");
    desc.add_options()
        ("help", "produce help message")
        ("compression", boost::program_options::value<int>(), "set compression level")
    ;

    boost::program_options::variables_map vm;
    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
    boost::program_options::notify(vm);    

    if (vm.count("help")) {
        std::cout << desc << "\n";
        return res;
    }

    if (vm.count("compression")) {
        std::cout << "Compression level was set to " 
    << vm["compression"].as<int>() << ".\n";
    } else {
        std::cout << "Compression level was not set.\n";
    }
    */

    return res;
}
