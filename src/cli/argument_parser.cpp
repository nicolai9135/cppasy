#include "argument_parser.hpp"
#include "smtlib_parse.hpp"
#include "parse_boundaries.hpp"

#include <boost/program_options.hpp>
#include <iostream>
#include <filesystem>

options parse_arguments(int argc, char* argv[])
{
    options res;

    // using the cli, formula_str contains the name of the file!
    res.formula_as_file = true;

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

    // handle formula file
    if (vm.count("formula-file"))
    {
        // get formula-file string
        res.formula_str = vm["formula-file"].as<std::string>();

        // chech whether specified string is a valid file
        if (!std::filesystem::is_regular_file(res.formula_str))
        {
            throw invalid_formula_path();
        }
        
        // get variable names
        try
        {
            res.variable_names = find_variable_names(res.formula_str, res.formula_as_file);
        }
        catch(const std::exception& e)
        {
            throw invalid_formula_file();
        }
    }
    else
    {
        throw no_input_file();
    }

    // handle boundaries file
    if (vm.count("boundaries-file"))
    {
        // get boundaries-file string
        std::string boundaries_file = vm["boundaries-file"].as<std::string>();

        // chech whether specified string is a valid file
        if (!std::filesystem::is_regular_file(boundaries_file))
        {
            throw invalid_boundaries_path();
        }

        // get boundaries from file
        res.initial_intervals = parse_boundaries(boundaries_file);

        // check consistency of boundaries-file and formula-file
        for(const auto &ii : res.initial_intervals)
        {
            if(!res.variable_names.contains(std::get<0>(ii)))
            {
                throw too_many_intervals();
            }
        }
        if(res.variable_names.size() > res.initial_intervals.size())
        {
            throw interval_missing();
        }
    }
    // set boundaries to default values
    else
    {
        res.initial_intervals = {};
        for(const auto &variable_name : res.variable_names)
        {
            res.initial_intervals.push_back({variable_name, "-1", "2"});
        }
    }

    // handle max-depth option
    if (vm.count("max-depth")) 
    {
        res.max_depth = vm["max-depth"].as<unsigned int>();
        std::cout << "Maximal depth was set to " << vm["max-depth"].as<unsigned int>() << std::endl;
    } 

    return res;
}