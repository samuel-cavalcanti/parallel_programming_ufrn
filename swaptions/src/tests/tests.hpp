#ifndef TESTS_HPP
#define TESTS_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include "../HJM_type.h"

class Tests
{
private:
    const char *testFile = "tests/test.txt";
    std::string read_test_file(const char *fileName);
    std::string swaptions_to_string(parm *swaptions, int nSwaptions);

public:
    Tests(/* args */);
    ~Tests();

    void run(parm *swaptions, int nSwaptions);
};

std::string Tests::swaptions_to_string(parm *swaptions, int nSwaptions)
{
    const char template_line[] = "Swaption %d: [SwaptionPrice: %.10lf StdError: %.10lf] \n";
    std::stringstream string_stream;
    /*
      template_line é um array com 56 elementos,
      onde SwaptionPrice e StdError tem 10 digitos cada.
    */
    char line[56 + 10 + 10];

    for (auto i = 0; i < nSwaptions; i++)
    {

        sprintf(line, template_line,
                i, swaptions[i].dSimSwaptionMeanPrice, swaptions[i].dSimSwaptionStdError);

        string_stream << line;
    }

    return string_stream.str();
}
std::string Tests::read_test_file(const char *fileName)
{
    auto stream = std::ifstream(fileName);
    std::stringstream ss;
    ss << stream.rdbuf();
    return ss.str();
}

void Tests::run(parm *swaptions, int nSwaptions)
{
    std::string test_file_content = read_test_file(testFile);
    std::string simulation_output = swaptions_to_string(swaptions, nSwaptions);

    if (test_file_content == simulation_output)
    {
        std::cout << "passed" << std::endl;
    }
    else
    {
        std::cout << "error" << std::endl;
    }
}

Tests::Tests(/* args */)
{
}

Tests::~Tests()
{
}

#endif // TESTS_HPP