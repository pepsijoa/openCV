#include<iostream>
#include<thread>

using std::thread;

void func1()
{
	for (int i = 0; i < 10; i++) {
		std::cout << "thread 1 working! \n";
	}
}


void func2()
{
	for (int i = 0; i < 10; i++) {
		std::cout << "thread 2 working! \n";
	}
}

void func3()
{
	for (int i = 0; i < 10; i++) {
		std::cout << "thread 3 working! \n";
	}
}

int main()
{
	thread t1(func1);
	thread t2(func2);
	thread t3(func3);

	/*t1.join();
	t2.join();
	t3.join();*/
}
