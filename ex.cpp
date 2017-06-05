#include "BigInteger.h"
#include<iostream>
#include<vector>

using namespace std;

int main()
{
    vector<BigInteger> Bints;
    Bints.push_back(1);
    for(int i = 1; i <= 1000; ++i){
        Bints.push_back(Bints[i-1]*i);
    }

    int n;
    while(cin >> n){
        cout << n << '!'<<endl;
        cout << Bints[n]<<endl;
    }
}