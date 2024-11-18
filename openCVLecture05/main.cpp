#include<opencv2/opencv.hpp>
#include <string>
#include <Windows.h>


using namespace std;
using namespace cv;

struct ImgHisto {
	string name;
	vector<pair<int, int>> location;
	vector<float> histo;
	size_t cornerCount = 0;

	ImgHisto(size_t cornercounter, string name)
		:location(cornercounter), histo(cornercounter * 9), cornerCount(cornercounter), name(name) {
	}
};

Mat cornerDetection(Mat img, ImgHisto& allhisto);
void minNorm(float* map, float max, float min, int size);
Mat lowPassFilter(Mat img);
float* centerHisto(int centerX, int centerY, float* degreeMap, float* magniMap, int height, int width);
cv::Point compareHisto(ImgHisto& h1, ImgHisto& h2, int h1index, int h2size);
Mat showWindow(ImgHisto& img1, ImgHisto& img2);

string path = "C:/Users/woo12/OneDrive/Desktop/cpp/img/";

Mat originalimg;
Mat frame, rotateframe;



int main(void)
{
	LARGE_INTEGER freq, start, stop;
	double diff;

	originalimg = imread(path + "erase.jpg", CV_LOAD_IMAGE_COLOR);
	ImgHisto originHisto(originalimg.rows * originalimg.cols, "origin");
	cornerDetection(originalimg, originHisto);

	VideoCapture capture(0);
	Mat cornerMap(frame.rows, frame.cols, CV_8UC3);

	while (true) {
		capture >> frame;
		QueryPerformanceFrequency(&freq); //test frame
		QueryPerformanceCounter(&start); //test frame

		ImgHisto video(frame.rows * frame.cols, "video");
		imshow("result", cornerDetection(frame, video));

		//imshow("Corner", showWindow(originHisto, video));
		
		QueryPerformanceCounter(&stop); //test frame
		diff = (double)(stop.QuadPart - start.QuadPart) / freq.QuadPart; //test frame
		cout << diff << '\n';  //test frame
		
		if (waitKey(30) >= 0) break;
	}
}

Mat lowPassFilter(Mat img)
{
	Mat output = img.clone();
	for (int y = 1; y < img.rows - 1; y++) {
		for (int x = 1; x < img.cols - 1; x++) {

			float mask = 0;
			for (int ymask = -1; ymask <= 1; ymask++) {
				for (int xmask = -1; xmask <= 1; xmask++) {
					mask += img.at<uchar>(y + ymask, x + xmask);
				}
			}
			mask = mask * 0.111;
			output.at<uchar>(y, x) = mask;
		}
	}
	return output;
}
Mat cornerDetection(Mat colorimg, ImgHisto& allhisto)
{
	int height = colorimg.rows;
	int width = colorimg.cols;

	// color img to gray img.
	Mat img = Mat::zeros(height, width, CV_8UC1);
	
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			img.at<uchar>(y, x) = (colorimg.at<Vec3b>(y, x)[0] +
				colorimg.at<Vec3b>(y, x)[1] +
				colorimg.at<Vec3b>(y, x)[2]) * 0.3333;
		}
	}

	img = lowPassFilter(img);


	int xmask[9]{ -1, -1, -1, 0, 0, 0, 1, 1, 1 };
	int ymask[9]{ -1, 0, 1, -1, 0, 1, -1, 0, 1 };


	float* ix = new float[height * width];
	float* iy = new float[height * width];

	float* magniSize = new float[height * width];
	float* degreeSize = new float[height * width];

	float ixmax = 0.0f;
	float ixmin = 0.0f;

	float iymax = 0.0f;
	float iymin = 0.0f;

	for (int y = 1; y < height - 1; y++) {
		for (int x = 1; x < width - 1; x++) {
			float fx = 0.0f;
			float fy = 0.0f;

			float magni = 0.0f;
			float degree = 0.0f;

			for (int my = 0; my <= 2; my++) {
				for (int mx = 0; mx <= 2; mx++) {
					fx += img.at<uchar>(y + (my - 1), x + (mx - 1)) * xmask[my * 3 + mx] / 255.0;
					fy += img.at<uchar>(y + (my - 1), x + (mx - 1)) * ymask[my * 3 + mx] / 255.0;
				}
			}

			//calculate magni
			magni = sqrt(fx * fx + fy * fy);
			magniSize[y * width + x] = magni;

			//calgulate degree
			degree = atan2(fy, fx) * 180.0 / 3.1415926535;
			if (degree < 0) degree += 180.0;
			if (magni != 0)
				degreeSize[y * width + x] = (degree - 1);
			else
				degreeSize[y * width + x] = 9999;


			ix[y * width + x] = fx;
			iy[y * width + x] = fy;
		}

	}

	float* R = new float[height * width];
	int* corner = new int[height * width];

	int cornerCount = 0;

	float k = 0.04f;
	float maxR = 0.0f;
	float minR = 0.0f;

	float th = 2.0f;

	//calculte R
	for (int y = 1; y < height - 1; y++) {
		for (int x = 1; x < width - 1; x++) {

			float a = 0.0f; // 1 1
			float b = 0.0f;
			float d = 0.0f; // 4 4
			for (int my = 0; my <= 2; my++) {
				for (int mx = 0; mx <= 2; mx++) {
					a += ix[(y + (my - 1)) * width + x + (mx - 1)] * ix[(y + (my - 1)) * width + x + (mx - 1)];
					b += ix[(y + (my - 1)) * width + x + (mx - 1)] * iy[(y + (my - 1)) * width + x + (mx - 1)];
					d += iy[(y + (my - 1)) * width + x + (mx - 1)] * iy[(y + (my - 1)) * width + x + (mx - 1)];

				}
			}
			float r = (a * d) - (b * b) - k * (a + d) * (a + d);

			if (r > th) {
				corner[y * width + x] = 1;
				cornerCount++;
			}

			R[y * width + x] = r;
			if (maxR < r) maxR = r;
			if (minR > r) minR = r;

		}
	}

	delete[] ix;
	delete[] iy;

	// calculate histogram around center
	int ceninput = 0;
	for (int y = 1; y < height - 1; y++) {
		for (int x = 1; x < width - 1; x++) {
			if (corner[y * width + x] == 1) {
				float* histo = centerHisto(x, y, degreeSize, magniSize, height, width);
				allhisto.cornerCount = cornerCount;
				allhisto.location[ceninput].first = y;
				allhisto.location[ceninput].second = x;
				for (int i = 0; i < 9; i++) {
					allhisto.histo[ceninput * 9 + i] = histo[i];
				}
				ceninput++;
				delete[] histo;
			}
		}
	}

	// spot corner visualization
	Point pcenter;
	Scalar color(0, 255, 0);
	for (int y = 1; y < height - 1; y++) {
		for (int x = 1; x < width - 1; x++) {
			if (corner[y * width + x] == 1) {
				pcenter.x = x;
				pcenter.y = y;
				circle(colorimg, pcenter, 1, color, 1, 8, 0);
			}

		}
	}
	return colorimg;

	delete[] degreeSize;
	delete[] magniSize;
	delete[] corner;
	delete[] R;
}

