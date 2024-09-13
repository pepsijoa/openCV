#include <iostream>

using namespace std;

class Simple
{
public:
	Simple() { cout << "Simple constructor called!" << endl; }
	~Simple() { cout << "Simple deconstructor called!" << endl; }
};

void main()
{
	multiArray();
}

void multiArray()
{
	char** allocate(size_t xDimension, size_t yDimension)
	{
		char** myArray {new char*[xDimension] }
	}
}
//객체 할당에 대한 내용, pg 330
void classAllo()
{
	Simple* singleptr{ new Simple[3] };
	// use
	delete[]singleptr;
	singleptr = nullptr;

	const size_t size{ 4 };
	Simple** mySimplePtrArray{ new Simple * [size] };
	for (size_t i{ 0 }; i < size; i++) { mySimplePtrArray[i] = new Simple[2]; }

	//use

	for (size_t i{ 0 }; i < size; i++) {
		delete[] mySimplePtrArray[i];
		mySimplePtrArray[i] = nullptr;
	}

	delete[] mySimplePtrArray;
	mySimplePtrArray = nullptr;
}