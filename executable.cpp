#include "cmdline.h"
#include "rule.h"
#include <unistd.h>

int main(int argc, char **argv)
{
    // Create a parser object and add the required options
    cmdline::parser a;
    a.add<std::string>("mode", 'm', "Mode of operation, which can be either \"compiler\" or \"runner\"", false,
                       "runner");
    a.add<std::string>("path", 'p', "Path to the executable file", true);

    // Parse the command line arguments
    a.parse_check(argc, argv);

    // Get the values of the options
    auto mode = a.get<std::string>("mode") == "compiler" ? 1 : 0;
    auto path = a.get<std::string>("path");

    // Load the security rules based on the mode
    if (mode)
    {
        return bsdbx::loadCompilerRule();
    }
    else
    {
        return bsdbx::loadRunnerRule(path.c_str());
    }

    // Prepare the arguments for the execve system call
    char **args = new char *[argc];
    args[0] = new char[path.size() + 1];
    strcpy(args[0], path.c_str());
    for (int i = 0; i < a.rest().size(); i++)
    {
        args[i + 1] = new char[a.rest()[i].size() + 1];
        strcpy(args[i + 1], a.rest()[i].c_str());
    }

    // Execute the executable file
    execve(path.c_str(), args, nullptr);
}
