#ifndef MONITOR_H
#define MONITOR_H

#include <signal.h>
#include <stdio.h>
#include <sys/wait.h>
#include <time.h>

namespace bsdbx
{

/**
 * @brief Monitors a process for a specified time limit and terminates it if it is still running.
 *
 * This function repeatedly checks if a process with the given PID is still running, up to a specified time limit.
 * If the process is still running after the time limit, it sends a SIGKILL signal to terminate the process.
 *
 * @param timeLimit The maximum amount of time (in miliseconds) to monitor the process.
 * @param pid The process ID of the process to monitor.
 * @param status A reference to an integer where the status of the process will be stored.
 * @return Returns 0 if the process terminates before the time limit, or 1 if the process is terminated by this
 * function.
 * @note This function uses nanosleep to wait for short intervals between checks.
 * @note The function is marked noexcept, indicating it does not throw exceptions.
 */
int monitorTime(int timeLimit, int pid) noexcept
{
    for (int i = 0; i < timeLimit; i++)
    {
        timespec spec;
        spec.tv_nsec = 1e6 - 20;
        spec.tv_sec = 0;
        nanosleep(&spec, nullptr);
        if (kill(pid, 0) == -1)
        {
            return i;
        }
    }
    kill(pid, SIGKILL);
    return -1;
}

/**
 * @brief Monitors the memory usage of a process and terminates it if it exceeds a specified limit.
 *
 * This function continuously checks the memory usage of a process identified by its PID.
 * If the memory usage exceeds the specified limit, the process is terminated.
 *
 * @param memoryLimit The memory limit in kilobytes. If the process exceeds this limit, it will be terminated.
 * @param pid The process ID of the process to monitor.
 * @return The maximum memory usage in kilobytes observed during the monitoring period.
 *         Returns -1 if the process is terminated due to exceeding the memory limit.
 *
 * @note This function uses the /proc filesystem to read the memory usage of the process.
 *       It assumes a Linux environment where the /proc filesystem is available.
 * @note The function is marked as noexcept, indicating that it does not throw exceptions.
 */
int monitorMemoryUsage(int memoryLimit, int pid) noexcept
{
    long PAGE_SIZE = sysconf(_SC_PAGESIZE);
    int maxMemory = 0;
    while (kill(pid, 0) == 0)
    {
        char path[256];
        snprintf(path, sizeof(path), "/proc/%d/statm", pid);
        FILE *file = fopen(path, "r");
        if (file == NULL)
        {
            // perror("fopen");
            break;
        }
        long size, resident, share, text, lib, data, dt;
        if (fscanf(file, "%ld %ld %ld %ld %ld %ld %ld", &size, &resident, &share, &text, &lib, &data, &dt) != 7)
        {
            // perror("fscanf");
            fclose(file);
            break;
        }
        fclose(file);
        long rss_in_bytes = resident * PAGE_SIZE;
        long rss_in_kb = rss_in_bytes / 1024;
        if (rss_in_kb > maxMemory)
        {
            maxMemory = rss_in_kb;
        }
        if (maxMemory > memoryLimit)
        {
            kill(pid, SIGKILL);
            return -1;
        }
    }
    return maxMemory;
}
} // namespace bsdbx

#endif // MONITOR_H