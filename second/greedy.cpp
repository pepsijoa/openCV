#include<bits/stdc++.h>

using namespace std;

int minusarr[100000];
int plusarr[100000];


int main(void)
{
	int num;
	int minusindex = 0;
	int plusindex = 0;
	int zero_is = 0;
	cin >> num;
	for (int i = 0; i < num; i++)
	{
		int a;
		cin >> a;
		if (a < 0) {
			minusarr[minusindex] = a;
			minusindex++;
		}
		else if( a > 0){
			plusarr[plusindex] = a;
			plusindex++;
		}
		else {
			zero_is = 1;
		}
		sort(minusarr, minusarr + minusindex);
		sort(plusarr, plusarr+ plusindex, greater<int>());
	}

	int sum = 0;
	for (int i = 0; i < minusindex; i++) {
		if (i == minusindex - 1) {
			sum += minusarr[i];
		}
		else {
			
		}
	}
	return 0;

}