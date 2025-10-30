#include <iostream>
#include <omp.h>
#include <vector>
#include <cstdio>
#include <cmath>
#include <vector>
#include <algorithm>
using namespace std;

vector<double> gaussElim(vector<vector<double>> matrix , vector<double> b, int N){
vector<double> x(N);

for(int k=0;k<N-1;k++){
double max_val =0;
int max_row = k;

for(int i = k+1;i<N;i++){
if(abs(matrix[i][k]) > max_val){
max_val = abs(matrix[i][k]);
max_row = i;
}
}
if(max_val < 1e-10){
printf("Matrix singular");
exit(1);
}
if(max_row!=k){
swap(matrix[k], matrix[max_row]);
swap(b[max_row],b[k]);
}
double factor = 0.0;
#pragma omp parallel for schedule(static) private(factor)
for(int i = k+1;i<N;i++){
factor = matrix[i][k]/matrix[k][k];

for(int j=k;j<N;j++){
matrix[i][j] = matrix[i][j] - factor*matrix[k][j];
}
b[i] = b[i]-factor*b[k];

}
}
for(int i=N-1;i>=0;i--){
double sum =0.0;
for(int j=i+1;j<N;j++){
sum += matrix[i][j] * x[j];
}
x[i] = (b[i] - sum)/matrix[i][i];
}

return x;

}



int main(){
vector<vector<double>> matrix = {
{2,1,-1},
{-3,-1,2},
{-2,1,2}
};
vector<double> b = {8,-11,-3};
vector<double> x(3);
x = gaussElim(matrix,b,3);
for(int i = 0;i<3;i++){
cout << x[i]<< "\n";
}
return 0;
}
