#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <vector>
#include <initializer_list>
#include <cmath>
#include<string>

using namespace cv;
using namespace std;

void grayScale();
void knn();

void colorotate(Mat input, float degree, float scaleXSize, float scaleYSize);
void rotate(Mat input, float degree, float scaleXSize, float scaleYSize);

vector<int> maxCal(Mat input, float radian, float scaleXSize, float scaleYSize);
pair<float, float> rerot(pair<int, int>point, float degree);
pair<float, float> rot(pair<int, int>point, float degree);

int main(void)
{
	String path = "C:/Users/woo12/OneDrive/Desktop/cpp/img/";
	String img = "konkuk.bmp";
	Mat imgColor = imread(path + img, CV_LOAD_IMAGE_COLOR);
	Mat imgGrey = imread(path + img, CV_LOAD_IMAGE_GRAYSCALE);

	for (int i = 0; i < 360; i++) {
		colorotate(imgColor, i, 0.5, 1);
	}
	//colorotate(imgColor, 18, 1, 1);
	
	return 0;
	
}

pair<float,float> rerot(pair<int, int>point, float degree) //intput point (y, x) & output point ( y, x) 
{
	pair<int, int>newPoint;
	newPoint.first =  point.second * (-1) * sin(degree) + point.first * cos(degree);
	newPoint.second = point.second * cos(degree) + point.first * sin(degree);

	return newPoint;
}

pair<float, float> rot(pair<int, int>point, float degree) //intput point (y, x) & output point ( y, x) 
{
	pair<float, float>newPoint;
	newPoint.first = point.second * sin(degree) + point.first * cos(degree);
	newPoint.second = point.second * cos(degree) - point.first * sin(degree);
	return newPoint;
}

vector<int> maxCal(Mat input, float radian, float scaleXSize, float scaleYSize)
{
	input.rows *= scaleYSize;
	input.cols *= scaleXSize;

	pair<int, int> oriedge[4]{ {0,0}, {0, input.cols - 1},{input.rows - 1, 0}, {input.rows - 1, input.cols - 1} };
	pair<float, float> rotedge[4]{ };
	
	for (int i = 0; i < 4; i++) {
		rotedge[i] = rot(oriedge[i], radian);
	}

	int topX{ (int)rotedge[0].second};
	int bottomX{ (int)rotedge[0].second };
	int topY{ (int)rotedge[0].second };
	int bottomY{ (int)rotedge[0].second };

	for (int i = 0; i < 4; i++) {
		if (topX < rotedge[i].second) topX = rotedge[i].second;
		if (bottomX > rotedge[i].second) bottomX = rotedge[i].second;
		if (topY < rotedge[i].first) topY = rotedge[i].first;
		if (bottomY > rotedge[i].first) bottomY = rotedge[i].first;
	}

	int xsize = topX - bottomX + 1;
	int ysize = topY - bottomY + 1;

	vector<int> size = { ysize, xsize };
	//cout << xsize << 'X' << ysize << endl;
	return size;

}

