#include<iostream>
#include<bitset>
#include<cmath>

using namespace std;

#define N 20000

bitset<N> isPrime;

void sieve()
{

	isPrime.set();
	for(int i = 2; i < sqrt(N); ++i){
		if(isPrime[i]){
			cout << i << endl;
			for(int j = i*i; j < N; j+=i){
				isPrime[j] = 0;
			}
		}
	}
}

int main()
{
	sieve();

	return 0;
}

