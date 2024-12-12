#include <iostream>
#include <string>
#include <sstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <cstdlib>

void execute_command(const std::string& command) {
    std::istringstream stream(command);
    std::string word;
    std::vector<std::string> args;

    while (stream >> word) {
        args.push_back(word);
    }

    if (args[0] == "exit") {
        std::exit(0);
    }

    pid_t pid = fork();

    if (pid < 0) {
        std::cerr << "Fork failed!" << std::endl;
        std::exit(1);
    }

    if (pid == 0) {
        if (args[0] == "silent") {
            args.erase(args.begin());
            std::string log_file = std::to_string(getpid()) + ".log";
            int fd = open(log_file.c_str(), O_CREAT | O_WRONLY | O_APPEND, 0644);
            if (fd == -1) {
                std::cerr << "Failed to open log file" << std::endl;
                std::exit(1);
            }
            dup2(fd, STDOUT_FILENO);
            dup2(fd, STDERR_FILENO);
            close(fd);
        }

        const char* env[] = {("PATH=" + std::string(getenv("PATH")) + ":.").c_str(), nullptr};
        execvp(args[0].c_str(), const_cast<char* const*>(args.data()));
        std::cerr << "Command execution failed!" << std::endl;
        std::exit(1);
    } else {
        int status;
        waitpid(pid, &status, 0);
    }
}

int main() {
    std::string input;

    while (true) {
        std::cout << "$ ";
        std::getline(std::cin, input);
        if (input.empty()) continue;
        execute_command(input);
    }

    return 0;
}
