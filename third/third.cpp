#include<bits/stdc++.h>

using namespace std;

int board[1001][1001];
int ans[1001][1001];

int main(void)
{
	int n, m;
	string num;
	cin >> n >> m; 
	for (int i = 0; i < n; i++) {
		cin >> num;
		for (int j = 0; j < m; j++) {
			board[i][j] = num[j] - '0';
		}
	}
	
	int answer = 0;
	for (int i = 1; i < n; i++) {
		for (int j = 1; j < m; j++) {
			if (board[i][j] == 1) {
				ans[i][j] = min(ans[i - 1][j - 1], min(ans[i][j - 1], ans[i - 1][j])) + 1;
				if (answer < ans[i][j]) {
					answer = ans[i][j];
				}
			}
			else {
				ans[i][j] = 0;
			}
		}
	}
	/*
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			cout << ans[i][j] << ' ';
		}
		cout << endl;
	}*/
	cout << answer * answer;
	return 0;
}
