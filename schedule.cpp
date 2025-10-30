#include <iostream>
#include <omp.h>
using namespace std;

int main() {
    int N = 100;
    int sum = 0;

    // Serial version
    for (int i = 1; i <= N; i++)
        sum += i;
    cout << "Serial Sum: " << sum << endl;

    // Parallel with schedule(static)
    sum = 0;
    #pragma omp parallel for schedule(static) reduction(+:sum)
    for (int i = 1; i <= N; i++)
        sum += i;
    cout << "Static: " << sum << endl;

    // Parallel with schedule(static, 5)
    sum = 0;
    #pragma omp parallel for schedule(static, 5) reduction(+:sum)
    for (int i = 1; i <= N; i++)
        sum += i;
    cout << "Static,5: " << sum << endl;

    // Parallel with schedule(dynamic)
    sum = 0;
    #pragma omp parallel for schedule(dynamic) reduction(+:sum)
    for (int i = 1; i <= N; i++)
        sum += i;
    cout << "Dynamic: " << sum << endl;

    // Parallel with schedule(dynamic, 5)
    sum = 0;
    #pragma omp parallel for schedule(dynamic, 5) reduction(+:sum)
    for (int i = 1; i <= N; i++)
        sum += i;
    cout << "Dynamic,5: " << sum << endl;

    // Parallel with schedule(guided)
    sum = 0;
    #pragma omp parallel for schedule(guided) reduction(+:sum)
    for (int i = 1; i <= N; i++)
        sum += i;
    cout << "Guided: " << sum << endl;

    // Parallel with schedule(guided, 5)
    sum = 0;
    #pragma omp parallel for schedule(guided, 5) reduction(+:sum)
    for (int i = 1; i <= N; i++)
        sum += i;
    cout << "Guided,5: " << sum << endl;

    return 0;
}
