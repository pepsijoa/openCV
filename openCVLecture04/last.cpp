//#include<opencv2/opencv.hpp>
//#include<iostream>
//#include<cmath>
//
//using namespace std;
//using namespace cv;
//
//float compare(float* arr1, float* arr2);
//float* edgeDetectionMin(Mat img);
//void grayScale(Mat img);
//float sumNorm(float* arr, int size);
//void minNorm(float* map, float max, float min, int size);
//
//int main(void)
//{
//
//	string path = "C:/Users/woo12/OneDrive/Desktop/cpp/img/";
//	/*Mat resize = imread(path + "cat.jpg", CV_LOAD_IMAGE_GRAYSCALE);
//	grayScale(resize);*/
//	Mat img1 = imread(path + "lecture3.bmp", CV_LOAD_IMAGE_GRAYSCALE);
//	float* img1histo = edgeDetectionMin(img1);
//
//	Mat com1 = imread(path + "compare1.bmp", CV_LOAD_IMAGE_GRAYSCALE);
//	float* com1histo = edgeDetectionMin(com1);
//
//	Mat com2 = imread(path + "compare2.bmp", CV_LOAD_IMAGE_GRAYSCALE);
//	float* com2histo = edgeDetectionMin(com2);
//
//	//code for output excel
//	std::ofstream file("output.csv");
//	float v1 = 0.0;
//	float v2 = 0.0;
//	float v3 = 0.0;
//
//	if (file.is_open()) {
//		file << "v1, v2, v3\n";
//		for (int i = 0; i < 945; i++)
//		{
//			file << img1histo[i] << ',' << com1histo[i] << ',' << com2histo[i] << '\n';
//		}
//		file.close();
//	}
//
//
//
//	cout << compare(img1histo, com1histo) << endl;
//	cout << compare(img1histo, com2histo) << endl;
//
//
//
//	delete[] com2histo;
//	delete[] com1histo;
//	delete[] img1histo;
//
//	imshow("com3", com2);
//	waitKey(0);
//
//	return 0;
//}
//
//void minNorm(float* map, float max, float min, int size)
//{
//	for (int i = 0; i < size; i++)
//	{
//		map[i] = (map[i] - min) / (max - min);
//	}
//}
//
//float* edgeDetectionMin(Mat img)
//{
//	int xmask[9]{ -1, -1, -1, 0, 0, 0, 1, 1, 1 };
//	int ymask[9]{ -1, 0, 1, -1, 0, 1, -1, 0, 1 };
//	int height = img.rows;
//	int width = img.cols;
//
//
//	float* map = new float[height * width];
//	float* degreeMap = new float[height * width];
//	float* degreeHis = new float[9] {0};
//
//
//
//	int degreebin = 9;
//	int heightHistoSize = height / 8 - 1;
//	int widthHistoSize = width / 8 - 1;
//
//	int histoSize = heightHistoSize * widthHistoSize * degreebin;
//
//	float* histo = new float[histoSize] {0};
//
//	Mat edgeimg = Mat::zeros(height, width, CV_8UC1);
//
//	float max = 0.0f;
//	float min = 0.0f;
//
//
//	for (int y = 1; y < height - 1; y++) {
//		for (int x = 1; x < width - 1; x++) {
//			float fx = 0.0f;
//			float fy = 0.0f;
//
//			float magni = 0.0f;
//			float degree = 0.0f;
//
//			for (int my = 0; my <= 2; my++) {
//				for (int mx = 0; mx <= 2; mx++) {
//					fx += img.at<uchar>(y + (my - 1), x + (mx - 1)) * xmask[my * 3 + mx];
//					fy += img.at<uchar>(y + (my - 1), x + (mx - 1)) * ymask[my * 3 + mx];
//				}
//			}
//
//			// calculate magnitude
//			// get max and min for normal
//			magni = sqrt(fx * fx + fy * fy);
//			if (max < magni) max = magni;
//			if (min > magni) min = magni;
//			map[y * width + x] = magni;
//
//			// calculate degree
//			degree = atan2(fy, fx) * 180.0 / 3.1415926535;
//			if (degree < 0) degree += 180.0;
//
//			if (magni != 0)
//				degreeMap[y * width + x] = (degree - 1);
//			else
//				degreeMap[y * width + x] = 9999;
//		}
//	}
//
//	//magnitude normalization
//	minNorm(map, max, min, height * width);
//
//
//	for (int y = 1; y < height - 1; y++) {
//		for (int x = 1; x < width - 1; x++) {
//
//			if (degreeMap[y * width + x] != 9999) {
//				int hIndex = 0;
//				int wIndex = 0;
//				int degreeIndex = 0;
//				degreeIndex = (int)(degreeMap[y * width + x] / 20);
//
//				if (x / 8 == 0) {
//					wIndex = 0;
//					if (y / 8 == 0) {
//						hIndex = 0;
//						histo[hIndex * widthHistoSize * degreebin +
//							wIndex * degreebin +
//							degreeIndex] += map[y * width + x];
//					}
//					else if (y / 8 == heightHistoSize) {
//						hIndex = heightHistoSize - 1;
//						histo[hIndex * widthHistoSize * degreebin +
//							wIndex * degreebin +
//							degreeIndex] += map[y * width + x];
//					}
//					else {
//						hIndex = y / 8;
//						histo[hIndex * widthHistoSize * degreebin +
//							wIndex * degreebin +
//							degreeIndex] += map[y * width + x];
//						histo[(hIndex - 1) * widthHistoSize * degreebin +
//							wIndex * degreebin +
//							degreeIndex] += map[y * width + x];
//					}
//				}
//				else if (x / 8 == widthHistoSize) {
//					wIndex = widthHistoSize - 1;
//					if (y / 8 == 0) {
//						hIndex = 0;
//						histo[hIndex * widthHistoSize * degreebin +
//							wIndex * degreebin +
//							degreeIndex] += map[y * width + x];
//					}
//					else if (y / 8 == heightHistoSize) {
//						hIndex = heightHistoSize - 1;
//						histo[hIndex * widthHistoSize * degreebin +
//							wIndex * degreebin +
//							degreeIndex] += map[y * width + x];
//					}
//					else {
//						hIndex = y / 8;
//						histo[hIndex * widthHistoSize * degreebin +
//							wIndex * degreebin +
//							degreeIndex] += map[y * width + x];
//						histo[(hIndex - 1) * widthHistoSize * degreebin +
//							wIndex * degreebin +
//							degreeIndex] += map[y * width + x];
//					}
//				}
//				else { // width is middle
//					wIndex = x / 8;
//					if (y / 8 == 0) {
//						hIndex = 0;
//						histo[hIndex * widthHistoSize * degreebin +
//							wIndex * degreebin +
//							degreeIndex] += map[y * width + x];
//						histo[hIndex * widthHistoSize * degreebin +
//							(wIndex - 1) * degreebin +
//							degreeIndex] += map[y * width + x];
//					}
//					else if (y / 8 == heightHistoSize) {
//						hIndex = heightHistoSize - 1;
//						histo[hIndex * widthHistoSize * degreebin +
//							wIndex * degreebin +
//							degreeIndex] += map[y * width + x];
//						histo[hIndex * widthHistoSize * degreebin +
//							(wIndex - 1) * degreebin +
//							degreeIndex] += map[y * width + x];
//					}
//					else { // include 4 blocks
//						hIndex = y / 8;
//						histo[hIndex * widthHistoSize * degreebin +
//							wIndex * degreebin +
//							degreeIndex] += map[y * width + x];
//						histo[(hIndex - 1) * widthHistoSize * degreebin +
//							wIndex * degreebin +
//							degreeIndex] += map[y * width + x];
//						histo[(hIndex - 1) * widthHistoSize * degreebin +
//							(wIndex - 1) * degreebin +
//							degreeIndex] += map[y * width + x];
//						histo[hIndex * widthHistoSize * degreebin +
//							(wIndex - 1) * degreebin +
//							degreeIndex] += map[y * width + x];
//
//					}
//				}
//			}
//		}
//	}
//
//	for (int i = 0; i < heightHistoSize; i++) {
//		for (int j = 0; j < widthHistoSize; j++) {
//
//
//			float sum = 0.0f;
//			for (int k = 0; k < degreebin; k++) {
//				degreeHis[k] = histo[i * widthHistoSize * degreebin + j * degreebin + k];
//			}
//			sum = sumNorm(degreeHis, 9);
//
//			if (sum != 0) {
//				for (int k = 0; k < degreebin; k++) {
//					histo[i * widthHistoSize * degreebin + j * degreebin + k] /= sum;
//				}
//			}
//		}
//	}
//
//
//
//	delete[] degreeHis;
//	delete[] degreeMap;
//	delete[] map;
//	return histo;
//}
//
//float sumNorm(float* arr, int size)
//{
//	float sum = 0.0f;
//	for (int i = 0; i < size; i++) {
//		sum += arr[i] * arr[i];
//	}
//	return sqrt(sum);
//}
//
//float compare(float* arr1, float* arr2)
//{
//	float sum = 0.0f;
//	for (int i = 0; i < 945; i++) {
//		sum += sqrt((arr1[i] - arr2[i]) * (arr1[i] - arr2[i]));
//	}
//	return sum;
//}
//
////image resize, by lecture02
//void grayScale(Mat img)
//{
//
//	int height{ 0 };
//	int width{ 0 };
//	height = img.rows;
//	width = img.cols;
//
//
//	float heightSize = 128.0 / (float)height;
//	float widthSize = 64.0 / (float)width;
//
//	int newheight = 128;
//	int newwidth = 64;
//
//
//	Mat newMat(newheight, newwidth, CV_8UC1);
//	for (int y = 0; y < newheight; y++)
//	{
//		for (int x = 0; x < newwidth; x++)
//		{
//			// resize
//			float oriX = x / widthSize;
//			float oriY = y / heightSize;
//
//			int x1 = static_cast<int>(oriX);
//			int y1 = static_cast<int>(oriY);
//			int x2, y2;
//			if (x1 + 1 >= width) {
//				x2 = width - 1;
//			}
//			else x2 = x1 + 1;
//
//			if (y1 + 1 >= height) {
//				y2 = height - 1;
//			}
//			else y2 = y1 + 1;
//
//			float a = oriX - x1;
//			float b = oriY - y1;
//
//			newMat.at<uchar>(y, x) = img.at<uchar>(y1, x1) * (1 - a) * (1 - b) +
//				img.at<uchar>(y2, x1) * (1 - a) * b +
//				img.at<uchar>(y1, x2) * (1 - b) * a +
//				img.at<uchar>(y2, x2) * a * b;
//		}
//	}
//
//	cv::imwrite("C:/Users/woo12/OneDrive/Desktop/cpp/img/cat.bmp", newMat);
//}