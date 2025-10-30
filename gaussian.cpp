#include <iostream>
#include <cstdlib>
#include <omp.h>
#include <vector>
#include <cmath>

using namespace std;

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

void parallelGaussian(vector<vector<double>>&A, vector<double>&B, int N) {
    for(int k=0; k<N; k++) {
        int maxRow = k;
        for(int i=k+1; i<N; i++) {
            if(fabs(A[i][k])>fabs(A[maxRow][k])) {
                maxRow=i;
            }
        }
        swap(A[k], A[maxRow]);
        swap(B[k], B[maxRow]);
        
#pragma omp parallel for schedule(static)
        for(int i=k+1; i<N; i++) {
            double f = A[i][k]/A[k][k];
            for(int j=k; j<N; j++) {
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
        x[i] /= A[i][i];
    }

    cout<<"Parallel Answer: "<<"\n";
    for(int i=0; i<N; i++) {
        cout<<x[i]<<" ";
    }
    cout<<endl;
}

int main() {
    int N;
    cout<<"Enter N";
    cin>>N;
    vector<vector<double>> A(N, vector<double>(N, 0.0));
    vector<double> B(N, 0);
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

    vector<vector<double>> A1=A;
    vector<vector<double>> A2=A;

    vector<double> B1=B;
    vector<double> B2 = B;


    serialGaussian(A1, B1, N);
    parallelGaussian(A, B, N);

    return 0;
    
}
