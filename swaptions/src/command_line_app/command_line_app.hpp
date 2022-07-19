#ifndef SWAPTIONS_COMMAND_LINE_APP_HPP
#define SWAPTIONS_COMMAND_LINE_APP_HPP

typedef struct InputCommandLine
{
    int swaptions;
    int simulation_trials;
    int threads;
    long seed;

} InputCommandLine;

class SwaptionsCommandLineApp
{
private:
    int argc;
    char **argv;
    int max_threads;
    int min_threads;
    InputCommandLine default_input;
    void print_usage();
    InputCommandLine parse();

public:
    SwaptionsCommandLineApp(int max_threads, int min_threads, InputCommandLine default_input);
    ~SwaptionsCommandLineApp();
    InputCommandLine get_parameters(int argc, char *argv[]);
};

SwaptionsCommandLineApp::SwaptionsCommandLineApp(int max_threads, int min_threads, InputCommandLine default_input)
{
    this->max_threads = max_threads;
    this->min_threads = min_threads;
    this->default_input = default_input;
}

SwaptionsCommandLineApp::~SwaptionsCommandLineApp()
{
}

InputCommandLine SwaptionsCommandLineApp::get_parameters(int argc, char *argv[])
{
    this->argc = argc;
    this->argv = argv;

    if (this->argc == 1)
    {
        print_usage();
        exit(1);
    }

    auto input = parse();

    if (input.swaptions < input.threads)
    {
        fprintf(stderr, "Error: Fewer swaptions than threads. %i < %i \n", input.swaptions, input.threads);
        print_usage();
        exit(1);
    }
    if ((input.threads < this->min_threads) || (input.threads > this->max_threads))
    {
        fprintf(stderr, "Number of threads must be between %d and %d.\n", this->min_threads, this->max_threads);
        exit(1);
    }

    return input;
}

InputCommandLine SwaptionsCommandLineApp::parse()
{
    InputCommandLine input = this->default_input;

    for (int j = 1; j < this->argc; j++)
    {
        if (!strcmp("-sm", this->argv[j]))
        {
            input.simulation_trials = atoi(this->argv[++j]);
        }
        else if (!strcmp("-nt", this->argv[j]))
        {
            input.threads = atoi(this->argv[++j]);
        }
        else if (!strcmp("-ns", this->argv[j]))
        {
            input.swaptions = atoi(this->argv[++j]);
        }
        else if (!strcmp("-sd", this->argv[j]))
        {
            input.seed = atoi(this->argv[++j]);
        }
        else
        {
            fprintf(stderr, "Error: Unknown option: %s\n", argv[j]);
            print_usage();
            exit(1);
        }
    }

    return input;
}

void SwaptionsCommandLineApp::print_usage()
{
    fprintf(stderr, "Usage: %s OPTION [OPTIONS]...\n", this->argv[0]);
    fprintf(stderr, "Options:\n");
    fprintf(stderr, "\t-ns [number of swaptions (should be > number of threads]\n");
    fprintf(stderr, "\t-sm [number of simulation trails]\n");
    fprintf(stderr, "\t-nt [number of threads]\n");
    fprintf(stderr, "\t-sd [random number seed]\n");
}

#endif // SWAPTIONS_COMMAND_LINE_APP_HPP