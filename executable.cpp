#include "cmdline.h"
#include "rule.h"
#include <unistd.h>

int main(int argc, char **argv, char **envp)
{
    // Create a parser object and add the required options
    cmdline::parser a;
    a.add<std::string>("mode", 'm', "Mode of operation, which can be either \"compiler\" or \"runner\"", false,
                       "runner");
    a.add<std::string>("path", 'p', "Path to the executable file", true);
    a.add<std::string>("output", 'o', "Output file path", false);

    // Parse the command line arguments
    a.parse_check(argc, argv);

    // Get the values of the options
    auto mode = a.get<std::string>("mode");
    auto path = a.get<std::string>("path");

    // Load the security rules based on the mode
    if (mode == "compiler")
    {
        auto r = bsdbx::loadCompilerRule();
        if (r < 0)
        {
            perror("Error loading compiler rule");
            return r;
        }
    }
    else if (mode == "runner")
    {
        auto r = bsdbx::loadRunnerRule(path.c_str());
        if (r < 0)
        {
            perror("Error loading runner rule");
            return r;
        }
    }
    else
    {
        std::cerr << "Invalid mode: " << mode << std::endl;
        return 1;
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
    args[a.rest().size() + 1] = nullptr;

    if (mode == "compiler" && a.exist("output"))
    {
        args[a.rest().size() + 1] = "-o";
        args[a.rest().size() + 2] = new char[a.get<std::string>("output").size() + 1];
        strcpy(args[a.rest().size() + 2], a.get<std::string>("output").c_str());
        args[a.rest().size() + 3] = nullptr;
    }

    // Execute the executable file
    auto result = execve(path.c_str(), args, envp);
    if (result)
    {
        perror("Error executing the file");
        return result;
    }
    
}
