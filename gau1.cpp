#include <iostream>
#include <vector>
#include <cmath>
#include <omp.h>

// Use the std namespace for cout, swap, etc.
using namespace std;

// The parallel version of your function
void parallelGaussian(vector<vector<double>> &A, vector<double> &B, int N) {
    // This outer 'k' loop MUST be serial
    for(int k=0; k<N; k++) {
        // --- Pivoting (Serial) ---
        // This part is very fast (O(N)) and has dependencies,
        // so we leave it serial.
        int maxRow=k;
        for(int i=k+1; i<N; i++) {
            if(fabs(A[i][k])>fabs(A[maxRow][k])) {
                maxRow=i;
            }
        }
        swap(A[k], A[maxRow]);
        swap(B[k], B[maxRow]);

        // --- Elimination (Parallel) ---
        // This is the O(N^3) part and the main bottleneck.
        // We can parallelize the 'i' loop because the calculation
        // for each row 'i' is independent of all other rows.
        
        #pragma omp parallel for schedule(static)
        for(int i=k+1; i<N; i++) {
            
            // 'f' is declared inside the parallel 'i' loop,
            // so it is automatically PRIVATE to each thread.
            double f = A[i][k]/A[k][k];
            
            // 'j' is also declared inside, so it is also PRIVATE.
            // This prevents the segmentation fault you saw earlier.
            for(int j=k; j<N; j++) {
                A[i][j] -= f*A[k][j];
            }
            B[i] -= f*B[k];
        }
        // An implicit barrier exists here: all threads
        // must finish the 'i' loop before moving to the
        // next 'k' iteration.
    }

    // --- Back Substitution (Serial) ---
    // This loop MUST be serial because solving for x[i]
    // depends on the value of x[i+1].
    vector<double> x(N, 0.0);
    for(int i=N-1; i>=0; i--) {
        x[i] = B[i];
        for(int j=i+1; j<N; j++) {
            x[i] -= A[i][j]*x[j];
        }
        x[i]/=A[i][i];
    }

    // --- Print (Serial) ---
    cout<<"Parallel Answer: "<<"\n";
    for(int i=0; i<N; i++) {
        cout<<x[i]<<" ";
    }
    cout<<endl;
}

// Your serial function (unchanged)
void serialGaussian(vector<vector<double>> &A, vector<double> &B, int N) {
    for(int k=0; k<N; k++) {
        int maxRow=k;
        for(int i=k+1; i<N; i++) {
            if(fabs(A[i][k])>fabs(A[maxRow][k])) {
                maxRow=i;
            }
        }
        swap(A[k], A[maxRow]);
        swap(B[k], B[maxRow]);

        for(int i=k+1; i<N;i++) {
            double f = A[i][k]/A[k][k];
            for(int j=k; j<N;j++) {
                A[i][j] -= f*A[k][j];
            }
            B[i] -= f*B[k];
        }
    }
    vector<double> x(N, 0.0);
    for(int i=N-1; i>=0; i--) {
        x[i] = B[i];
        for(int j=i+1; j<N; j++) {
            x[i] -= A[i][j]*x[j];
        }
        x[i]/=A[i][i];
    }
    cout<<"Serial Answer: "<<"\n";
    for(int i=0; i<N; i++) {
        cout<<x[i]<<" ";
    }
    cout<<endl;
}

// Main function to run the test
int main() {
    int N;
    cout << "Enter N (e.g., 3): ";
    cin >> N;
    
    vector<vector<double>> A(N, vector<double>(N, 0.0));
    vector<double> B(N, 0);

    // Use the example matrix from your previous question
    if (N == 3) {
        A = {
            {1, -1, 1},
            {1, -4, 2},
            {1,  2, 8}
        };
        B = {4, 8, 12};
        cout << "Using default 3x3 matrix.\n";
    } else {
         cout<<"Enter A matrix: "<<endl;
         for(int i=0; i<N; i++) {
             for(int j=0; j<N; j++) {
                 cin>>A[i][j];
             }
         }
         cout<<"Enter B: "<<endl;
         for(int i=0; i<N; i++) {
             cin>>B[i];
         }
    }

    // Create copies for serial and parallel versions
    vector<vector<double>> A_serial = A;
    vector<double> B_serial = B;
    
    vector<vector<double>> A_parallel = A;
    vector<double> B_parallel = B;

    cout << "\nRunning Serial Version...\n";
    double start_serial = omp_get_wtime();
    serialGaussian(A_serial, B_serial, N);
    double end_serial = omp_get_wtime();
    cout << "Serial Time: " << (end_serial - start_serial) << "s\n";

    cout << "\nRunning Parallel Version...\n";
    double start_parallel = omp_get_wtime();
    parallelGaussian(A_parallel, B_parallel, N);
    double end_parallel = omp_get_wtime();
    cout << "Parallel Time: " << (end_parallel - start_parallel) << "s\n";

    return 0;
}
