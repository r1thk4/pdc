#include <iostream>
#include <omp.h>
#include <cstdlib>
#include <vector>

using namespace std;

void printArray(vector<int> &A) {
    int n = A.size();
    for(int i=0; i<min(n, 20); i++) {
        cout<<A[i]<<" ";
    }
    cout<<endl;
}

void bubbleSort(vector<int> &A) {
    vector<int> B = A;
    int n = B.size();
    for(int i = 0; i<n-1; i++) {
        for(int j=0; j<n-i-1; j++) {
            if(B[j]>B[j+1]) {
                swap(B[j], B[j+1]);
            }
        }
    }
    printArray(B);
}

void OddEven(vector<int> &A) {
    vector<int> B = A;
    int n = B.size();
    bool isSwapped;
    int i=0;
    while(i < n) {
        isSwapped = false;
        for(int j=1; j<n-1; j++) {
            if(B[j]>B[j+1]) {
                swap(B[j], B[j+1]);
                isSwapped = true;
            }
        }
        for(int j=0; j<n-1; j++) {
            if(B[j]>B[j+1]) {
                swap(B[j], B[j+1]);
                isSwapped = true;
            }
        }
        if(!isSwapped) break;
    }
    printArray(B);
    return;
}

void OddEvenP(vector<int> &A) {
    vector<int> B = A;
    int n = B.size();
    bool isSwapped;
    int i=0;
    while(i < n) {
        isSwapped = false;
        #pragma omp parallel 
        {
            bool localSwapped = false;
            #pragma omp for
            for(int j=1; j<n-1; j++) {
                if(B[j]>B[j+1]) {
                    swap(B[j], B[j+1]);
                    localSwapped = true;
                }
            }
            #pragma omp barrier

            #pragma omp for
            for(int j=0; j<n-1; j++) {
                if(B[j]>B[j+1]) {
                    swap(B[j], B[j+1]);
                    localSwapped = true;
                }
            }

            #pragma omp critical 
            {
                if(localSwapped) isSwapped = true;
            }
        }
        if(!isSwapped) break;        
    }
    printArray(B);
    return;
}

int main() {
    cout<<"Enter the number of elements: ";
    int n;
    cin>>n;
    vector<int> A(n, 0);
    cout<<"Enter the elements: "<<endl;
    if(n<=10) {
        for(int i=0; i<n; i++) {
            cin>>A[i];
        }
    } else {
        for(int i=0; i<n; i++) {
            A[i] = (int) rand() % (n*100) + 1;
        }
    }
    printArray(A);

    double sb = omp_get_wtime();
    bubbleSort(A);
    double eb = omp_get_wtime();
    cout<<"\nTime taken for Bubble Sort: "<<eb-sb<<endl;

    double so = omp_get_wtime();
    OddEven(A);
    double eo = omp_get_wtime();
    cout<<"\nTime taken for Odd-Even Sort: "<<eo-so<<endl;

    double sp = omp_get_wtime();
    OddEvenP(A);
    double ep = omp_get_wtime();
    cout<<"\nTime taken for Odd-Even Sort Parallel: "<<ep-sp<<endl;

    return 0;
}