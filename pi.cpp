#include <iostream>
#include <cstdlib>
#include <omp.h>
#include <cmath>
#include <iomanip>

using namespace std;

double f(double x) {
    return 4.0/(1.0 + x*x);
}

int main() {
    cout<<fixed<<setprecision(8);

    double pi = acos(-1.0);

    int n;
    cout<<"Enter the number of rectangles: ";
    cin>>n;

    double a = 0.0, b = 1.0;

    double width = (b - a)/n;
    cout<<"Width of one rectangle: "<<width<<endl;

    double ss = omp_get_wtime();
    double sum = 0.0;
    double pi_s;
    for(int i = 0; i < n; i++) {
        double x = a + (i + 0.5) * width;
        double height = f(x);
        cout<<"\nHeight of "<<i<<"th rectangle is: "<<height<<endl;
        sum += height;
    }
    pi_s = sum * width;
    double es = omp_get_wtime();
    cout<<"Serial pi: "<<pi_s<<endl;
    cout<<"Actual pi: "<<pi<<endl;
    cout<<"Time taken: "<<(es-ss);

    double sp = omp_get_wtime();
    sum = 0.0;
    double pi_p;
    #pragma omp parallel for
    for(int i = 0; i < n; i++) {
        double x = a + (i + 0.5) * width;
        double height = f(x);
        cout<<"\nHeight of "<<i<<"th rectangle is: "<<height<<endl;
        sum += height;
    }
    pi_p = sum * width;
    double ep = omp_get_wtime();
    cout<<"Serial pi: "<<pi_p<<endl;
    cout<<"Actual pi: "<<pi<<endl;
    cout<<"Time taken: "<<(ep-sp);

    double sc = omp_get_wtime();
    sum = 0.0;
    double pi_c;
    #pragma omp parallel for
    for(int i = 0; i < n; i++) {
        double x = a + (i + 0.5) * width;
        double height = f(x);
        cout<<"\nHeight of "<<i<<"th rectangle is: "<<height<<endl;
        #pragma omp critical
        {
            sum += height;
        }
    }
    pi_c = sum * width;
    double ec = omp_get_wtime();
    cout<<"Serial pi: "<<pi_c<<endl;
    cout<<"Actual pi: "<<pi<<endl;
    cout<<"Time taken: "<<(ec-sc);

    double sa = omp_get_wtime();
    sum = 0.0;
    double pi_a;
    #pragma omp parallel for
    for(int i = 0; i < n; i++) {
        double x = a + (i + 0.5) * width;
        double height = f(x);
        cout<<"\nHeight of "<<i<<"th rectangle is: "<<height<<endl;
        #pragma omp atomic
            sum += height;
    }
    pi_a = sum * width;
    double ea = omp_get_wtime();
    cout<<"Serial pi: "<<pi_a<<endl;
    cout<<"Actual pi: "<<pi<<endl;
    cout<<"Time taken: "<<(ea-sa);

    double sr = omp_get_wtime();
    sum = 0.0;
    double pi_r;
    #pragma omp parallel for reduction(+:sum)
    for(int i = 0; i < n; i++) {
        double x = a + (i + 0.5) * width;
        double height = f(x);
        cout<<"\nHeight of "<<i<<"th rectangle is: "<<height<<endl;
        sum += height;
        
    }
    pi_r = sum * width;
    double er = omp_get_wtime();
    cout<<"Serial pi: "<<pi_r<<endl;
    cout<<"Actual pi: "<<pi<<endl;
    cout<<"Time taken: "<<(er-sr);

    return 0;
}