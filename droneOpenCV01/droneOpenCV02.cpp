#include<iostream>
#include<opencv2/core.hpp>

using namespace std;
using namespace cv;

int main(void)
{
	Mat mat1 = Mat::zeros(3, 4, CV_8UC1);
	for (int j = 0; j < mat1.rows; j++) {
		for (int i = 0; i < mat1.cols; i++) {
			mat1.at<uchar>(j, i)++;
		}
	}
	//cout << mat1 << endl;

	

	Mat mat3 = Mat::zeros(1, 4, CV_8UC1);
	mat1.push_back(mat3);
	cout << mat1;
}