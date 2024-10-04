#include<opencv2/opencv.hpp>
#include <string>

using namespace std;
using namespace cv;

struct ImgHisto {
	vector<pair<int, int>> location;
	vector<float> histo;
	size_t cornerCount = 0;

	ImgHisto(size_t cornercounter)
		:location(cornercounter), histo(cornercounter * 9), cornerCount(cornercounter) {
	}
};

void cornerDetection(Mat img, ImgHisto& allhisto);
void minNorm(float* map, float max, float min, int size);
Mat lowPassFilter(Mat img);
float* centerHisto(int centerX, int centerY, float* degreeMap, float* magniMap, int height, int width);
pair<int, int> compareHisto(ImgHisto& h1, ImgHisto& h2, int h1index, int h2size);


string path = "C:/Users/woo12/OneDrive/Desktop/cpp/img/";




int main(void)
{


	Mat img = imread(path + "desk.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	ImgHisto input(img.rows * img.cols);
	cornerDetection(img, input);
	
	
	Mat compareimg = imread(path + "desk2.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	ImgHisto compare(compareimg.rows * compareimg.cols);
	cornerDetection(compareimg, compare);

	

	Mat edgeimg = Mat::zeros(compareimg.rows, compareimg.cols, CV_8UC3);
	for (int y = 0; y < compareimg.rows; y++) {
		for (int x = 0; x < compareimg.cols; x++) {
			for (int i = 0; i < 3; i++) {
				edgeimg.at<Vec3b>(y, x)[i] = compareimg.at<uchar>(y, x);
			}
		}
	}

	Point pcenter;
	Scalar color(0, 255, 0);

	vector<pair<int, int>> candidate;
	
	for (int i = 0; i < input.cornerCount; i++)
	{
		candidate.push_back(compareHisto(input, compare, i, compare.cornerCount));
		pcenter.x = candidate[i].second;
		pcenter.y = candidate[i].first;
		circle(edgeimg, pcenter, 10, color, 1, 8, 0);
	}
	cv::imwrite(path + "1cornermap.bmp", edgeimg);
}

Mat lowPassFilter(Mat img)
{
	Mat output = img.clone();
	for (int y = 1; y < img.rows-1; y++) {
		for (int x = 1; x < img.cols-1; x++) {

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
void cornerDetection(Mat img, ImgHisto& allhisto)
{	
	img = lowPassFilter(img);

	int xmask[9]{ -1, -1, -1, 0, 0, 0, 1, 1, 1 };
	int ymask[9]{ -1, 0, 1, -1, 0, 1, -1, 0, 1 };

	int height = img.rows;
	int width = img.cols;

	float* ix = new float[height * width];
	float* iy = new float[height * width];

	float* magniSize = new float[height * width];
	float* degreeSize = new float[height * width];

	float ixmax = 0.0f;
	float ixmin = 0.0f;

	float iymax = 0.0f;
	float iymin = 0.0f;


	// low pass fiter

	// copy gray img to color img.
	Mat edgeimg = Mat::zeros(height, width, CV_8UC3);
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			for (int i = 0; i < 3; i++) {
				edgeimg.at<Vec3b>(y, x)[i] = img.at<uchar>(y, x);
			}
		}
	}
	
	float magniMax = 0.0f;
	float magniMin = 0.0f;

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
			if (magniMax < magni) magniMax = magni;
			if (magniMin > magni) magniMin = magni;
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

	//norm magnitude
	minNorm(magniSize, magniMax, magniMin, height * width);




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

	//cout << "ÄÚ³Ê ÁÂÇ¥ ¼ö : " << ceninput << endl;
	//Norm for visualization
	//minNorm(R, maxR, minR, height * width);

	// spot corner visualization
	Point pcenter;
	Scalar color(0, 255, 0);
	for (int y = 1; y < height - 1; y++) {
		for (int x = 1; x < width - 1; x++) {
			if (corner[y * width + x] == 1) {
				pcenter.x = x;
				pcenter.y = y;
				circle(edgeimg, pcenter, 1, color, 1, 8, 0);
			}
			
			/*edgeimg.at<uchar>(y, x) = R[y * width + x];*/
		}
	}

	//cv::imwrite(path + to_string(int(th)) + "withcircle.bmp", img);
	cv::imshow("dst", edgeimg);
	waitKey(0);
	//waitKey(0);
	delete[] degreeSize;
	delete[] magniSize;
	delete[] corner;
	delete[] R;
}

float* centerHisto(int centerX, int centerY, float* degreeMap, float* magniMap, int height, int width)
{
	float* histo = new float[9] {0};

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

	// normalize histogram
	float sum = 0.0f;
	for (int i = 0; i < 9; i++) sum += histo[i];
	sum = sqrt(sum);
	for (int i = 0; i < 9; i++) histo[i] = histo[i] / sum;
	return histo;
}

pair<int,int> compareHisto(ImgHisto& h1, ImgHisto& h2, int h1index, int h2size)
{
	
	pair<int, int> candloc;
	
	int count = 0;
	float minsum = 40000.0f;
	
	for (int h2index = 0; h2index  < h2size; h2index++) {
		float sum = 0.0f;
		for (int k = 0; k < 9; k++) {
			count++;
			sum += sqrt((h1.histo[9 * h1index + k] - h2.histo[9 * h2index + k]) *
				(h1.histo[9 * h1index + k] - h2.histo[9 * h2index + k]));
		}
		if (minsum > sum) {
			candloc.first = h2.location[h2index].first;
			candloc.second = h2.location[h2index].second;
			minsum = sum;
		}
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