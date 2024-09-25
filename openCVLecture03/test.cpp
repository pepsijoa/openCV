#include<opencv2/opencv.hpp>
#include<iostream>
#include<cmath>

using namespace std;
using namespace cv;

float* HOG(Mat img);
float compare(float* arr1, float* arr2);

int main(void)
{
	
	string path = "C:/Users/woo12/OneDrive/Desktop/cpp/img/";
	Mat img1 = imread(path + "lecture3.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	float* img1histo = HOG(img1);
		
	Mat com1 = imread(path + "compare1.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	float* com1histo = HOG(com1);

	Mat com2 = imread(path + "compare2.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	float* com2histo = HOG(com2);

	cout << compare(com2histo, com1histo) << endl;
	//cout << compare(img1histo, com2histo) << endl;

	delete[] com2histo;
	delete[] com1histo;
	delete[] img1histo;
	return 0;
}

float compare(float* arr1, float* arr2)
{
	float sum = 0.0f;
	for (int i = 0; i < 945; i++) {
		sum += sqrt((arr1[i] - arr2[i]) * (arr1[i] - arr2[i]));
	}
	return sum;
}

float* HOG(Mat img)
{

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

	int height = img.rows;
	int width = img.cols;
	int degreebin = 9;


	int heightHistoSize = height / 8 - 1; // 128X64 -> 15
	int widthHistoSize = width / 8 - 1; // 128X64 -> 7

	int size = heightHistoSize * widthHistoSize * degreebin;
	cout << "array size = " << size << endl;

	float* histo = new float[size];
	for (int i = 0; i < size; i++) histo[i] = 0;


	//check line for debug.
	Mat edgeimg = Mat::zeros(height, width, CV_8UC1);

	for (int y = 1; y < height - 1; y++) {
		for (int x = 1; x < width - 1; x++) {

			float fx = 0.0f;
			float fy = 0.0f;

			float magni = 0.0f;
			double degree = 0.0f;

			for (int my = 0; my <= 2; my++) {
				for (int mx = 0; mx <= 2; mx++) {
					fx += img.at<uchar>(y + (my - 1), x + (mx - 1)) * xmask[my][mx];
					fy += img.at<uchar>(y + (my - 1), x + (mx - 1)) * ymask[my][mx];
				}
			}

			//calculate magnitude
			magni = sqrt(fx * fx + fy * fy);
			int normMagni = max(0, min(255, (int)magni));
			edgeimg.at<uchar>(y, x) = normMagni;

			//calculate orientation
			degree = atan2(fy, fx) * (180.0 / 3.14159);
			if (degree < 0) degree += 180.0;

			int hIndex = 0;
			int wIndex = 0;
			int degreeIndex = 0;

			// histo index cal
			if (degree == 180)
				degreeIndex = degreebin - 1;
			else
				degreeIndex =  (int) (degree / 20.0);
			
			if (x / 8 == 0) {
				wIndex = 0;
				if (y / 8 == 0) {
					hIndex = 0;
					histo[hIndex * widthHistoSize * degreebin +
						wIndex * degreebin +
						degreeIndex] += magni;
				}
				else if (y / 8 == heightHistoSize) {
					hIndex = heightHistoSize - 1;
					histo[hIndex * widthHistoSize * degreebin +
						wIndex * degreebin +
						degreeIndex] += magni;
				}
				else {
					hIndex = y / 8;
					histo[hIndex * widthHistoSize * degreebin +
						wIndex * degreebin +
						degreeIndex] += magni;
					histo[(hIndex-1)*widthHistoSize * degreebin +
						wIndex * degreebin +
						degreeIndex] += magni;
				}
			}
			else if (x / 8 == widthHistoSize) {
				wIndex = widthHistoSize - 1;
				if (y / 8 == 0) {
					hIndex = 0;
					histo[hIndex * widthHistoSize * degreebin +
						wIndex * degreebin +
						degreeIndex] += magni;
				}
				else if (y / 8 == heightHistoSize) {
					hIndex = heightHistoSize - 1;
					histo[hIndex * widthHistoSize * degreebin +
						wIndex * degreebin +
						degreeIndex] += magni;
				}
				else {
					hIndex = y / 8;
					histo[hIndex * widthHistoSize * degreebin +
						wIndex * degreebin +
						degreeIndex] += magni;
					histo[(hIndex-1)*widthHistoSize * degreebin +
						wIndex * degreebin +
						degreeIndex] += magni;
				}
			}
			else { // width is middle
				wIndex = x / 8;
				if (y / 8 == 0) {
					hIndex = 0;
					histo[hIndex * widthHistoSize * degreebin +
						wIndex * degreebin +
						degreeIndex] += magni;
					histo[hIndex * widthHistoSize * degreebin +
						(wIndex-1)*degreebin +
						degreeIndex] += magni;
				}
				else if (y / 8 == heightHistoSize) {
					hIndex = heightHistoSize - 1;
					histo[hIndex * widthHistoSize * degreebin +
						wIndex * degreebin +
						degreeIndex] += magni;
					histo[hIndex * widthHistoSize * degreebin +
						(wIndex-1)*degreebin +
						degreeIndex] += magni;
				}
				else { // include 4 blocks
					hIndex = y / 8;
					histo[hIndex * widthHistoSize * degreebin +
						wIndex * degreebin +
						degreeIndex] += magni;
					histo[(hIndex-1)*widthHistoSize * degreebin +
						wIndex * degreebin +
						degreeIndex] += magni;
					histo[(hIndex-1)*widthHistoSize * degreebin +
						(wIndex-1)*degreebin +
						degreeIndex] += magni;
					histo[hIndex * widthHistoSize * degreebin +
						(wIndex-1)*degreebin +
						degreeIndex] += magni;

				}
			}
		}
	}

	
	for (int i = 0; i < heightHistoSize; i++) {
		for (int j = 0; j < widthHistoSize; j++) {
			
			float sum = 0.0f;
			for (int k = 0; k < degreebin; k++) {
				sum += histo[i * widthHistoSize * degreebin + j * degreebin + k] *
					histo[i * widthHistoSize * degreebin + j * degreebin + k];
			}

			if (sum != 0) {
				for (int k = 0; k < degreebin; k++) {
					histo[i * widthHistoSize * degreebin + j * degreebin + k] /= sqrt(sum);
				}
			}
		}
	}

	/*for (int i = 0; i < 945; i++) {
		cout << histo[i] << endl;
	}*/
	

	/*cv::imshow("ori", img);
	cv::imshow("edge", edgeimg);
	cv::waitKey(0);
	cout << "work" << endl;*/
	return histo;
}
