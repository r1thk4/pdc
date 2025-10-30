#include <iostream>
#include <vector>
#include <omp.h>
#include <cstdlib>
#include <ctime>

using namespace std;

void merge(vector<int> &A, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;
    int *L = new int[n1], *R = new int[n2];
    for(int i=0; i<n1; i++) {
        L[i] = A[l+i];
    }
    for(int j=0; j<n2; j++) {
        R[j] = A[m+1+j];
    }
    int i=0, j=0, k=l;
    while(i<n1 && j<n2) {
        if(L[i]<R[j]) {
            A[k++] = L[i++];
        } else {
            A[k++] = R[j++];
        }
    }
    while(i<n1) {
        A[k++] = L[i++];
    }
    while(j<n2) {
        A[k++] = R[j++];
    }

    delete[] L;
    delete[] R;
}

void mergeSortS(vector<int> & A, int l, int r) {
    if(l < r) {
        int m = l + (r - l)/2;
        mergeSortS(A, l, m);
        mergeSortS(A, m+1, r);
        merge(A, l, m, r);
    }    
}

void mergeSortP(vector<int> & A, int l, int r) {
        if(l < r) {
            int m = l + (r - l)/2;
            #pragma omp parallel sections 
            {
                #pragma omp section
                {
                    mergeSortP(A, l, m);
                }
                #pragma omp section
                {
                    mergeSortP(A, m+1, r);
                }        
            }    
            merge(A, l, m, r);
        }
}

int main() {
    srand(time(0));
    int n;
    cout<<"Enter the number of elements: "<<endl;
    cin>>n;
    vector<int> A(n);
    if(n<=10) {
        cout<<"Enter the elements";
        for(int i=0; i<n; i++) {
            cin>>A[i];
        }
    } else {
        for(int i=0; i<n; i++) {
            A[i] = (int) rand() % (n*100) + 1;
        }
    }
    vector<int> A1 = A;
    vector<int> A2 = A;
    double ss = omp_get_wtime();
    mergeSortS(A1, 0, n -1);
    double es = omp_get_wtime();
    cout<<"\nSerial\n";

    for(int i=0; i<min(n,20); i++) {
        cout<< A1[i] << " "; 
    }

    cout<<"\n Time: "<<es-ss<<endl;
    
    double sp = omp_get_wtime();
    mergeSortP(A2, 0, n -1);
    double ep = omp_get_wtime();

    cout<<"\n Parallel\n";
    for(int i=0; i<min(n,20); i++) {
        cout<< A2[i] << " "; 
    }

    cout<<"\n Time: "<<ep - sp<<endl;
    return 0;
}