#ifndef SWAPTIONS_COMMAND_LINE_APP_H
#define SWAPTIONS_COMMAND_LINE_APP_H

#include <string.h>

typedef struct
{
    int swaptions;
    int simulation_trials;
    int threads;
    long seed;

} InputCommandLine;

InputCommandLine get_parameters(InputCommandLine default_input, int argc, char *argv[]);

#endif // SWAPTIONS_COMMAND_LINE_APP_H