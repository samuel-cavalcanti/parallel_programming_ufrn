#define MIN_THREAD 1

#ifdef ENABLE_THREADS
#define MAX_THREAD 1024
#else
#define MAX_THREAD 1
#endif // ENABLE_THREADS

#include <stdio.h>
#include <stdlib.h>
#include "command_line_app.h"

void print_usage(char *program_name);
InputCommandLine parse(int argc, char *argv[], InputCommandLine &default_input);

InputCommandLine get_parameters(InputCommandLine default_input, int argc, char *argv[])
{

    if (argc == 1)
    {
        print_usage(argv[0]);
        exit(1);
    }

    auto input = parse(argc,argv,default_input);

    if (input.swaptions < input.threads)
    {
        fprintf(stderr, "Error: Fewer swaptions than threads. %i < %i \n", input.swaptions, input.threads);
        print_usage(argv[0]);
        exit(1);
    }
    if ((input.threads < MIN_THREAD) || (input.threads > MAX_THREAD))
    {
        fprintf(stderr, "Number of threads must be between %d and %d.\n", MIN_THREAD, MAX_THREAD);
        exit(1);
    }

    return input;
}

InputCommandLine parse(int argc, char *argv[], InputCommandLine &default_input)
{
    InputCommandLine input = default_input;

    for (int j = 1; j < argc; j++)
    {
        if (!strcmp("-sm", argv[j]))
        {
            input.simulation_trials = atoi(argv[++j]);
        }
        else if (!strcmp("-nt", argv[j]))
        {
            input.threads = atoi(argv[++j]);
        }
        else if (!strcmp("-ns", argv[j]))
        {
            input.swaptions = atoi(argv[++j]);
        }
        else if (!strcmp("-sd", argv[j]))
        {
            input.seed = atoi(argv[++j]);
        }
        else
        {
            fprintf(stderr, "Error: Unknown option: %s\n", argv[j]);
            print_usage(argv[0]);
            exit(1);
        }
    }

    return input;
}

void print_usage(char *program_name)
{
    fprintf(stderr, "Usage: %s OPTION [OPTIONS]...\n", program_name);
    fprintf(stderr, "Options:\n");
    fprintf(stderr, "\t-ns [number of swaptions (should be > number of threads]\n");
    fprintf(stderr, "\t-sm [number of simulation trails]\n");
    fprintf(stderr, "\t-nt [number of threads]\n");
    fprintf(stderr, "\t-sd [random number seed]\n");
}