#include <iostream>
#include <csignal>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <cstring>

using namespace std;

void sigusr1_handler(int signum, siginfo_t *info, void *context) {
    pid_t sender_pid = info->si_pid;
    uid_t sender_uid = info->si_uid;
    struct passwd *pwd = getpwuid(sender_uid);
    string sender_username = (pwd != nullptr) ? pwd->pw_name : "Unknown";

    unsigned long eip, eax, ebx;

    asm volatile(
        "mov %%eip, %0\n\t"
        "mov %%eax, %1\n\t"
        "mov %%ebx, %2\n\t"
        : "=r" (eip), "=r" (eax), "=r" (ebx)
        :
        : "%eax", "%ebx"
    );

    cout << "Received a SIGUSR1 signal from process " << sender_pid 
         << " executed by " << sender_uid << " (" << sender_username << ")." << endl;
    cout << "State of the context: EIP = " << eip 
         << ", EAX = " << eax << ", EBX = " << ebx << endl;
}

int main(void) {
    cout << "Process PID: " << getpid() << endl;

    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = sigusr1_handler;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGUSR1, &sa, nullptr) == -1) {
        cerr << "Error setting up SIGUSR1 handler" << endl;
        return 1;
    }

    while (true) {
        sleep(10);
    }

    return 0;
}
