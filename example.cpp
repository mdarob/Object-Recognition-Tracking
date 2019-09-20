#include<iostream>
int main()
{
	const int nSteps = 10;   // thats how many steps
	const int nReal = 2;
	const int N = nSteps * nReal;
	double H[N] = {0.0};
	for (int i = 0; i< N; i++)
	{ std::cout << H[i] << std::endl;  }
}
