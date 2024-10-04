#include<opencv2/opencv.hpp>
#include <string>

using namespace std;
using namespace cv;

void cornerDetection(Mat img);
void minNorm(float* map, float max, float min, int size);
Mat lowPassFilter(Mat img);

string path = "C:/Users/woo12/OneDrive/Desktop/cpp/img/";

int main(void)
{
	Mat img = imread(path + "desk.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	cornerDetection(img);

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
void cornerDetection(Mat img)
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
			}

			R[y * width + x] = r;
			if (maxR < r) maxR = r;
			if (minR > r) minR = r;

		}
	}
	
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
	cv::imwrite(path + "1cornermap.bmp", edgeimg);
	
	delete[] degreeSize;
	delete[] magniSize;
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
	}
}