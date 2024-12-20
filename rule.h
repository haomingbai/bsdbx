#ifndef RULE_H
#define RULE_H

#include <fcntl.h>
#include <seccomp.h>
#include <string>
#include <unistd.h>

namespace bsdbx
{
/**
 * @brief Loads a general seccomp rule that whitelists a predefined set of system calls.
 *
 * This function initializes a seccomp filter context with a default action of killing the process.
 * It then adds a whitelist of system calls that are allowed to be executed. If any of the system
 * calls cannot be added to the whitelist, the function releases the seccomp context and returns
 * the error code. Finally, it loads the seccomp rules into the kernel and releases the context.
 *
 * @return int Returns 0 on success, or a negative error code on failure.
 */
inline int loadGeneralRule() noexcept
{
    // Define a whitelist of actions
    int actions[] = {SCMP_SYS(_llseek),
                     SCMP_SYS(_newselect),
                     SCMP_SYS(accept),
                     SCMP_SYS(accept4),
                     SCMP_SYS(access),
                     SCMP_SYS(acct),
                     SCMP_SYS(adjtimex),
                     SCMP_SYS(alarm),
                     SCMP_SYS(arch_prctl),
                     SCMP_SYS(arm_fadvise64_64),
                     SCMP_SYS(arm_sync_file_range),
                     SCMP_SYS(bind),
                     SCMP_SYS(bpf),
                     SCMP_SYS(breakpoint),
                     SCMP_SYS(brk),
                     SCMP_SYS(cacheflush),
                     SCMP_SYS(cachestat),
                     SCMP_SYS(capget),
                     SCMP_SYS(capset),
                     SCMP_SYS(chdir),
                     SCMP_SYS(chmod),
                     SCMP_SYS(chown),
                     SCMP_SYS(chown32),
                     SCMP_SYS(chroot),
                     SCMP_SYS(clock_adjtime),
                     SCMP_SYS(clock_adjtime64),
                     SCMP_SYS(clock_getres),
                     SCMP_SYS(clock_getres_time64),
                     SCMP_SYS(clock_gettime),
                     SCMP_SYS(clock_gettime64),
                     SCMP_SYS(clock_nanosleep),
                     SCMP_SYS(clock_nanosleep_time64),
                     SCMP_SYS(clock_settime),
                     SCMP_SYS(clock_settime64),
                     SCMP_SYS(clone),
                     SCMP_SYS(clone3),
                     SCMP_SYS(close),
                     SCMP_SYS(close_range),
                     SCMP_SYS(connect),
                     SCMP_SYS(copy_file_range),
                     SCMP_SYS(creat),
                     SCMP_SYS(delete_module),
                     SCMP_SYS(dup),
                     SCMP_SYS(dup2),
                     SCMP_SYS(dup3),
                     SCMP_SYS(epoll_create),
                     SCMP_SYS(epoll_create1),
                     SCMP_SYS(epoll_ctl),
                     SCMP_SYS(epoll_ctl_old),
                     SCMP_SYS(epoll_pwait),
                     SCMP_SYS(epoll_pwait2),
                     SCMP_SYS(epoll_wait),
                     SCMP_SYS(epoll_wait_old),
                     SCMP_SYS(eventfd),
                     SCMP_SYS(eventfd2),
                     SCMP_SYS(execve),
                     SCMP_SYS(execveat),
                     SCMP_SYS(exit),
                     SCMP_SYS(exit_group),
                     SCMP_SYS(faccessat),
                     SCMP_SYS(faccessat2),
                     SCMP_SYS(fadvise64),
                     SCMP_SYS(fadvise64_64),
                     SCMP_SYS(fallocate),
                     SCMP_SYS(fanotify_init),
                     SCMP_SYS(fanotify_mark),
                     SCMP_SYS(fchdir),
                     SCMP_SYS(fchmod),
                     SCMP_SYS(fchmodat),
                     SCMP_SYS(fchmodat2),
                     SCMP_SYS(fchown),
                     SCMP_SYS(fchown32),
                     SCMP_SYS(fchownat),
                     SCMP_SYS(fcntl),
                     SCMP_SYS(fcntl64),
                     SCMP_SYS(fdatasync),
                     SCMP_SYS(fgetxattr),
                     SCMP_SYS(finit_module),
                     SCMP_SYS(flistxattr),
                     SCMP_SYS(flock),
                     SCMP_SYS(fork),
                     SCMP_SYS(fremovexattr),
                     SCMP_SYS(fsconfig),
                     SCMP_SYS(fsetxattr),
                     SCMP_SYS(fsmount),
                     SCMP_SYS(fsopen),
                     SCMP_SYS(fspick),
                     SCMP_SYS(fstat),
                     SCMP_SYS(fstat64),
                     SCMP_SYS(fstatat64),
                     SCMP_SYS(fstatfs),
                     SCMP_SYS(fstatfs64),
                     SCMP_SYS(fsync),
                     SCMP_SYS(ftruncate),
                     SCMP_SYS(ftruncate64),
                     SCMP_SYS(futex),
                     SCMP_SYS(futex_requeue),
                     SCMP_SYS(futex_time64),
                     SCMP_SYS(futex_wait),
                     SCMP_SYS(futex_waitv),
                     SCMP_SYS(futex_wake),
                     SCMP_SYS(futimesat),
                     SCMP_SYS(get_mempolicy),
                     SCMP_SYS(get_robust_list),
                     SCMP_SYS(get_thread_area),
                     SCMP_SYS(getcpu),
                     SCMP_SYS(getcwd),
                     SCMP_SYS(getdents),
                     SCMP_SYS(getdents64),
                     SCMP_SYS(getegid),
                     SCMP_SYS(getegid32),
                     SCMP_SYS(geteuid),
                     SCMP_SYS(geteuid32),
                     SCMP_SYS(getgid),
                     SCMP_SYS(getgid32),
                     SCMP_SYS(getgroups),
                     SCMP_SYS(getgroups32),
                     SCMP_SYS(getitimer),
                     SCMP_SYS(getpeername),
                     SCMP_SYS(getpgid),
                     SCMP_SYS(getpgrp),
                     SCMP_SYS(getpid),
                     SCMP_SYS(getppid),
                     SCMP_SYS(getpriority),
                     SCMP_SYS(getrandom),
                     SCMP_SYS(getresgid),
                     SCMP_SYS(getresgid32),
                     SCMP_SYS(getresuid),
                     SCMP_SYS(getresuid32),
                     SCMP_SYS(getrlimit),
                     SCMP_SYS(getrusage),
                     SCMP_SYS(getsid),
                     SCMP_SYS(getsockname),
                     SCMP_SYS(getsockopt),
                     SCMP_SYS(gettid),
                     SCMP_SYS(gettimeofday),
                     SCMP_SYS(getuid),
                     SCMP_SYS(getuid32),
                     SCMP_SYS(getxattr),
                     SCMP_SYS(init_module),
                     SCMP_SYS(inotify_add_watch),
                     SCMP_SYS(inotify_init),
                     SCMP_SYS(inotify_init1),
                     SCMP_SYS(inotify_rm_watch),
                     SCMP_SYS(io_cancel),
                     SCMP_SYS(io_destroy),
                     SCMP_SYS(io_getevents),
                     SCMP_SYS(io_pgetevents),
                     SCMP_SYS(io_pgetevents_time64),
                     SCMP_SYS(io_setup),
                     SCMP_SYS(io_submit),
                     SCMP_SYS(ioctl),
                     SCMP_SYS(ioperm),
                     SCMP_SYS(iopl),
                     SCMP_SYS(ioprio_get),
                     SCMP_SYS(ioprio_set),
                     SCMP_SYS(ipc),
                     SCMP_SYS(kcmp),
                     SCMP_SYS(kill),
                     SCMP_SYS(landlock_add_rule),
                     SCMP_SYS(landlock_create_ruleset),
                     SCMP_SYS(landlock_restrict_self),
                     SCMP_SYS(lchown),
                     SCMP_SYS(lchown32),
                     SCMP_SYS(lgetxattr),
                     SCMP_SYS(link),
                     SCMP_SYS(linkat),
                     SCMP_SYS(listen),
                     SCMP_SYS(listxattr),
                     SCMP_SYS(llistxattr),
                     SCMP_SYS(lookup_dcookie),
                     SCMP_SYS(lremovexattr),
                     SCMP_SYS(lseek),
                     SCMP_SYS(lsetxattr),
                     SCMP_SYS(lstat),
                     SCMP_SYS(lstat64),
                     SCMP_SYS(madvise),
                     SCMP_SYS(map_shadow_stack),
                     SCMP_SYS(mbind),
                     SCMP_SYS(membarrier),
                     SCMP_SYS(memfd_create),
                     SCMP_SYS(memfd_secret),
                     SCMP_SYS(mincore),
                     SCMP_SYS(mkdir),
                     SCMP_SYS(mkdirat),
                     SCMP_SYS(mknod),
                     SCMP_SYS(mknodat),
                     SCMP_SYS(mlock),
                     SCMP_SYS(mlock2),
                     SCMP_SYS(mlockall),
                     SCMP_SYS(mmap),
                     SCMP_SYS(mmap2),
                     SCMP_SYS(modify_ldt),
                     SCMP_SYS(mount),
                     SCMP_SYS(mount_setattr),
                     SCMP_SYS(move_mount),
                     SCMP_SYS(mprotect),
                     SCMP_SYS(mq_getsetattr),
                     SCMP_SYS(mq_notify),
                     SCMP_SYS(mq_open),
                     SCMP_SYS(mq_timedreceive),
                     SCMP_SYS(mq_timedreceive_time64),
                     SCMP_SYS(mq_timedsend),
                     SCMP_SYS(mq_timedsend_time64),
                     SCMP_SYS(mq_unlink),
                     SCMP_SYS(mremap),
                     SCMP_SYS(msgctl),
                     SCMP_SYS(msgget),
                     SCMP_SYS(msgrcv),
                     SCMP_SYS(msgsnd),
                     SCMP_SYS(msync),
                     SCMP_SYS(munlock),
                     SCMP_SYS(munlockall),
                     SCMP_SYS(munmap),
                     SCMP_SYS(name_to_handle_at),
                     SCMP_SYS(nanosleep),
                     SCMP_SYS(newfstatat),
                     SCMP_SYS(open),
                     SCMP_SYS(open_by_handle_at),
                     SCMP_SYS(open_tree),
                     SCMP_SYS(openat),
                     SCMP_SYS(openat2),
                     SCMP_SYS(pause),
                     SCMP_SYS(perf_event_open),
                     SCMP_SYS(personality),
                     SCMP_SYS(pidfd_getfd),
                     SCMP_SYS(pidfd_open),
                     SCMP_SYS(pidfd_send_signal),
                     SCMP_SYS(pipe),
                     SCMP_SYS(pipe2),
                     SCMP_SYS(pkey_alloc),
                     SCMP_SYS(pkey_free),
                     SCMP_SYS(pkey_mprotect),
                     SCMP_SYS(poll),
                     SCMP_SYS(ppoll),
                     SCMP_SYS(ppoll_time64),
                     SCMP_SYS(prctl),
                     SCMP_SYS(pread64),
                     SCMP_SYS(preadv),
                     SCMP_SYS(preadv2),
                     SCMP_SYS(prlimit64),
                     SCMP_SYS(process_madvise),
                     SCMP_SYS(process_mrelease),
                     SCMP_SYS(process_vm_readv),
                     SCMP_SYS(process_vm_writev),
                     SCMP_SYS(pselect6),
                     SCMP_SYS(pselect6_time64),
                     SCMP_SYS(ptrace),
                     SCMP_SYS(pwrite64),
                     SCMP_SYS(pwritev),
                     SCMP_SYS(pwritev2),
                     SCMP_SYS(quotactl),
                     SCMP_SYS(quotactl_fd),
                     SCMP_SYS(read),
                     SCMP_SYS(readahead),
                     SCMP_SYS(readlink),
                     SCMP_SYS(readlinkat),
                     SCMP_SYS(readv),
                     SCMP_SYS(reboot),
                     SCMP_SYS(recv),
                     SCMP_SYS(recvfrom),
                     SCMP_SYS(recvmmsg),
                     SCMP_SYS(recvmmsg_time64),
                     SCMP_SYS(recvmsg),
                     SCMP_SYS(remap_file_pages),
                     SCMP_SYS(removexattr),
                     SCMP_SYS(rename),
                     SCMP_SYS(renameat),
                     SCMP_SYS(renameat2),
                     SCMP_SYS(restart_syscall),
                     SCMP_SYS(riscv_flush_icache),
                     SCMP_SYS(rmdir),
                     SCMP_SYS(rseq),
                     SCMP_SYS(rt_sigaction),
                     SCMP_SYS(rt_sigpending),
                     SCMP_SYS(rt_sigprocmask),
                     SCMP_SYS(rt_sigqueueinfo),
                     SCMP_SYS(rt_sigreturn),
                     SCMP_SYS(rt_sigsuspend),
                     SCMP_SYS(rt_sigtimedwait),
                     SCMP_SYS(rt_sigtimedwait_time64),
                     SCMP_SYS(rt_tgsigqueueinfo),
                     SCMP_SYS(s390_pci_mmio_read),
                     SCMP_SYS(s390_pci_mmio_write),
                     SCMP_SYS(s390_runtime_instr),
                     SCMP_SYS(sched_get_priority_max),
                     SCMP_SYS(sched_get_priority_min),
                     SCMP_SYS(sched_getaffinity),
                     SCMP_SYS(sched_getattr),
                     SCMP_SYS(sched_getparam),
                     SCMP_SYS(sched_getscheduler),
                     SCMP_SYS(sched_rr_get_interval),
                     SCMP_SYS(sched_rr_get_interval_time64),
                     SCMP_SYS(sched_setaffinity),
                     SCMP_SYS(sched_setattr),
                     SCMP_SYS(sched_setparam),
                     SCMP_SYS(sched_setscheduler),
                     SCMP_SYS(sched_yield),
                     SCMP_SYS(seccomp),
                     SCMP_SYS(select),
                     SCMP_SYS(semctl),
                     SCMP_SYS(semget),
                     SCMP_SYS(semop),
                     SCMP_SYS(semtimedop),
                     SCMP_SYS(semtimedop_time64),
                     SCMP_SYS(send),
                     SCMP_SYS(sendfile),
                     SCMP_SYS(sendfile64),
                     SCMP_SYS(sendmmsg),
                     SCMP_SYS(sendmsg),
                     SCMP_SYS(sendto),
                     SCMP_SYS(set_mempolicy),
                     SCMP_SYS(set_mempolicy_home_node),
                     SCMP_SYS(set_robust_list),
                     SCMP_SYS(set_thread_area),
                     SCMP_SYS(set_tid_address),
                     SCMP_SYS(set_tls),
                     SCMP_SYS(setdomainname),
                     SCMP_SYS(setfsgid),
                     SCMP_SYS(setfsgid32),
                     SCMP_SYS(setfsuid),
                     SCMP_SYS(setfsuid32),
                     SCMP_SYS(setgid),
                     SCMP_SYS(setgid32),
                     SCMP_SYS(setgroups),
                     SCMP_SYS(setgroups32),
                     SCMP_SYS(sethostname),
                     SCMP_SYS(setitimer),
                     SCMP_SYS(setns),
                     SCMP_SYS(setpgid),
                     SCMP_SYS(setpriority),
                     SCMP_SYS(setregid),
                     SCMP_SYS(setregid32),
                     SCMP_SYS(setresgid),
                     SCMP_SYS(setresgid32),
                     SCMP_SYS(setresuid),
                     SCMP_SYS(setresuid32),
                     SCMP_SYS(setreuid),
                     SCMP_SYS(setreuid32),
                     SCMP_SYS(setrlimit),
                     SCMP_SYS(setsid),
                     SCMP_SYS(setsockopt),
                     SCMP_SYS(settimeofday),
                     SCMP_SYS(setuid),
                     SCMP_SYS(setuid32),
                     SCMP_SYS(setxattr),
                     SCMP_SYS(shmat),
                     SCMP_SYS(shmctl),
                     SCMP_SYS(shmdt),
                     SCMP_SYS(shmget),
                     SCMP_SYS(shutdown),
                     SCMP_SYS(sigaltstack),
                     SCMP_SYS(signalfd),
                     SCMP_SYS(signalfd4),
                     SCMP_SYS(sigprocmask),
                     SCMP_SYS(sigreturn),
                     SCMP_SYS(socket),
                     SCMP_SYS(socketcall),
                     SCMP_SYS(socketpair),
                     SCMP_SYS(splice),
                     SCMP_SYS(stat),
                     SCMP_SYS(stat64),
                     SCMP_SYS(statfs),
                     SCMP_SYS(statfs64),
                     SCMP_SYS(statx),
                     SCMP_SYS(stime),
                     SCMP_SYS(swapcontext),
                     SCMP_SYS(symlink),
                     SCMP_SYS(symlinkat),
                     SCMP_SYS(sync),
                     SCMP_SYS(sync_file_range),
                     SCMP_SYS(sync_file_range2),
                     SCMP_SYS(syncfs),
                     SCMP_SYS(sysinfo),
                     SCMP_SYS(syslog),
                     SCMP_SYS(tee),
                     SCMP_SYS(tgkill),
                     SCMP_SYS(time),
                     SCMP_SYS(timer_create),
                     SCMP_SYS(timer_delete),
                     SCMP_SYS(timer_getoverrun),
                     SCMP_SYS(timer_gettime),
                     SCMP_SYS(timer_gettime64),
                     SCMP_SYS(timer_settime),
                     SCMP_SYS(timer_settime64),
                     SCMP_SYS(timerfd_create),
                     SCMP_SYS(timerfd_gettime),
                     SCMP_SYS(timerfd_gettime64),
                     SCMP_SYS(timerfd_settime),
                     SCMP_SYS(timerfd_settime64),
                     SCMP_SYS(times),
                     SCMP_SYS(tkill),
                     SCMP_SYS(truncate),
                     SCMP_SYS(truncate64),
                     SCMP_SYS(ugetrlimit),
                     SCMP_SYS(umask),
                     SCMP_SYS(umount),
                     SCMP_SYS(umount2),
                     SCMP_SYS(uname),
                     SCMP_SYS(unlink),
                     SCMP_SYS(unlinkat),
                     SCMP_SYS(unshare),
                     SCMP_SYS(utime),
                     SCMP_SYS(utimensat),
                     SCMP_SYS(utimensat_time64),
                     SCMP_SYS(utimes),
                     SCMP_SYS(vfork),
                     SCMP_SYS(vhangup),
                     SCMP_SYS(vmsplice),
                     SCMP_SYS(wait4),
                     SCMP_SYS(waitid),
                     SCMP_SYS(waitpid),
                     SCMP_SYS(write),
                     SCMP_SYS(writev)};

    // Add the rule in the whitelist
    auto context = seccomp_init(SCMP_ACT_KILL);
    for (auto action : actions)
    {
        int result =
            seccomp_rule_add(context, SCMP_ACT_ALLOW, action, 0); // Get the result of adding rule to the context
        if (result < 0)
        {
            seccomp_release(context);
            return result;
        }
    }

    // Load the rules
    auto result = seccomp_load(context); // Fetch the result of loading the rules
    seccomp_release(context);
    return result;
}

/**
 * @brief Loads and applies security rules for a runner.
 *
 * This function initializes a seccomp context and applies a set of security rules to restrict certain system calls.
 * It first loads general rules, then defines and bans specific actions, and finally adds rules related to file
 * execution.
 *
 * @param filename The name of the file to be executed, used in the execve rule.
 * @return Returns 0 on success, or a negative error code on failure.
 */
int loadRunnerRule(const char filename[])
{
    // Load the general rules
    auto preload = loadGeneralRule();
    if (preload < 0)
    {
        return preload;
    }

    // Define the banned actions
    int bannedActions[] = {SCMP_SYS(socket),    SCMP_SYS(setuid),   SCMP_SYS(setgid),   SCMP_SYS(setpgid),
                           SCMP_SYS(setsid),    SCMP_SYS(setreuid), SCMP_SYS(setregid), SCMP_SYS(setgroups),
                           SCMP_SYS(setrlimit), SCMP_SYS(vfork),    SCMP_SYS(chmod),    SCMP_SYS(chown),
                           SCMP_SYS(chown32),   SCMP_SYS(fchmod),   SCMP_SYS(fchown),   SCMP_SYS(fchownat),
                           SCMP_SYS(link),      SCMP_SYS(shutdown), SCMP_SYS(seccomp),  SCMP_SYS(rmdir),
                           SCMP_SYS(rename)};

    auto context = seccomp_init(SCMP_ACT_ALLOW);
    for (auto action : bannedActions)
    {
        auto result = seccomp_rule_add(context, SCMP_ACT_KILL, action, 0);
        if (result < 0)
        {
            seccomp_release(context);
            return result;
        }
    }

    // Add rules related to file execution
    int probe = 0;
    probe +=
        seccomp_rule_add(context, SCMP_ACT_KILL, SCMP_SYS(execve), 1, SCMP_A0(SCMP_CMP_NE, (scmp_datum_t)(filename)));
    probe +=
        seccomp_rule_add(context, SCMP_ACT_KILL, SCMP_SYS(open), 1, SCMP_CMP(1, SCMP_CMP_MASKED_EQ, O_RDWR, O_RDWR));
    probe += seccomp_rule_add(context, SCMP_ACT_KILL, SCMP_SYS(openat), 1,
                              SCMP_CMP(2, SCMP_CMP_MASKED_EQ, O_WRONLY, O_WRONLY));
    probe +=
        seccomp_rule_add(context, SCMP_ACT_KILL, SCMP_SYS(openat), 1, SCMP_CMP(2, SCMP_CMP_MASKED_EQ, O_RDWR, O_RDWR));

    if (probe < 0)
    {
        seccomp_release(context);
        return probe;
    }

    // Load the rules
    probe += seccomp_load(context);
    seccomp_release(context);
    return probe;
}

/**
 * @brief Loads the compiler rule by initializing and configuring seccomp rules.
 *
 * This function first loads the general rule using `loadGeneralRule()`. If the loading fails,
 * it returns the error code. Then, it defines a set of banned system calls and adds them to
 * the seccomp context with the action `SCMP_ACT_KILL`. Finally, it loads the seccomp rules
 * and releases the context.
 *
 * @return int Returns 0 on success, or a negative error code on failure.
 */
int loadCompilerRule()
{
    // Load the general rule
    int preload = loadGeneralRule();
    if (preload < 0)
    {
        return preload;
    }

    // Define the banned actions
    int bannedRules[] = {
        SCMP_SYS(socket),   SCMP_SYS(setuid),   SCMP_SYS(setgid),    SCMP_SYS(setpgid),   SCMP_SYS(setsid),
        SCMP_SYS(setreuid), SCMP_SYS(setregid), SCMP_SYS(setgroups), SCMP_SYS(setrlimit), SCMP_SYS(seccomp),
    };

    // Add the banned actions to the context
    auto context = seccomp_init(SCMP_ACT_ALLOW);
    for (auto action : bannedRules)
    {
        auto result = seccomp_rule_add(context, SCMP_ACT_KILL, action, 0);
        if (result < 0)
        {
            seccomp_release(context);
            return result;
        }
    }

    // Load the rules
    int result = seccomp_load(context);
    seccomp_release(context);
    return result;
}

int loadBanFork()
{
    auto context = seccomp_init(SCMP_ACT_ALLOW);
    auto result = seccomp_rule_add(context, SCMP_ACT_KILL, SCMP_SYS(fork), 0);
    if (result < 0)
    {
        seccomp_release(context);
        return result;
    }
    result = seccomp_load(context);
    seccomp_release(context);
    return result;
}
} // namespace bsdbx

#endif