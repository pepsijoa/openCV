#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include "SLIC.h"
using namespace cv;
using namespace std;

void maskgrab(Mat img, Mat input_mask)
{
	int x, y;
	int width, height;
	height = img.rows;
	width = img.cols;

	//Rect rectangle(0, 0, width - 1, height - 1);
	Rect rectangle(0, 0, 0, 0);
	Mat mask = Mat::ones(height, width, CV_8UC1) * GC_PR_BGD;

	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			if (input_mask.at<Vec3b>(y, x)[0] == 0 and
				input_mask.at<Vec3b>(y, x)[1] == 255 and
				input_mask.at<Vec3b>(y, x)[2] == 255)
				mask.at<uchar>(y, x) = GC_BGD;

			if (input_mask.at<Vec3b>(y, x)[0] == 0 and
				input_mask.at<Vec3b>(y, x)[1] == 0 and
				input_mask.at<Vec3b>(y, x)[2] == 255)
				mask.at<uchar>(y, x) = GC_FGD;
		}
	}

	Mat final_result = Mat::zeros(height, width, CV_8UC3);
	Mat mask_result = Mat::zeros(height, width, CV_8UC1);
	Mat bg, fg;

	final_result = img.clone();

	grabCut(img, mask, rectangle, bg, fg, 8, GC_INIT_WITH_MASK);
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			if (mask.at<uchar>(y, x) == GC_FGD || mask.at<uchar>(y, x) == GC_PR_FGD) {
				mask_result.at<uchar>(y, x) = 255;
			}
			else {
				final_result.at<Vec3b>(y, x)[0] = 255;
				final_result.at<Vec3b>(y, x)[1] = 255;
				final_result.at<Vec3b>(y, x)[2] = 255;
			}
		}
	}
	imshow("final_result", final_result);
	imshow("mask_result", mask_result);
	waitKey(0);
}

void SLICsegmentation(Mat image, int k)
{
	SLIC slic;
	int x, y;
	int height, width;
	int numlabels; // Generated number of superpixels
	int m_spcount = k; // Desired number of superpixels
	double m_compactness = 40;// 20.0; // compactness factor (1-40)
	height = image.rows;
	width = image.cols;
	unsigned int* ubuff = (unsigned int*)calloc(height * width, sizeof(unsigned int));
	int* labels = (int*)calloc(height * width, sizeof(int));
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			ubuff[y * width + x] = (int)image.at<Vec3b>(y, x)[0] + ((int)image.at<Vec3b>(y, x)[1] << 8) +
				((int)image.at<Vec3b>(y, x)[2] << 16);
		}
	}
	slic.DoSuperpixelSegmentation_ForGivenNumberOfSuperpixels(ubuff, width, height, labels,
		numlabels, m_spcount, m_compactness);
	
	Mat result(height, width, CV_8UC3);
	slic.DrawContoursAroundSegments(ubuff, labels, width, height, 0);
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			result.at<Vec3b>(y, x)[0] = ubuff[y * width + x] & 0xff;
			result.at<Vec3b>(y, x)[1] = ubuff[y * width + x] >> 8 & 0xff;
			result.at<Vec3b>(y, x)[2] = ubuff[y * width + x] >> 16 & 0xff;
		}
	}
	imshow("SLIC_segmentation.bmp", result);
	waitKey(0);


	free(ubuff);
	free(labels);
}

void main()
{
	Mat image = imread("car.jpg");
	for (int i = 10; i < 500; i += 10)
	{
		cout << i << '\n';
		SLICsegmentation(image, i);
	}
}
