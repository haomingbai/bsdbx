#include "monitor.h"
#include "rule.h"
#include <exception>
#include <future>
#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <sys/resource.h>
#include <unistd.h>
#include <vector>

// Memory limit in KB
// Time limit in miliseconds

int main(int argc, char **argv, char **envp)
{
    // Process the args manually
    bool mode = 0;
    int removeArgNum = 0;
    int findMode = 0;
    int findTimeLimit = 0;
    int timeLimit = 0;
    int memoryLimit = 0;
    int findMemoryLimit = 0;

    // Fetch the mode.
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

    // Fetch the time limit.
    for (size_t i = 0; i < argc; i++)
    {
        std::string_view str(argv[i]);
        if (str == "--time-limit")
        {
            findTimeLimit = 2;
            if (i >= argc - 1)
            {
                throw std::invalid_argument("Missing argument for sandbox mode");
            }
            i++;
            std::string_view m(argv[i]);
            int t = std::stoi(m.data());
            timeLimit = t;
            break;
        }
        else if (str.substr(0, 13) == "--time-limit=")
        {
            findTimeLimit = 1;
            auto s = str.substr(13);
            int t = std::stoi(s.data());
            timeLimit = t;
            break;
        }
        else if (str == "-t")
        {
            findTimeLimit = 2;
            if (i >= argc - 1)
            {
                throw std::invalid_argument("Missing argument for sandbox mode");
            }
            i++;
            std::string_view m(argv[i]);
            int t = std::stoi(m.data());
            timeLimit = t;
            break;
        }
    }

    // Fetch the memory limit.
    for (size_t i = 0; i < argc; i++)
    {
        std::string_view str(argv[i]);
        if (str == "--memory-limit")
        {
            findMemoryLimit = 2;
            if (i >= argc - 1)
            {
                throw std::invalid_argument("Missing argument for sandbox mode");
            }
            i++;
            std::string_view m(argv[i]);
            int t = std::stoi(m.data());
            memoryLimit = t;
            break;
        }
        else if (str.substr(0, 15) == "--memory-limit=")
        {
            findMemoryLimit = 1;
            auto s = str.substr(15);
            int t = std::stoi(s.data());
            memoryLimit = t;
            break;
        }
    }

    // Test whether there possibly exists an executable path.
    if (argc - findMode - findTimeLimit <= 1)
    {
        throw std::invalid_argument("No executable file");
    }

    // Copy args.
    auto args = new char *[argc]();
    for (size_t i = 0, flag = 0, moFlag = 0, memFlag = 0, tFlag = 0;
         i < argc - 1 - findMode - findTimeLimit - findMemoryLimit; i++)
    {
        std::string_view str = argv[i + 1 + flag];
        if (!moFlag && (str == "-m" || str == "--mode" || str.substr(0, 7) == "--mode="))
        {
            flag += findMode;
            moFlag = 1;
            str = argv[i + 1 + flag];
        }
        if (!tFlag && (str == "-t" || str == "--time-limit" || str.substr(0, 13) == "--time-limit="))
        {
            flag += findTimeLimit;
            tFlag = 1;
            str = argv[i + 1 + flag];
        }
        if (!memFlag && (str == "--memory-limit" || str.substr(0, 15) == "--memory-limit="))
        {
            flag += findMemoryLimit;
            memFlag = 1;
            str = argv[i + 1 + flag];
        }
        if (i < argc - 1 - findMode - findTimeLimit - findMemoryLimit)
        {
            args[i] = argv[i + 1 + flag];
        }
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

    auto pid = fork();

    if (pid == 0)
    {
        // bsdbx::loadBanFork();
        execve(args[0], args, envp);
    }
    else if (pid > 0)
    {
        std::future<int> futureMemory, futureTime;
        if (memoryLimit)
        {
            futureMemory = std::async(std::launch::async, bsdbx::monitorMemoryUsage, memoryLimit, pid);
        }
        else
        {
            futureMemory = std::async(std::launch::async, bsdbx::monitorMemoryUsage, 1 << 30, pid);
        }

        if (timeLimit)
        {
            futureTime = std::async(std::launch::async, bsdbx::monitorTime, timeLimit, pid);
        }
        else
        {
            futureTime = std::async(std::launch::async, bsdbx::monitorTime, 1e9, pid);
        }

        int status = 0;
        waitpid(pid, &status, 0);
        int memory = futureMemory.get();
        int time = futureTime.get();

        if (memory != -1)
        {
            std::cerr << memory << std::endl;
        }
        else
        {
            std::cerr << "MLE" << std::endl;
        }

        if (time != -1)
        {
            std::cerr << time << std::endl;
        }
        else
        {
            std::cerr << "TLE" << std::endl;
        }

        if (time == -1 || memory == -1)
        {
            return -1;
        }
        else
        {
            return WIFEXITED(status) ? WEXITSTATUS(status) : -1;
        }
    }
    else
    {
        throw std::runtime_error("Failed to fork");
    }
}
