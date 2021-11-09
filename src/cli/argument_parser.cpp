#include "argument_parser.hpp"

#include <boost/program_options.hpp>
#include <iostream>

cli_options parse_arguments(int argc, char* argv[])
{
    cli_options res;

    // visible options
    boost::program_options::options_description visible("Allowed options");
    visible.add_options()
        ("help,h", "produce help message")
        ("max-depth", boost::program_options::value<unsigned int>()->default_value(1), "set maximal depth")
        ("boundaries-file", boost::program_options::value<std::string>(), ".txt file containing a list of all variables and their boundaries")
    ;

    // hidden options
    boost::program_options::options_description hidden("Hidden options");
    hidden.add_options()
        ("formula-file", boost::program_options::value<std::string>(), ".smt2 file containing the formula")
    ;

    // make formula-file positional (no "--formula-file=" necessary)
    boost::program_options::positional_options_description p;
    p.add("formula-file", -1);

    // merge both options
    boost::program_options::options_description cmdline_options;
    cmdline_options.add(visible).add(hidden);

    boost::program_options::variables_map vm;
    boost::program_options::store(boost::program_options::command_line_parser(argc, argv).options(cmdline_options).positional(p).run(), vm);

    if (vm.count("help")) 
    {
        std::cout << visible << std::endl;

        // exception ensures termination of the program
        throw help();
    }

    boost::program_options::notify(vm);    

    if (vm.count("formula-file"))
    {
        res.formula_file = vm["formula-file"].as<std::string>();
    }
    else
    {
        throw no_input_file();
    }

    if (vm.count("max-depth")) 
    {
        res.max_depth = vm["max-depth"].as<unsigned int>();
        std::cout << "Maximal depth was set to " << vm["max-depth"].as<unsigned int>() << std::endl;
    } 

    if (vm.count("boundaries-file"))
    {
        res.boundaries_file = vm["boundaries-file"].as<std::string>();
    }

    return res;
}