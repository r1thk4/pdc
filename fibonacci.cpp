#include <iostream>
#include <omp.h>
using namespace std;

// ---------- Serial Recursive Fibonacci ----------
long long serialFib(int n) {
    if (n <= 1)
        return n;
    return serialFib(n - 1) + serialFib(n - 2);
}

// ---------- Parallel Recursive Fibonacci ----------
long long parallelFib(int n) {
    if (n <= 20) // base case threshold to avoid too many threads
        return serialFib(n);

    long long x, y;

    #pragma omp task shared(x)
    x = parallelFib(n - 1);

    #pragma omp task shared(y)
    y = parallelFib(n - 2);

    #pragma omp taskwait
    return x + y;
}

int main() {
    int n;
    cout << "Enter n: ";
    cin >> n;

    // ----- Serial -----
    double start = omp_get_wtime();
    long long serialResult = serialFib(n);
    double end = omp_get_wtime();
    cout << "Serial Fibonacci(" << n << ") = " << serialResult << endl;
    cout << "Serial Time: " << (end - start) << " seconds\n\n";

    // ----- Parallel -----
    long long parallelResult;
    start = omp_get_wtime();
    #pragma omp parallel
    {
        #pragma omp single
        {
            parallelResult = parallelFib(n);
        }
    }
    end = omp_get_wtime();
    cout << "Parallel Fibonacci(" << n << ") = " << parallelResult << endl;
    cout << "Parallel Time: " << (end - start) << " seconds\n";

    return 0;
}
