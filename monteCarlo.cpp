#include <iostream>
#include <cmath>
#include <iomanip>
#include <omp.h>
#include <ctime>
#include <cstdlib>

using namespace std;

int main() {
    cout<<fixed<<setprecision(6);

    double pi = acos(-1.0);
    srand(time(0));
    long long n;
    cout<<"Enter number of points: ";
    cin >> n;

    int circlePoints = 0;
    double ss = omp_get_wtime();
    for(long long i=0; i<n; i++) {
        double x = (double) rand() / RAND_MAX*2.0 - 1.0;
        double y = (double) rand() / RAND_MAX*2.0 - 1.0;
        if( (x*x) + (y*y) <= 1) {
            circlePoints++;
        }
    }
    double pi_s = 4.0*circlePoints/n;
    double es = omp_get_wtime();
    cout<<"Serial pi: "<<pi_s<<endl;
    cout<<"Actual pi: "<<pi<<endl;
    cout<<"Time taken: "<<es-ss<<endl;

    circlePoints = 0;
    double sp = omp_get_wtime();
    #pragma omp parallel
    {
        unsigned int seed = time(0) ^ omp_get_thread_num();
        #pragma omp for reduction(+:circlePoints)
        for(long long i=0; i<n; i++) {
            double x = (double) rand_r(&seed) / RAND_MAX*2.0 - 1.0;
            double y = (double) rand_r(&seed) / RAND_MAX*2.0 - 1.0;
            if( (x*x) + (y*y) <= 1) {
                circlePoints+=1;
            }
        }
    }
    double pi_p = 4.0*circlePoints/n;
    double ep = omp_get_wtime();
    cout<<"Parallel pi: "<<pi_p<<endl;
    cout<<"Actual pi: "<<pi<<endl;
    cout<<"Time taken: "<<ep-sp<<endl;

    return 0;
}