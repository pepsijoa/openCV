#include<bits/stdc++.h>

using namespace std;

int vis[1001][1001]{ 0 };
int board[1001][1001]{ 0 };

int dx[4]{ 1, 0, -1, 0 };
int dy[4]{ 0, -1, 0, 1 };

int main(void)
{
	ios::sync_with_stdio(0);
	cin.tie(0);

	int n = 0;
	int m = 0;

	cin >> m >> n;

	queue<pair<int, int>> Q;
	for (int row = 0; row < n; row++) {
		for (int col = 0; col < m; col++) {
			cin >> board[row][col];
			if (board[row][col] == 1) {
				vis[row][col] = 1;
				Q.push({ row, col });
			}
		}
	}

	while (!Q.empty())
	{
		pair<int, int> cur = Q.front(); Q.pop();
		for (int dir = 0; dir < 4; dir++) {
			int nx = cur.first + dx[dir];
			int ny = cur.second + dy[dir];
			if (nx < 0 || nx > n - 1 || ny < 0 || ny > m - 1) continue;
			if (vis[nx][ny] || board[nx][ny] != 0) continue;
			vis[nx][ny] = vis[cur.first][cur.second] + 1;
			Q.push({ nx,ny });
		}
	}

	int ans = 1;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			if (vis[i][j] == 0 && board[i][j] != -1) {
				ans = -1;
				break;
			}

			if (ans < vis[i][j]) {
				ans = vis[i][j];
			}
		}
		if (ans == -1) break;
	}
	if (ans >= 1) cout << ans - 1;
	else cout << -1;







}