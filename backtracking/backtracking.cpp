#include<iostream>
#include<algorithm>
using namespace std;


char code[20]{};
bool use[20]{};

char ans[20]{};

int l, c;

void func(int a, char b)
{
	if (a == l)
	{
		int acount = 0;
		int bcount = 0;
		for (int i = 0; i < l; i++) {
			if (ans[i] == 'a' or ans[i] == 'e' or ans[i] == 'i' or ans[i] == 'o' or ans[i] == 'u') acount++;
			else bcount++;
		}
			
		
		if (acount and bcount >= 2) {
			for (int i = 0; i < l; i++) {
				cout << ans[i];
			}
			cout << endl;
		}
		
		return;
	} 
	else {
		for (int i = 0; i < c; i++) {
			if (!use[i] and code[i] > b) {
				ans[a] = code[i];
				use[i] = true;
				func(a + 1, code[i]);
				use[i] = false;
			}
		}
	}
}

int main(void)
{
	cin >> l >> c;
	for (int i = 0; i < c ;i++)
		cin >> code[i];
	sort(code, code + c);
	func(0, 0);
	return 0;
}