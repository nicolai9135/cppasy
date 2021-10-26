#include "argument_parser.hpp"

#include <boost/program_options.hpp>
#include <iostream>


options parse_arguments(int argc, char* argv[])
{
    options res;

    // visible options
    boost::program_options::options_description visible("Allowed options");
    visible.add_options()
        ("help,h", "produce help message")
        ("max-depth", boost::program_options::value<int>()->default_value(3), "set maximal depth")
    ;

    // hidden options
    boost::program_options::options_description hidden("Hidden options");
    hidden.add_options()
        ("formula-file", boost::program_options::value<std::string>()->required(), ".smt2 file containing the formula")
    ;

    boost::program_options::options_description cmdline_options;
    cmdline_options.add(visible).add(hidden);

    boost::program_options::positional_options_description p;
    p.add("formula-file", -1);

    boost::program_options::variables_map vm;

    boost::program_options::store(boost::program_options::command_line_parser(argc, argv).options(cmdline_options).positional(p).run(), vm);

    if (vm.count("help")) 
    {
        std::cout << visible << std::endl;
        throw help();
    }

    boost::program_options::notify(vm);    

    if (vm.count("formula-file"))
    {
        res.formula_file = vm["formula-file"].as<std::string>();
        std::cout << "formula files is: "  << vm["formula-file"].as<std::string>() << std::endl;
    }

    // no else necessary because of default argument
    if (vm.count("max-depth")) 
    {
        res.max_depth = vm["max-depth"].as<int>();
        std::cout << "Maximal depth was set to " << vm["max-depth"].as<int>() << std::endl;
    } 

    return res;
}
