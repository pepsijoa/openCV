#include<opencv2/opencv.hpp>
#include<iostream>
#include<cmath>

using namespace std;
using namespace cv;

float* HOG(Mat img);
float compare(float* arr1, float* arr2);
float* edgeDetectionMin(Mat img);
Mat edgeDetection(Mat img);
void grayScale(Mat img);
float sumNorm(float* arr, int size);

int main(void)
{
	
	string path = "C:/Users/woo12/OneDrive/Desktop/cpp/img/";
	
	/*Mat grey = imread(path + "Lenna.png", CV_LOAD_IMAGE_GRAYSCALE);
	grayScale(grey);*/
	
	Mat img1 = imread(path + "lecture3.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	float* img1histo = HOG(img1);
	
	Mat com1 = imread(path + "compare1.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	float* com1histo = HOG(com1);

	Mat com2 = imread(path + "result.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	float* com2histo = HOG(com2);



	cout << compare(img1histo, com1histo) << endl;
	cout << compare(img1histo, com2histo) << endl;

	delete[] com2histo;
	delete[] com1histo;
	delete[] img1histo;
	
	

	return 0;
}

// histo ok.
Mat edgeDetection(Mat img)
{
	int xmask[9]{ -1, -1, -1, 0, 0, 0, 1, 1, 1 };
	int ymask[9]{ -1, 0, 1, -1, 0, 1, -1, 0, 1 };
	int height = img.rows;
	int width = img.cols;

	float* map = new float[height * width];
	float* degreeMap = new float[height * width];

	float* degreeHis = new float[9] {0};


	Mat edgeimg = Mat::zeros(height, width, CV_8UC1);

	float max = 0.0f;
	float min = 0.0f;


	for (int y = 1; y < height - 1; y++) {
		for (int x = 1; x < width - 1; x++) {
			float fx = 0.0f;
			float fy = 0.0f;

			float magni = 0.0f;
			float degree = 0.0f;

			for (int my = 0; my <= 2; my++) {
				for (int mx = 0; mx <= 2; mx++) {
					fx += img.at<uchar>(y + (my - 1), x + (mx - 1)) * xmask[my * 3 + mx];
					fy += img.at<uchar>(y + (my - 1), x + (mx - 1)) * ymask[my * 3 + mx];
				}
			}

			// calculate magnitude
			magni = sqrt(fx * fx + fy * fy);
			if (max < magni) max = magni;
			if (min > magni) min = magni;
			map[y * width + x] = magni;

			// calculate degree
			degree = atan2(fy, fx) * 180.0 / 3.1415926535;
			if (degree < 0) degree += 180.0;

			if (magni != 0)
				degreeMap[y * width + x] = (degree - 1);
			else
				degreeMap[y * width + x] = 9999;
		}
	}



	for (int y = 1; y < height - 1; y++) {
		for (int x = 1; x < width - 1; x++) {
			edgeimg.at<uchar>(y, x) = 255 * (map[y * width + x] - min) / (max - min);
			edgeimg.at<uchar>(y, x) = 255 - edgeimg.at<uchar>(y, x);
		}
	}

	for (int y = 1; y < height - 1; y++) {
		for (int x = 1; x < width - 1; x++) {
			if (degreeMap[y * width + x] != 9999) // if no magnitude don't have degree value.
				degreeHis[(int)(degreeMap[y * width + x] / 20)] += map[y * width + x];
		}
	}

	//no norm
	for (int i = 0; i < 9; i++) {
		cout << degreeHis[i] << endl;
	}

	cout << '\n';
	//histo norm
	float rotSum = sumNorm(degreeHis, 9);
	cout << "rotSum = " << rotSum << endl;
	for (int i = 0; i < 9; i++) {
		degreeHis[i] /= rotSum;
		cout << degreeHis[i] << endl;
	}


	delete[] degreeHis;
	delete[] degreeMap;
	delete[] map;
	return edgeimg;
} 

float* edgeDetectionMin(Mat img)
{
	int xmask[9]{-1, -1, -1, 0, 0, 0, 1, 1, 1};
	int ymask[9]{ -1, 0, 1, -1, 0, 1, -1, 0, 1 };
	int height = img.rows;
	int width = img.cols;


	float* map = new float[height * width];
	float* degreeMap = new float[height * width];
	float* degreeHis = new float[9] {0};

	

	int degreebin = 9;
	int heightHistoSize = height / 8 - 1; 
	int widthHistoSize = width / 8 - 1; 
	int histoSize = heightHistoSize * widthHistoSize * degreebin;
	float* histo = new float[histoSize] {0};

	Mat edgeimg = Mat::zeros(height, width, CV_8UC1);

	float max = 0.0f;
	float min = 0.0f;


	for (int y = 1; y < height-1; y++) {
		for (int x = 1; x < width-1; x++) {
			float fx = 0.0f;
			float fy = 0.0f;
			
			float magni = 0.0f;
			float degree = 0.0f;

			for (int my = 0; my <= 2; my++) {
				for (int mx = 0; mx <= 2; mx++) {
					fx += img.at<uchar>(y + (my - 1), x + (mx - 1)) * xmask[my*3+mx];
					fy += img.at<uchar>(y + (my - 1), x + (mx - 1)) * ymask[my*3+mx];
				}
			}

			// calculate magnitude
			magni = sqrt(fx * fx + fy * fy);
			if (max < magni) max = magni;
			if (min > magni) min = magni;
			map[y*width+x] = magni;
		
			// calculate degree
			degree = atan2(fy, fx) * 180.0 / 3.1415926535;
			if (degree < 0) degree += 180.0;
			
			if (magni != 0)
				degreeMap[y * width + x] = (degree - 1);
			else
				degreeMap[y * width + x] = 9999;
		}
	}

	for (int y = 1; y < height - 1; y++) {
		for (int x = 1; x < width - 1; x++) {
			
			if (degreeMap[y * width + x] != 9999) {
				int hIndex = 0;
				int wIndex = 0;
				int degreeIndex = 0;
				degreeIndex = (int)(degreeMap[y * width + x] / 20);
				
				if (x / 8 == 0) {
					wIndex = 0;
					if (y / 8 == 0) {
						hIndex = 0;
						histo[hIndex * widthHistoSize * degreebin +
							wIndex * degreebin +
							degreeIndex] += map[y * width + x];
					}
					else if (y / 8 == heightHistoSize) {
						hIndex = heightHistoSize - 1;
						histo[hIndex * widthHistoSize * degreebin +
							wIndex * degreebin +
							degreeIndex] += map[y * width + x];
					}
					else {
						hIndex = y / 8;
						histo[hIndex * widthHistoSize * degreebin +
							wIndex * degreebin +
							degreeIndex] += map[y * width + x];
						histo[(hIndex - 1) * widthHistoSize * degreebin +
							wIndex * degreebin +
							degreeIndex] += map[y * width + x];
					}
				}
				else if (x / 8 == widthHistoSize) {
					wIndex = widthHistoSize - 1;
					if (y / 8 == 0) {
						hIndex = 0;
						histo[hIndex * widthHistoSize * degreebin +
							wIndex * degreebin +
							degreeIndex] += map[y * width + x];
					}
					else if (y / 8 == heightHistoSize) {
						hIndex = heightHistoSize - 1;
						histo[hIndex * widthHistoSize * degreebin +
							wIndex * degreebin +
							degreeIndex] += map[y * width + x];
					}
					else {
						hIndex = y / 8;
						histo[hIndex * widthHistoSize * degreebin +
							wIndex * degreebin +
							degreeIndex] += map[y * width + x];
						histo[(hIndex - 1) * widthHistoSize * degreebin +
							wIndex * degreebin +
							degreeIndex] += map[y * width + x];
					}
				}
				else { // width is middle
					wIndex = x / 8;
					if (y / 8 == 0) {
						hIndex = 0;
						histo[hIndex * widthHistoSize * degreebin +
							wIndex * degreebin +
							degreeIndex] += map[y * width + x];
						histo[hIndex * widthHistoSize * degreebin +
							(wIndex - 1) * degreebin +
							degreeIndex] += map[y * width + x];
					}
					else if (y / 8 == heightHistoSize) {
						hIndex = heightHistoSize - 1;
						histo[hIndex * widthHistoSize * degreebin +
							wIndex * degreebin +
							degreeIndex] += map[y * width + x];
						histo[hIndex * widthHistoSize * degreebin +
							(wIndex - 1) * degreebin +
							degreeIndex] += map[y * width + x];
					}
					else { // include 4 blocks
						hIndex = y / 8;
						histo[hIndex * widthHistoSize * degreebin +
							wIndex * degreebin +
							degreeIndex] += map[y * width + x];
						histo[(hIndex - 1) * widthHistoSize * degreebin +
							wIndex * degreebin +
							degreeIndex] += map[y * width + x];
						histo[(hIndex - 1) * widthHistoSize * degreebin +
							(wIndex - 1) * degreebin +
							degreeIndex] += map[y * width + x];
						histo[hIndex * widthHistoSize * degreebin +
							(wIndex - 1) * degreebin +
							degreeIndex] += map[y * width + x];

					}
				}
			}
			
		}
	}

	for (int i = 0; i < heightHistoSize; i++) {
		for (int j = 0; j < widthHistoSize; j++) {

			
			float sum = 0.0f;
			for (int k = 0; k < degreebin; k++) {
				degreeHis[k] = histo[i * widthHistoSize * degreebin + j * degreebin + k];
			}
			sum = sumNorm(degreeHis, 9);

			if (sum != 0) {
				for (int k = 0; k < degreebin; k++) {
					histo[i * widthHistoSize * degreebin + j * degreebin + k] /= sum;
				}
			}
		}
	}

	/*for (int i = 0; i < 945; i++) {
		cout << histo[i] << endl;
	}
	cout << "work" << endl;*/


	delete[] degreeHis;
	delete[] degreeMap;
	delete[] map;
	return histo;
}

float sumNorm(float* arr, int size)
{
	float sum = 0.0f;
	for (int i = 0; i < size; i++) {
		sum += arr[i] * arr[i];
	}
	return sqrt(sum);
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
				degreeIndex = (int)(degree / 20.0);

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
					histo[(hIndex - 1) * widthHistoSize * degreebin +
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
					histo[(hIndex - 1) * widthHistoSize * degreebin +
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
						(wIndex - 1) * degreebin +
						degreeIndex] += magni;
				}
				else if (y / 8 == heightHistoSize) {
					hIndex = heightHistoSize - 1;
					histo[hIndex * widthHistoSize * degreebin +
						wIndex * degreebin +
						degreeIndex] += magni;
					histo[hIndex * widthHistoSize * degreebin +
						(wIndex - 1) * degreebin +
						degreeIndex] += magni;
				}
				else { // include 4 blocks
					hIndex = y / 8;
					histo[hIndex * widthHistoSize * degreebin +
						wIndex * degreebin +
						degreeIndex] += magni;
					histo[(hIndex - 1) * widthHistoSize * degreebin +
						wIndex * degreebin +
						degreeIndex] += magni;
					histo[(hIndex - 1) * widthHistoSize * degreebin +
						(wIndex - 1) * degreebin +
						degreeIndex] += magni;
					histo[hIndex * widthHistoSize * degreebin +
						(wIndex - 1) * degreebin +
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


	/*cv::imshow("ori", img);
	cv::imshow("edge", edgeimg);
	cv::waitKey(0);
	cout << "work" << endl;*/
	return histo;
}

void grayScale(Mat img)
{
	
	int height{ 0 };
	int width{ 0 };
	height = img.rows;
	width = img.cols;

	
	float heightSize = 128.0 / (float)height;
	float widthSize = 64.0 / (float)width;

	int newheight = height * heightSize;
	int newwidth = width * widthSize;


	Mat newMat(newheight, newwidth, CV_8UC1);
	for (int y = 0; y < newheight; y++)
	{
		for (int x = 0; x < newwidth; x++)
		{
			// resize
			float oriX = x / widthSize;
			float oriY = y / heightSize;

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

			newMat.at<uchar>(y, x)= img.at<uchar>(y1, x1) * (1 - a) * (1 - b) +
				img.at<uchar>(y2, x1) * (1 - a) * b +
				img.at<uchar>(y1, x2) * (1 - b) * a +
				img.at<uchar>(y2, x2) * a * b;
		}
	}

	cv::imwrite("C:/Users/woo12/OneDrive/Desktop/cpp/img/result.bmp", newMat);
}