float* centerHisto(int centerX, int centerY, float* degreeMap, float* magniMap, int height, int width)
{
	float* histo = new float[9] {0};

	float hmax = 0.0f;
	float hmin = 0.0f;

	for (int y = -8; y <= 8; y++) {
		for (int x = -8; x <= 8; x++) {
			int pointX = centerX + x;
			int pointY = centerY + y;
			if (pointX >= 0 and pointX < width and pointY >= 0 and pointY < height) {
				if (degreeMap[pointY * width + pointX] != 9999) {
					int degreeIndex = 0;
					degreeIndex = (int)(degreeMap[pointY * width + pointX] / 20);
					histo[degreeIndex] += magniMap[pointY * width + pointX];
				}
			}
		}
	}

	for (int i = 0; i < 9; i++) {
		if (hmin > histo[i]) hmin = histo[i];
		if (hmax < histo[i]) hmax = histo[i];
	}
	// normalize histogram
	
	for (int i = 0; i < 9; i++) {
		histo[i] = (histo[i] - hmin) / (hmax - hmin);
	}
	return histo;
}
Mat showWindow(ImgHisto& img1, ImgHisto& img2)
{
	int height = 0;
	if (originalimg.rows > frame.rows) {
		height = originalimg.rows;
	}
	else height = frame.rows;
	int totalWidth = originalimg.cols + frame.cols;
	Mat comImage(height, totalWidth, originalimg.type());

	originalimg.copyTo(comImage(cv::Rect(0, 0, originalimg.cols, originalimg.rows)));
	frame.copyTo(comImage(cv::Rect(originalimg.cols, 0, frame.cols, frame.rows)));

	Point start;
	Point end;

	for (int i = 0; i < img1.cornerCount; i++) {
		start = Point(img1.location[i].second, img1.location[i].first);

		end = compareHisto(img1, img2, i, img2.cornerCount);
		if (end.x != 999999) {
			end = Point(end.x + originalimg.cols, end.y);
			line(comImage, start, end, Scalar(0, 0, 255), 1);
		}

	}
	return comImage;
	


}
cv::Point compareHisto(ImgHisto& h1, ImgHisto& h2, int h1index, int h2size)
{

	cv::Point candloc;

	int count = 0;
	float th = 1.f;
	float minsum = th;

	for (int h2index = 0; h2index < h2size; h2index++) {
		float sum = 0.0f;
		for (int k = 0; k < 9; k++) {
			count++;
			sum += sqrt((h1.histo[9 * h1index + k] - h2.histo[9 * h2index + k]) *
				(h1.histo[9 * h1index + k] - h2.histo[9 * h2index + k]));
		}
		if (minsum > sum) {
			candloc.x = h2.location[h2index].second;
			candloc.y = h2.location[h2index].first;
			minsum = sum;
		}
	}

	if (minsum == th) {
		return Point(999999, 0);
	}
	return candloc;
}
void minNorm(float* map, float max, float min, int size)
{
	for (int i = 0; i < size; i++)
	{
		map[i] = (map[i] - min) / (max - min);
	}
}