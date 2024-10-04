#include<bits/stdc++.h>

using namespace std;

int vis[102][102][102]{ 0 };
int board[102][102][102]{ 0 };

int dx[6]{ 1, 0, -1, 0 , 0, 0};
int dy[6]{ 0, -1, 0, 1 , 0, 0};
int dz[6]{ 0,  0, 0, 0 , 1, -1};
int main(void)
{
	ios::sync_with_stdio(0);
	cin.tie(0);

	int n = 0;
	int m = 0;
	int h = 0;

	cin >> m >> n >> h;

	queue<pair<pair<int, int>, int>> Q;
	for (int height = 0; height < h; height++) {
		for (int row = 0; row < n; row++) {
			for (int col = 0; col < m; col++) {
				cin >> board[height][row][col];
				if (board[height][row][col] == 1) {
					vis[height][row][col] = 1;
					Q.push({ {height, row}, col });
				}
				
			}
		}
	}
	

	while (!Q.empty())
	{
		pair<pair<int, int> , int>cur = Q.front(); Q.pop();
		for (int dir = 0; dir < 6; dir++) {
			int nx = cur.first.second+ dx[dir];
			int ny = cur.second + dy[dir];
			int nz = cur.first.first + dz[dir];

			if (nx < 0 || nx > n - 1 || ny < 0 || ny > m - 1 || nz < 0 || nz > h-1) continue;
			if (vis[nz][nx][ny] || board[nz][nx][ny] != 0) continue;
			vis[nz][nx][ny] = vis[cur.first.first][cur.first.second][cur.second] + 1;
			/*cout << cur.first.first << ' ' << cur.first.second << ' ' << cur.second <<endl;*/
			Q.push({ { nz,nx } , ny});
		}
	}
	for (int height = 0; height < h; height++) {
		for (int row = 0; row < n; row++) {
			for (int col = 0; col < m; col++) {
				cout << vis[height][row][col] << ' ';

			}
			cout << endl;
		}
	}

	int ans = 1;
	for (int height = 0; height< h; height++) {
		for (int row = 0; row < n; row++) {		
			for (int col = 0; col < m; col++) {
				if (vis[height][row][col] == 0 && board[height][row][col] != -1) {
					ans = -1;
					break;
				}
				if (ans < vis[height][row][col]) {
					ans = vis[height][row][col];
				}
			}
			if (ans == -1) break;
		}
		if (ans == -1) break;
	}
	if (ans >= 1) cout << ans - 1;
	else cout << -1;


}