#include "rule.h"
#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unistd.h>
#include <vector>

int main(int argc, char **argv, char **envp)
{
    // Process the args manually
    bool mode = 0;
    int findMode = 0;
    for (size_t i = 0; i < argc; i++)
    {
        std::string_view str(argv[i]);
        if (str == "--mode")
        {
            findMode = 2;
            if (i >= argc - 1)
            {
                throw std::invalid_argument("Missing argument for sandbox mode");
            }
            i++;
            std::string_view m(argv[i]);
            if (m == "compiler")
            {
                mode = 1;
            }
            else if (m == "runner")
            {
                mode = 0;
            }
            else
            {
                std::string ex = "Invalid mode: ";
                ex += m;
                throw std::invalid_argument(ex);
            }
            break;
        }
        else if (str.substr(0, 7) == "--mode=")
        {
            findMode = 1;
            if (str.substr(7) == "compiler")
            {
                mode = 1;
            }
            else if (str.substr(7) == "runner")
            {
                mode = 0;
            }
            else
            {
                std::string ex = "Invalid mode: ";
                ex += str.substr(7);
                throw std::invalid_argument(ex);
            }
            break;
            ;
        }
        else if (str == "-m")
        {
            findMode = 2;
            if (i >= argc - 1)
            {
                throw std::invalid_argument("Missing argument for sandbox mode");
            }
            i++;
            std::string_view m(argv[i]);
            if (m == "compiler")
            {
                mode = 1;
            }
            else if (m == "runner")
            {
                mode = 0;
            }
            else
            {
                std::string ex = "Invalid mode: ";
                ex += m;
                throw std::invalid_argument(ex);
            }
            break;
        }
    }

    // Test whether there possibly exists an executable path.
    if (argc - findMode <= 1)
    {
        throw std::invalid_argument("No executable file");
    }

    // Copy args.
    auto args = new char *[argc]();
    for (size_t i = 0, flag = 0; i < argc - 1 - findMode; i++)
    {
        std::string_view str = argv[i + 1];
        if (!flag && (str == "-m" || str == "--mode" || str.substr(0, 7) == "--mode="))
        {
            flag = findMode;
        }
        args[i] = argv[i + 1 + flag];
    }

    // Load security mode.
    if (mode)
    {
        bsdbx::loadCompilerRule();
    }
    else
    {
        bsdbx::loadRunnerRule(args[0]);
    }

    // Execute the command
    auto res = execve(args[0], args, envp);

    // If there is an error, process it.
    perror("Execution Fail");
    return res;
}
