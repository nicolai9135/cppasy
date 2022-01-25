#include "argument_parser.hpp"
#include "smtlib_parse.hpp"
#include "parse_boundaries.hpp"
#include "polytope.hpp"

#include <boost/program_options.hpp>
#include <iostream>
#include <filesystem>
#include <string>

options parse_arguments(int argc, char* argv[])
{
    options res;

    // using the cli, formula_str contains the name of the file!
    res.formula_as_file = true;

    // visible options
    boost::program_options::options_description visible("Allowed options");
    visible.add_options()
        ("help,h", "produce help message")
        ("boundaries-file", boost::program_options::value<std::string>(), "text file containing a list of all variables and their boundaries. The file should contain lines of the from '<variable-name> <lower-bound> <upper-bound>'")
        ("max-depth", boost::program_options::value<unsigned int>()->default_value(11), "set maximal depth")
        ("default-boundaries", boost::program_options::value<unsigned int>()->default_value(10), "set default 'radius' of the inital orthotope")
        ("splitting-heuristic", boost::program_options::value<std::string>()->default_value("bisect_all"), "Choose a splitting heuristic if you want to use sampling. Options are 'bisect_all'")
        ("sampling-heuristic", boost::program_options::value<std::string>()->default_value("no_sampling"), "Choose a sampling heuristic if you want to use sampling. Options are 'center'")
        ("split-samples", "also split samples when splitting orthotopes")
        ("save-model", "Save models found by solver. Only usefull if split-samples enabled!")
        ("execute-2in1", "use 2in1 execution to reuse context")
        ("splits-needed", "returns true if splits are needed to process this formula.")
    ;

    // hidden options
    boost::program_options::options_description hidden("Hidden options");
    hidden.add_options()
        ("formula-file", boost::program_options::value<std::string>(), ".smt2 file containing the formula")
    ;

    // make formula-file a positional argument(no "--formula-file=" necessary)
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
        res.sanity_check_intervals();
    }
    // set boundaries to default values
    else if (vm.count("default-boundaries")) 
    {
        res.initial_intervals = {};
        for(const auto &variable_name : res.variable_names)
        {
            unsigned int default_boundary = vm["default-boundaries"].as<unsigned int>();
            std::string default_boundary_string = std::to_string(default_boundary);
            res.initial_intervals.push_back({variable_name, "-" + default_boundary_string, default_boundary_string });
        }
    }
    else 
    {
        res.initial_intervals = {};
        for(const auto &variable_name : res.variable_names)
        {
            res.initial_intervals.push_back({variable_name, "-1", "1"});
        }
    }

    // handle max-depth option
    if (vm.count("max-depth")) 
    {
        res.max_depth = vm["max-depth"].as<unsigned int>();
    } 

    // handle splitting-heuristic option
    if (vm.count("splitting-heuristic"))
    {
        std::string splitting_heuristic_str = vm["splitting-heuristic"].as<std::string>();
        auto it = splitting_bimap.left.find(splitting_heuristic_str);
        if(it != splitting_bimap.left.end())
        {
            res.splitting_h = it->second;
        }
        else
        {
            throw invalid_splitting_heuristic();
        }
    }

    // handle sampling-heuristic option
    if (vm.count("sampling-heuristic"))
    {
        std::string sampling_heuristic_str = vm["sampling-heuristic"].as<std::string>();
        auto it = sampling_bimap.left.find(sampling_heuristic_str);
        if(it != sampling_bimap.left.end())
        {
            res.sampling_h = it->second;
        }
        else
        {
            throw invalid_sampling_heuristic();
        }
    }

    if (vm.count("split-samples")) 
    {
        res.use_split_samples = true;
    }
    else
    {
        res.use_split_samples = false;
    }

    if (vm.count("save-model")) 
    {
        res.use_save_model = true;
    }
    else
    {
        res.use_save_model = false;
    }

    if (vm.count("execute-2in1")) 
    {
        res.use_execute_2in1 = true;
    }
    else
    {
        res.use_execute_2in1 = false;
    }

    if (vm.count("splits-needed")) 
    {
        res.splits_needed = true;
    }

    return res;
}