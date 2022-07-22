#ifndef TESTS_HPP
#define TESTS_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "HJM/HJM_type.h"
#include "swaptions_simulation/swaptions_simulation.h"
#include "command_line_app/command_line_app.h"

#include <stdio.h>
#include <stdlib.h>

class Tests
{
private:
    std::string read_test_file(const char *fileName)
    {
        auto stream = std::ifstream(fileName);
        std::stringstream ss;
        ss << stream.rdbuf();
        return ss.str();
    }

    std::string swaptions_to_string(parm *swaptions, int nSwaptions)
    {
        const char template_line[] = "Swaption %d: [SwaptionPrice: %.10lf StdError: %.10lf] \n";
        std::stringstream string_stream;
        /*
          template_line é um array com 56 elementos,
          onde SwaptionPrice e StdError tem 10 digitos cada.
          e 3 é número máximo de digitos que nSwaptions pode
          ter
        */
        char line[56 + 10 + 10 + 3];

        for (auto i = 0; i < nSwaptions; i++)
        {

            sprintf(line, template_line,
                    i, swaptions[i].dSimSwaptionMeanPrice, swaptions[i].dSimSwaptionStdError);

            string_stream << line;
        }

        return string_stream.str();
    }

public:
    Tests(/* args */) {}
    ~Tests()
    {
    }

    void runAll()
    {
        auto number_of_threads = 8;

        InputCommandLine simdev{3,             // swaptions
                                50,            // simulation_trials
                                number_of_threads,             // threads
                                DEFAULT_SEED}; // seed

        InputCommandLine simtest{1,             // swaptions
                                 5,             // simulation_trials
                                 number_of_threads,             // threads
                                 DEFAULT_SEED}; // seed

        InputCommandLine simsmall{16,            // swaptions
                                  10000,         // simulation_trials
                                  number_of_threads,             // threads
                                  DEFAULT_SEED}; // seed

        InputCommandLine simmedium{32,            // swaptions
                                   20000,         // simulation_trials
                                   number_of_threads,             // threads
                                   DEFAULT_SEED}; // seed

        InputCommandLine simlarge{64,            // swaptions
                                  40000,         // simulation_trials
                                  number_of_threads,             // threads
                                  DEFAULT_SEED}; // seed

        std::vector<InputCommandLine> inputs = {simdev, simtest, simsmall, simmedium, simlarge};
        std::vector<const char *> files = {"tests/simdev.txt", "tests/simtest.txt", "tests/simsmall.txt", "tests/simmedium.txt", "tests/simlarge.txt"};

        auto totalTests = files.size();
        for (std::size_t i = 0; i < totalTests; i++)
        {

            this->runTest(inputs[i], files[i]);
        }
    }

    void runTest(InputCommandLine &input, const char *fileName)
    {
        auto swaptions = new parm[input.swaptions];
        run_swaptions_simulation(swaptions, input.swaptions, input.simulation_trials, input.threads, input.seed);

        this->run(swaptions, input.swaptions, fileName);

        delete[] swaptions;
    }

    void run(parm *swaptions, int nSwaptions, const char *fileName)
    {
        std::string test_file_content = read_test_file(fileName);
        std::string simulation_output = swaptions_to_string(swaptions, nSwaptions);

        if (test_file_content == simulation_output)
        {
            std::cout << fileName << " passed" << std::endl;
        }
        else
        {
            std::cout << "error on " << fileName << std::endl;
        }
    }
};

#endif // TESTS_HPP