void colorotate(Mat input, float degree, float scaleXSize, float scaleYSize)
{

	int height{ 0 };
	int width{ 0 };
	height = input.rows;
	width = input.cols;

	float radian = degree * (3.14159) / 180;

	vector<int> nsize = maxCal(input, radian, scaleXSize, scaleYSize);

	Mat newMat(nsize[0], nsize[1], CV_8UC3);
	//Mat newMat = Mat::zeros(nsize[0], nsize[1], CV_8UC3);

	float ncenh = nsize[0] * 0.5;
	float ncenw = nsize[1] * 0.5;

	float cenh = height * 0.5;
	float cenw = width * 0.5;

	for (int y = 0; y < nsize[0]; y++)
	{
		for (int x = 0; x < nsize[1]; x++)
		{
			// resize
			float posX = x - ncenw;
			float posY = y - ncenh;

			pair<float, float>ori = rerot({ posY, posX }, radian);

			ori.first /= scaleYSize;
			ori.second /= scaleXSize;

			ori.first += cenh;
			ori.second += cenw;

			if (ori.first > 0 && ori.first < height - 1 && ori.second > 0 && ori.second < width - 1) {
				int x1 = 0;
				int y1 = 0;
				int x2 = 0;
				int y2 = 0;
				
				x1 = static_cast<int>(ori.second);
				y1 = static_cast<int>(ori.first);
				if (x1 + 1 >= width) {
					x2 = width - 1;
				}
				else x2 = x1 + 1;

				if (y1 + 1 >= height) {
					y2 = height - 1;
				}
				else y2 = y1 + 1;

				float a = 0.0f;
				a = ori.second - x1;
				float b = 0.0f;
				b = ori.first - y1;


				for (int i = 0; i < 3; i++) {
					newMat.at<Vec3b>(y, x)[i] = input.at<Vec3b>(y1, x1)[i] * (1 - a) * (1 - b) +
						input.at<Vec3b>(y2, x1)[i] * (1 - a) * b +
						input.at<Vec3b>(y1, x2)[i] * (1 - b) * a +
						input.at<Vec3b>(y2, x2)[i] * a * b;
					
					
					
					//cout << '('<< x << ',' << y << ')' << '=' << newMat.at<Vec3b>(y, x)[i] << endl;
				}
				
			}
		}
	}

	// 
	cv::imshow("result", newMat);
	cv::waitKey(0);/*
	cv::imwrite("C:/Users/woo12/OneDrive/Desktop/cpp/img/result"+
		(String)to_string(degree)+(String)to_string(scaleYSize)+(String)to_string(scaleXSize)+
			"2.bmp", newMat);*/
}

void rotate(Mat input, float degree, float scaleXSize, float scaleYSize)
{
	int height{ 0 };
	int width{ 0 };
	height = input.rows;
	width = input.cols;

	//float degree;
	//cin >> degree;
	float radian = degree * (3.14159) / 180;

	//nsize = { y , x } 
	vector<int> nsize = maxCal(input, radian, scaleXSize, scaleYSize);
	//cout << nsize[0] << nsize[1];
	Mat newMat(nsize[0], nsize[1], CV_8UC1);

	/*float xratio = nsize[1] / (input.cols + 1);
	float yratio = nsize[0] / (input.rows + 1);*/

	float ncenh = nsize[0] * 0.5;
	float ncenw = nsize[1] * 0.5;

	float cenh = height * 0.5;
	float cenw = width * 0.5;

	for (int y = 0; y < nsize[0]; y++)
	{
		for (int x = 0; x < nsize[1]; x++)
		{
			// resize
			float posX = x - ncenw;
			float posY = y - ncenh;

			pair<float, float>ori = rerot({ posY, posX }, radian);
			ori.first += cenh;
			ori.second += cenw;

			if (ori.first > 0 && ori.first < height - 1 && ori.second > 0 && ori.second < width - 1) {
				int x1 = static_cast<int>(ori.second);
				int y1 = static_cast<int>(ori.first);
				int x2, y2;
				if (x1 + 1 >= width) {
					x2 = width - 1;
				}
				else x2 = x1 + 1;

				if (y1 + 1 >= height) {
					y2 = height - 1;
				}
				else y2 = y1 + 1;

				float a = ori.second - x1;
				float b = ori.first - y1;


				newMat.at<uchar>(y, x) = input.at<uchar>(y1, x1) * (1 - a) * (1 - b) +
					input.at<uchar>(y2, x1) * (1 - a) * b +
					input.at<uchar>(y1, x2) * (1 - b) * a +
					input.at<uchar>(y2, x2) * a * b;
			}



		}
	}

	cv::imshow("orig", input);
	cout << degree << endl;
	cv::imshow("result", newMat);
	cv::waitKey(0);
	//imwrite(path + "result.bmp", newMat);

}

