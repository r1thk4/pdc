#include <iostream>
#include <vector>
#include <cmath>
#include <omp.h>

using namespace std;

void gaussian_elimination_parallel(vector<vector<double>> &A, vector<double> &b)
{
    int n = A.size();

    for (int k = 0; k < n; k++)
    {
        // Pivoting (serial, since it requires comparison)
        int maxRow = k;
        for (int i = k + 1; i < n; i++)
        {
            if (fabs(A[i][k]) > fabs(A[maxRow][k]))
                maxRow = i;
        }
        swap(A[k], A[maxRow]);
        swap(b[k], b[maxRow]);

// Parallel elimination
#pragma omp parallel for
        for (int i = k + 1; i < n; i++)
        {
            double factor = A[i][k] / A[k][k];
            for (int j = k; j < n; j++)
            {
                A[i][j] -= factor * A[k][j];
            }
            b[i] -= factor * b[k];
            printf("Thread %d updated row %d\n", omp_get_thread_num(), i);
        }
    }
}

vector<double> back_substitution(vector<vector<double>> &A, vector<double> &b)
{
    int n = A.size();
    vector<double> x(n);

    for (int i = n - 1; i >= 0; i--)
    {
        x[i] = b[i];
        for (int j = i + 1; j < n; j++)
        {
            x[i] -= A[i][j] * x[j];
        }
        x[i] /= A[i][i];
    }
    return x;
}

int main()
{
    int N = 3;
    vector<vector<double>> A = {
        {1, -1, 1},
        {1, -4, 2},
        {1, 2, 8}};
    vector<double> b = {4, 8, 12};

    omp_set_num_threads(4); // Example: use 4 threads
    double start = omp_get_wtime();
    gaussian_elimination_parallel(A, b);
    vector<double> x = back_substitution(A, b);
    double end = omp_get_wtime();

    cout << "Parallel Solution:\n";
    for (int i = 0; i < N; i++)
    {
        cout << "x[" << i << "] = " << x[i] << "\n";
    }
    cout << "Execution time (parallel): " << (end - start) << " seconds\n";

    return 0;
}