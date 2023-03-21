/*
This program first generate a vector V1 of n elements (as double) that are randomly drawn
from a negative binomial distribution (initialized with the probability of a trial gen-
erating true p and a number of trial successes k and result type int). The random
numbers should be generated with the Mersenne-Twister algorithm by Matsumoto
and Nishimura (2000) using 64-bit integers, with default parameters and a fixed seed s.
Next, generate a vector V2 of m elements (as double) and modify them in the same way as before, but
with a different seed s2 = s · 2.

Then, given these two vectors V1 and V2, perform a two-sample t-test on the scaled and non-scaled
values and output the t-statistic

*/

#include<iostream>
#include <vector>
#include <functional>
#include <random>
#include <cmath>
#include <numeric>
#include <iomanip>
#include <string>
#include <iterator>
using namespace std;

//function to convert string to unsigned int
inline unsigned int stoui(const std::string& s)
{
    unsigned long lresult = stoul(s);
    unsigned int result = lresult;
    if (result != lresult) { cout<<"Invalid error";}
    return result;
}
//function to calculate the mean
double mean(vector<double> v) {
    double sum = 0;
    for (auto i = 0; i < v.size(); i++) {
        sum = sum + v[i];
    }
    double mean = sum / v.size();
    return mean;
}

//function to calculate the standard deviation
double standard_deviation(vector<double> v) {
    double variance, var;
    double standardDeviation;
    for (auto i = 0; i < v.size(); i++) {
        variance+= pow( (v[i] - mean(v)), 2);
        var = variance / (v.size() - 1);
        standardDeviation = sqrt(var);
    }
    return standardDeviation;

}

//function to scale the values with log2
vector<double> scale(vector<double> v) {
    sort(v.begin(), v.end(), greater<>());
    vector<double> s;
    for (double i=0;i<v.size();i++) {
        s.push_back(log2(abs(v[i]) + 1));
    }
    return s;
}
//unsorted_scale
vector<double> unsorted_scale(vector<double> v) {
    vector<double> s;
    for (double i=0;i<v.size();i++) {
        s.push_back(log2(abs(v[i]) + 1));
    }
    return s;
}
//function to calculate the T-test
double Ttest(vector<double> v, vector<double> vector1) {
    double test = 0;
    double x1, x2;
    double s1, s2, sp;
    double degree;
    x1 = mean(v);
    x2 = mean(vector1);
    s1 = standard_deviation(v);
    s2 = standard_deviation(vector1);
    double n1 = v.size();
    double n2 = vector1.size();
    degree = n1 + n2 - 2;

    sp = sqrt((((n1 - 1) * pow(s1, 2)) + (n2 - 1) * (pow(s2, 2))) / degree);
    test = (x1 - x2) / (sp * sqrt(1 / n1 + 1 / n2));

    return test;

}

// main function
int main(int argc, char *argv[]) {

    if(argc <6){ //checking for correct program arguments
        cout<<"Missing arguments"<<endl;
        cout<<argv[0]<<"5"<<endl;
        exit (EXIT_FAILURE);
    }
   unsigned int n, m, s, k;
    double p;
    n = stoui(argv[1]);
    m = stoui(argv[2]);
    s = stoui(argv[3]);
    p = stod(argv[4]);
    k = stoui(argv[5]);
//vector V1
    vector<double> V1;
    mt19937_64 generator(s); ////random number generator using negative bionomial distribution
    negative_binomial_distribution<int> distribution(k, p);
    for (auto i = 0; i < n; i++) {
        V1.push_back(distribution(generator));
    }

//vector V2
    vector<double> V2;
    mt19937_64 twister(s * 2); //random number generator using negative bionomial distribution
    negative_binomial_distribution<int> binomial(k, p);
    for (auto i = 0; i < m; i++) {
        V2.push_back(binomial(twister));

    }
    cout<<setprecision(3)<<"V1 Mean: "<<mean(V1)<<endl; //prinitng mean for V1
    cout<<setprecision(3)<<"V1 Sample standard deviation: "<<standard_deviation(V1)<<endl;
    vector<double> scaled_V1 = scale(V1);
    if(n>=10){ //printing the top elements for v1
        cout<<"V1 Top 10 elements: ";
       unsigned int cnt = 0;
        for(auto iter=scaled_V1.begin(); iter != scaled_V1.end() && cnt < n ; ++iter,++cnt){
            if (iter != scaled_V1.begin()) cout << ", ";
            cout << setprecision(3)<<*iter;
        }
    }
    else {
        cout << setprecision(3) << "V1 Top " << n << " elements: ";
        for (auto iter = scaled_V1.begin(); iter != scaled_V1.end(); iter++) {
            if (iter != scaled_V1.begin()) cout << ", ";
            cout << setprecision(3)<<*iter;
        }
    }
    cout<<endl;
    cout<<setprecision(3)<<"V2 Mean: "<<mean(V2)<<endl;
    cout<<setprecision(3)<<"V2 Sample standard deviation: "<<standard_deviation(V2)<<endl;
    vector<double> scaled_V2 = scale(V2);
    if(m>=10){
        cout<<"V2 Top 10 elements: ";
        unsigned int cnt = 0;
        for(auto iter=scaled_V2.begin(); iter != scaled_V2.end() && cnt < m ; ++iter,++cnt){
            if (iter != scaled_V2.begin()) cout << ", ";
            cout << setprecision(3)<<*iter;
        }
    }
    else {
        cout << setprecision(3) << "V2 Top " << m<< " elements: ";
        for (auto iter = scaled_V2.begin(); iter != scaled_V2.end(); iter++) {
            if (iter != scaled_V2.begin()) cout << ", ";
            cout << setprecision(3)<<*iter;
        }
    }

    cout<<"\n";
    cout<<setprecision(3)<<"Comparing V1 and V2..."<<endl;
    cout<<setprecision(3)<<"Unscaled t-statistic: "<<Ttest(V1,V2)<<endl;
    cout<<setprecision(3)<<"Comparing log2 scaled V1 and V2..."<<endl;
    cout<<setprecision(3)<<"Scaled t-statistic: "<<Ttest(scale(V1),scale(V2))<<endl;
    cout<<setprecision(3)<<"Total degrees of freedom: "<< n + m- 2<<endl;

    vector<double> V3(n);//Vector for srandom shuffled value

    iota(V3.begin(), V3.end(), 1);
    shuffle(V3.begin(), V3.end(), mt19937_64 (s * 7));
    vector<double> unsorted_V1= unsorted_scale(V1);
    vector<double> result(n);
    vector<double> even;
    vector<double> odd;
    transform(V3.begin(), V3.end(), unsorted_V1.begin(), result.begin(), multiplies<>()); //multiplying the shuffled vector with V1 and storing it result vector
    //storing even values in even vector and odd vales in odd vector
    for (auto i:result) {
        if ((int) rint(i) % 2 == 0) {
            even.push_back(i);

        } else {
            odd.push_back(i);

        }
    }

    sort(even.begin(), even.end(), greater<>()); //sorting even vector in descending  order
    sort(odd.begin(), odd.end()); //sorting odd vector in ascending order

    even.insert(even.end(), odd.begin(), odd.end()); //concatenating even and odd vector. Storing odd vector in even
    cout<<"Sorted vector: ";
    for (auto iter = even.begin(); iter != even.end(); iter++) {
        if (iter != even.begin()) cout << ", ";
        cout << setprecision(3)<<*iter;
    }
    cout<<endl;

    return 0;


}