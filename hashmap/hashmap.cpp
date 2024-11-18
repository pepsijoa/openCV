#include<bits/stdc++.h>
#include<unordered_set>
using namespace std;

unordered_set<long long>s;
vector<int>sum;

long long arr[1001];
int main(void)
{
	int n;
	cin >> n;
	for (int i = 0; i < n; i++) {
		long long num=0;
		cin >> num;
		arr[i] = num;
	}
	sort(arr, arr + n, greater<>());
	
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			sum.push_back(arr[i] + arr[j]);
		}
	}
	for (int i = 0; i < n; i++) {
		s.insert(arr[i]);
	}


	long long max = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < sum.size(); j++) {
			long long find = arr[i] + sum[j];
			if (s.find(find) != s.end()) {
				if (max < find) max = find;
			}
		}
	}
	cout << max;
}