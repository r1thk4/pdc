#include <iostream>
#include <vector>
#include <omp.h>

using namespace std;

void printMatrix(vector<vector<int>> &C, int r, int c) {
    for(int i=0; i<r; i++) {
        for(int j=0; j<c; j++) {
            cout<<C[i][j]<<" ";
        }
        cout<<endl;
    }
}

void multiplySerial(vector<vector<int>> &A, vector<vector<int>> &B, int r1, int r2, int c1, int c2) {
    vector<vector<int>> C(r1, vector<int>(c2, 0));
    double start = omp_get_wtime();
    for(int i=0; i<r1; i++) {
        for(int j=0; j<c2; j++) {
            for(int k=0; k<c1; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    cout<<"\n\n Serial: \n";
    double end = omp_get_wtime();
    printMatrix(C, r1, c2);
    cout<<"\nSerial Time taken: "<<(end - start)<<endl;
    return;
}

void multiplyParallel(vector<vector<int>> &A, vector<vector<int>> &B, int r1, int r2, int c1, int c2) {
    vector<vector<int>> C(r1, vector<int>(c2,0)); 
    double start = omp_get_wtime();
    #pragma omp parallel for collapse(2)
    for(int i=0; i<r1; i++) {
        for( int j=0; j<c2; j++) {
            for(int k=0; k<c1; k++) {
                C[i][j] += A[i][k]*B[k][j];
            }
        }
    }
    cout<<"\n\n Parallel: \n";
    double end = omp_get_wtime();
    printMatrix(C, r1, c2);
    cout<<"\nParallel Time taken: "<<(end - start)<< endl;
    return;
}


int main() { 
    int r1, r2, c1, c2;
    cout<<"Enter the rows and cols for matrx 1 and 2 respectively:"<<endl;
    cin>>r1>>c1>>r2>>c2;
    if(c1!=r2) {
        cout<<"Matrix multiplication not possible";
        return 0;
    }
    vector<vector<int>> A(r1, vector<int>(c1));
    vector<vector<int>> B(r2, vector<int>(c2));
    cout<<"Enter matrix 1"<<endl;
    for(int i=0; i<r1; i++) {
        for(int j=0; j<c1; j++) {
            cin>>A[i][j];
        }
    }
    cout<<"Enter matrix 2:"<<endl;
    for(int i=0; i<r2; i++) {
        for(int j=0; j<c2; j++) {
            cin>>B[i][j];
        }
    }
    multiplySerial(A, B, r1, r2, c1, c2);
    multiplyParallel(A, B, r1, r2, c1, c2);
    return 0;
}


