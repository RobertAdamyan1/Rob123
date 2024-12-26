#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>
#include <cstring>

using namespace std;

int calculateNthPrime(int n) {
    if (n < 1) return -1;
    int count = 0, num = 2;
    while (count < n) {
        bool isPrime = true;
        for (int i = 2; i * i <= num; ++i) {
            if (num % i == 0) {
                isPrime = false;
                break;
            }
        }
        if (isPrime) {
            count++;
        }
        num++;
    }
    return num - 1;
}

int main(void) {
    int parentToChild[2];
    int childToParent[2];

    if (pipe(parentToChild) == -1 || pipe(childToParent) == -1) {
        cerr << "Pipe creation failed!" << endl;
        return 1;
    }

    pid_t pid = fork();

    if (pid < 0) {
        cerr << "Fork failed!" << endl;
        return 1;
    }

    if (pid > 0) {
        close(parentToChild[0]);
        close(childToParent[1]);

        while (true) {
            cout << "[Parent] Please enter the number (or type 'exit' to quit): ";
            string input;
            getline(cin, input);

            if (input == "exit") {
                break;
            }

            int m;
            try {
                m = stoi(input);
            } catch (const invalid_argument&) {
                cerr << "[Parent] Invalid input! Please enter an integer." << endl;
                continue;
            }

            cout << "[Parent] Sending " << m << " to the child process..." << endl;
            write(parentToChild[1], &m, sizeof(m));

            cout << "[Parent] Waiting for the response from the child process..." << endl;

            int result;
            read(childToParent[0], &result, sizeof(result));

            cout << "[Parent] Received calculation result of prime(" << m << ") = " << result << "..." << endl;
        }

        close(parentToChild[1]);
        close(childToParent[0]);
        wait(nullptr);

    } else {
        close(parentToChild[1]);
        close(childToParent[0]);

        while (true) {
            int m;
            read(parentToChild[0], &m, sizeof(m));

            cout << "[Child] Calculating " << m << "-th prime number..." << endl;

            int result = calculateNthPrime(m);

            cout << "[Child] Sending calculation result of prime(" << m << ")..." << endl;
            write(childToParent[1], &result, sizeof(result));
        }

        close(parentToChild[0]);
        close(childToParent[1]);
    }

    return 0;
}
