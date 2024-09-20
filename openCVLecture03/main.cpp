#include<opencv2/opencv.hpp>
#include<cstdlib>
#include<cmath>
	
using namespace std;
using namespace cv;
string path = "C:/Users/woo12/OneDrive/Desktop/cpp/img/";

int xmask[3][3]{
	{-1, 0, 1},
	{-1, 0, 1},
	{-1, 0, 1},
};

int ymask[3][3]{
	{-1, -1, -1},
	{ 0,  0,  0},
	{ 1,  1,  1},
};

queue<float>pixel;
vector<float>degreeHis;

int main(void)
{
	
	int height, width;
	Mat img = imread(path + "Lenna.png", CV_LOAD_IMAGE_GRAYSCALE);
	height = img.rows;
	width = img.cols;

	Mat imgEdge(height, width, CV_8UC1);
	Mat imgEdgeNorm(height, width, CV_8UC1);
	Mat imgEdgeZNorm(height, width, CV_8UC1);


	float maxMagni = 0.0f;
	float minMagni = 0.0f;

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			float fx = 0.f;
			float fy = 0.f;
			
			float magni = 0.f;
			float degree = 0.f;

			for (int my = 0; my <= 2; my++) {
				for (int mx = 0; mx <= 2; mx++) {
					fx += img.at<uchar>(y + (my - 1), x + (mx - 1) ) * xmask[my][mx];
					fy += img.at<uchar>(y + (my - 1), x + (mx - 1)) * ymask[my][mx];
				}
			}

			magni = sqrt(fx * fx + fy * fy);

			//without normalization
			imgEdge.at<uchar>(y, x) = magni;
			
			
			if (maxMagni < magni) maxMagni = magni;
			if (minMagni > magni) minMagni = magni;
			pixel.push(magni);
			
			degree = atan(fy / fx) * (180.0 / 3.14159);
			degreeHis.push_back(degree);
		}
	}

	float correct = 0.0f;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			float curPixel = pixel.front(); pixel.pop();
			curPixel = (curPixel - minMagni) / (maxMagni - minMagni);
			curPixel *= 255;

			if (correct < curPixel) correct = curPixel;

			imgEdgeNorm.at<uchar>(y, x) = (int)curPixel;
		}
	}
	
	cv::imshow("ori", img);
	cv::imshow("No norm", imgEdge);
	cv::imshow("With norm", imgEdgeNorm);
	waitKey(0);
	/*cout << height << '\n' << width << endl;
	cv::imshow("lenna", img);
	waitKey(0);*/
}

