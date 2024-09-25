#include<opencv2/opencv.hpp>


using namespace std;
using namespace cv;

string path = "C:/Users/woo12/OneDrive/Desktop/cpp/img/";

int main(void)
{
	Mat img = imread(path + "Lenna.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	cv::imshow("ori", img);
	cv::waitKey(0);
}