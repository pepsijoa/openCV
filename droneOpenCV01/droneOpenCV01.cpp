#include "opencv2/opencv.hpp"
#include <iostream>
using namespace cv;
using namespace std;
int main(void) {
	cout << "Hello OpenCV " << CV_VERSION << endl;
	Mat img;
	img = imread("C:/Users/woo12/OneDrive/Desktop/cpp/img/Lenna.bmp");
	if (img.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}
	namedWindow("image");
	imshow("image", img);
	waitKey(0);
	return 0;

}	