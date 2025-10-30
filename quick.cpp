#include <iostream>
#include <cstdlib>
#include <vector>
#include <omp.h>
#include <ctime>

using namespace std;

int partition_f(vector<int> &A, int low, int high) {
    int i = high + 1;
    int pivot = A[low];
    int j = high;
    while(j>low) {
        if(A[j]>pivot) {
            i--;
            swap(A[j], A[i]);
        }
        j--;
    }
    swap(A[i-1], A[low]);
    return (i-1);
}

int partition_l(vector<int> &A, int low, int high) {
    int i = low -1;
    int pivot = A[high];
    int j = low;
    while(j<high) {
        if(A[j]<pivot) {
            i++;
            swap(A[i], A[j]);
        }
        j++;
    }
    swap(A[i+1], A[high]);
    return i+1;
}

int partition_m(vector<int> &A, int low, int high) {
    int m = low + (high - low)/2;
    swap(A[m], A[high]);
    return partition_l(A, low, high);
}

void quickSortS(vector<int> &A, int pc, int l, int r) {
    int pi;
    if(l < r) {
        
        if(pc==1) {
            pi = partition_f(A, l, r);
        } else if(pc==2) {
            pi = partition_m(A, l, r);
        } else {
            pi = partition_l(A, l, r);
        }
        quickSortS(A, pc, l, pi-1);
        quickSortS(A, pc, pi+1, r);  
    }  
}

void quickSortP(vector<int> &A, int pc, int l, int r) {
    int pi;
    if(l < r) {
        
        if(pc==1) {
            pi = partition_f(A, l, r);
        } else if(pc==2) {
            pi = partition_m(A, l, r);
        } else {
            pi = partition_l(A, l, r);
        }
         #pragma omp parallel sections
            {
                #pragma omp section
                {
                    quickSortP(A, pc, l, pi-1);
                }
                #pragma omp section
                {
                    quickSortP(A, pc, pi+1, r); 
                }
            }   
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
    cout<<"Enter pivot choice, first -1, middle -2, last -3;";
    int pivot_choice;
    cin>>pivot_choice;

    vector<int> A1 = A;
    vector<int> A2 = A;
    double ss = omp_get_wtime();
    quickSortS(A1, pivot_choice, 0, n -1);
    double es = omp_get_wtime();
    cout<<"\nSerial\n";

    for(int i=0; i<min(n,20); i++) {
        cout<< A1[i] << " "; 
    }

    cout<<"\n Time: "<<es-ss<<endl;
    
    double sp = omp_get_wtime();
    quickSortP(A2, pivot_choice, 0, n -1);
    double ep = omp_get_wtime();

    cout<<"\n Parallel\n";
    for(int i=0; i<min(n,20); i++) {
        cout<< A2[i] << " "; 
    }

    cout<<"\n Time: "<<ep - sp<<endl;
    return 0;
}