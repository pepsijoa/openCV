#include<bits/stdc++.h>

using namespace std;

string bomb;
char sen[1000001];

vector<char> func(vector<char> sen);

int main(void)
{
	vector<char> ans;
	cin >> sen;
	char a = sen[0];
	for (int i = 1; a != '\0';i++)
	{
		ans.push_back(a);
		a = sen[i];
	}

	cin >> bomb;

	vector<char>res2 = func(ans);
	
	for (int i = 0; i < res2.size(); i++) {
		cout << res2[i];
	}
	cout << '\n' << res2.size();

}

vector<char> func(vector<char> sen)
{
	vector<char> res;
	
	for (int i = 0; i < sen.size(); i++)
	{
		int correct = 0;
		if (sen[i] == bomb[0]) {
			for (int k = 0; k < bomb.size(); k++) {
				if (i + k >= sen.size()) break;
				if (sen[i+k] == bomb[k]) {
					//cout << "s = " << sen[k] << ' ' << endl;
					correct++;
				}
			}
		}
		
		if (i + correct < sen.size()) {
			res.push_back(sen[i + correct]);
		}
		
	}
}