void colorScale(Mat input, float size)
{
	
	int height{ 0 };
	int width{ 0 };
	height = input.rows;
	width = input.cols;

	int newHeight = height * size;
	int newWidth = width * size;

	Mat newMat(newHeight, newWidth, CV_8UC3);

	for (int y = 0; y < newHeight; y++)
	{
		for (int x = 0; x < newWidth; x++)
		{
			// resize
			float oriX = x / size;
			float oriY = y / size;

			int x1 = static_cast<int>(oriX);
			int y1 = static_cast<int>(oriY);
			int x2, y2;
			if (x1 + 1 >= width) {
				x2 = width - 1;
			}
			else x2 = x1 + 1;

			if (y1 + 1 >= height) {
				y2 = height - 1;
			}
			else y2 = y1 + 1;

			float a = oriX - x1;
			float b = oriY - y1;

			for (int i = 0; i < 3; i++) {
				newMat.at<Vec3b>(y, x)[i] = input.at<Vec3b>(y1, x1)[i] * (1 - a) * (1 - b) +
					input.at<Vec3b>(y2, x1)[i] * (1 - a) * b +
					input.at<Vec3b>(y1, x2)[i] * (1 - b) * a +
					input.at<Vec3b>(y2, x2)[i] * a * b;
			}

		}
	}
	imshow("orig", input);
	imshow("result", newMat);
	cv::waitKey(0);
}

void grayScale()
{
	String path = "C:/Users/woo12/OneDrive/Desktop/cpp/img/";
	String img = "konkuk.jpg";
	Mat imgColor = imread(path + img, CV_LOAD_IMAGE_COLOR);
	Mat imgGrey = imread(path + img, CV_LOAD_IMAGE_GRAYSCALE);


	int height{ 0 };
	int width{ 0 };
	height = imgColor.rows;
	width = imgColor.cols;

	cout << "height = " << height;
	cout << " width = " << width << '\n';


	float size = 0.0f;
	cout << "input size = ";
	cin >> size;

	int newHeight = height * size;
	int newWidth = width * size;

	Mat newMat(newHeight, newWidth, CV_8UC3);

	for (int y = 0; y < newHeight; y++)
	{
		for (int x = 0; x < newWidth; x++)
		{
			// resize
			float oriX = x / size;
			float oriY = y / size;
			
			int x1 = static_cast<int>(oriX);
			int y1 = static_cast<int>(oriY);
			int x2, y2;
			if (x1 + 1 >= width) {
				x2 = width - 1;
			}
			else x2 = x1 + 1;

			if (y1 + 1 >= height) {
				y2 = height - 1;
			}
			else y2 = y1 + 1;

			float a = oriX - x1;
			float b = oriY - y1;

			for (int i = 0; i < 3; i++) {
				newMat.at<Vec3b>(y, x)[i] =  imgColor.at<Vec3b>(y1, x1)[i] * (1 - a) * (1 - b) +
					imgColor.at<Vec3b>(y2, x1)[i] * (1 - a) * b +
					imgColor.at<Vec3b>(y1, x2)[i] * (1 - b) * a +
					imgColor.at<Vec3b>(y2, x2)[i] * a * b;
			}
			
		}
	}
	imshow("orig", imgColor);
	imshow("result", newMat);
	cv::waitKey(0);
	//imwrite(path + "result.bmp", newMat);
	//cout << "\nnew Height = " << newHeight;
	//cout << "\nnew width = " << newWidth;
}

void knn()
{
	Mat img = imread("C:/Users/woo12/OneDrive/Desktop/cpp/img/konkuk.jpg", 0);
	int height = img.rows;
	int width = img.cols;

	float size = 2;

	//loss occur
	int nh = height * size;
	int nw = width * size;

	Mat result(nh, nw, CV_8UC1);

	for (int y = 0; y < nh; y++) {
		for (int x = 0; x < nw; x++) {
			int ox = x / size;
			int oy = y / size;
			result.at<uchar>(y, x) = img.at<uchar>(oy, ox);
		}
	}
	imshow("ori", img);
	imshow("img", result);
	cv::waitKey(0);
}
