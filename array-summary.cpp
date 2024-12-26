#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <ctime>

using namespace std;

long long sum_array(const vector<int>& arr, int start, int end) {
    long long sum = 0;
    for (int i = start; i < end; ++i) {
        sum += arr[i];
    }
    return sum;
}

void thread_sum(const vector<int>& arr, int start, int end, long long& result) {
    result = sum_array(arr, start, end);
}

int main(int argc, char** argv) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <N> <M>" << endl;
        return 1;
    }

    int N = stoi(argv[1]);
    int M = stoi(argv[2]);

    if (N <= 1 || M <= 0) {
        cerr << "N must be greater than 1 and M must be positive." << endl;
        return 1;
    }

    srand(time(nullptr));

    vector<int> arr(N);
    for (int i = 0; i < N; ++i) {
        arr[i] = rand() % 1000;
    }

    auto start_time = chrono::high_resolution_clock::now();
    long long sum_no_threads = 0;
    for (int i = 0; i < N; ++i) {
        sum_no_threads += arr[i];
    }
    auto end_time = chrono::high_resolution_clock::now();
    auto duration_no_threads = chrono::duration_cast<chrono::milliseconds>(end_time - start_time);
    cout << "Time spent without threads: " << duration_no_threads.count() << " ms" << endl;

    start_time = chrono::high_resolution_clock::now();

    int chunk_size = N / M;
    vector<thread> threads(M);
    vector<long long> partial_sums(M);

    for (int i = 0; i < M; ++i) {
        int start = i * chunk_size;
        int end = (i == M - 1) ? N : (i + 1) * chunk_size;
        threads[i] = thread(thread_sum, cref(arr), start, end, ref(partial_sums[i]));
    }

    for (int i = 0; i < M; ++i) {
        threads[i].join();
    }

    long long sum_with_threads = 0;
    for (long long partial_sum : partial_sums) {
        sum_with_threads += partial_sum;
    }

    end_time = chrono::high_resolution_clock::now();
    auto duration_with_threads = chrono::duration_cast<chrono::milliseconds>(end_time - start_time);
    cout << "Time spent with " << M << " threads: " << duration_with_threads.count() << " ms" << endl;

    return 0;
}
