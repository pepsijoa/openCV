#include<opencv2/opencv.hpp>
#include <string>

using namespace std;
using namespace cv;

void cornerDetection(Mat img);
void minNorm(float* map, float max, float min, int size);

string path = "C:/Users/woo12/OneDrive/Desktop/cpp/img/";



int main(void)
{
	Mat img = imread(path + "apart.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	cornerDetection(img);
}

void cornerDetection(Mat img)
{	
	int xmask[9]{ -1, -1, -1, 0, 0, 0, 1, 1, 1 };
	int ymask[9]{ -1, 0, 1, -1, 0, 1, -1, 0, 1 };
	int height = img.rows;
	int width = img.cols;

	float* ix = new float[height * width];
	float* iy = new float[height * width];

	
	

	float ixmax = 0.0f;
	float ixmin = 0.0f;

	float iymax = 0.0f;
	float iymin = 0.0f;

	Mat edgeimg = Mat::zeros(height, width, CV_8UC1);
	
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

			ix[y * width + x] = fx;
			iy[y * width + x] = fy;
		}
		
	}

	float* R = new float[height * width];
	int* corner = new int[height * width];

	float k = 0.04f;
	float maxR = 0.0f;
	float minR = 0.0f;
	
	float th = 15.0f;

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
			}

			R[y * width + x] = r;
			if (maxR < r) maxR = r;
			if (minR > r) minR = r;

		}
	}
	
	minNorm(R, maxR, minR, height * width);
	


	Point pcenter;
	Scalar color(0, 255, 0);
	for (int y = 1; y < height - 1; y++) {
		for (int x = 1; x < width - 1; x++) {
			if (corner[y * width + x] == 1) {
				pcenter.x = x;
				pcenter.y = y;
				circle(img, pcenter, 1, color, 2, 8, 0);
			}
			
			edgeimg.at<uchar>(y, x) = R[y * width + x];
		}
	}

	cv::imwrite(path + to_string(int(th)) + "withcircle.bmp", img);
	//cv::imwrite(path + "cornermap.bmp", edgeimg);

	delete[] corner;
	delete[] ix;
	delete[] iy;
	delete[] R;
}

void minNorm(float* map, float max, float min, int size)
{
	for (int i = 0; i < size; i++)
	{
		map[i] = (map[i] - min) / (max - min);
		map[i] = 255 * map[i];
	}
}