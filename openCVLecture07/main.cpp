#include<opencv2/opencv.hpp>
using namespace cv;
using namespace std;

string path = "C:/Users/woo12/OneDrive/Desktop/cpp/img/";

float sumNorm(float* arr, int size);
float* calHisto(Mat img);
float compare(float* tar, float* ref);
void tarHisto(Mat img, float* orihisto);
float* onlyHisto(int height, int width, float* magniSize, float* degreeSize);

int main(void)
{
	Mat ref = imread(path + "face_ref.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	Mat tar = imread(path + "face_tar.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	
	float* refhisto = calHisto(ref);
	tarHisto(tar, refhisto);
	delete[] refhisto;
}


float compare(float* tar, float* ref)
{
	float nomi = 0;
	float denomi = 0;
	float refMag = 0;
	float tarMag = 0;

	for (int k = 0; k < 225; k++) {
		nomi += ref[k] * tar[k];
		refMag += ref[k] * ref[k];
		tarMag += tar[k] * tar[k];
	}
	denomi = sqrt(refMag) * sqrt(tarMag);
	return exp (5* (nomi / denomi));
}

void tarHisto(Mat img, float* orihisto)
{
	int height = img.rows;
	int width = img.cols;

	Mat result = Mat::zeros(img.rows, img.cols, CV_8UC1);

	int xmask[9]{ -1, -1, -1, 0, 0, 0, 1, 1, 1 };
	int ymask[9]{ -1, 0, 1, -1, 0, 1, -1, 0, 1 };

	float* magniSize = new float[height * width] {0};
	float* degreeSize = new float[height * width] {0};

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			float fx = 0.0f;
			float fy = 0.0f;

			float magni = 0.0f;
			float degree = 0.0f;

			for (int my = 0; my <= 2; my++) {
				for (int mx = 0; mx <= 2; mx++) {
					if (y + (my - 1) >= 0 and y + (my - 1) < height and x + (mx - 1) >= 0 and x + (mx - 1) < width) {
						fx += img.at<uchar>(y + (my - 1), x + (mx - 1)) * xmask[my * 3 + mx] / 255.0;
						fy += img.at<uchar>(y + (my - 1), x + (mx - 1)) * ymask[my * 3 + mx] / 255.0;
					}

				}
			}

			//calculate magni
			magni = sqrt(fx * fx + fy * fy);
			magniSize[y * width + x] = magni;

			//calgulate degree
			degree = atan2(fy, fx) * 180.0 / 3.1415926535;
			if (degree < 0) degree += 180.0;
			degreeSize[y * width + x] = degree - 1;
		}

	}

	float* newhisto = new float[225] {0};
	float* newdegree = new float[36 * 36] {0};
	float* newmagni = new float[36 * 36] {0};
	float* value = new float[height * width] {0};
	float max = 0;
	float min = 999;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			
			for (int ny = 0; ny < 36; ny++) {
				for (int nx = 0; nx < 36; nx++) {
					for (int mx = -17; mx <= 18; mx++) {
						for (int my = -17; my <= 18; my++) {
							if (y + my >= 0 and y + my < height and x + mx >= 0 and x + mx < width) {
								
								int lx = x + mx;
								int ly = y + my;

								newdegree[ny * 36 + nx] = degreeSize[ ly * width + lx ];
								newmagni[ny * 36 + nx] = magniSize[ly * width + lx];

							}
						}
					}

				}
			}
			//cout << count << endl;
			newhisto = onlyHisto(36, 36, newmagni, newdegree);
			//compare
			value[y*width+x] = (compare(newhisto, orihisto));
			if (max > value[y * width + x]) max = value[y * width + x];
			if (min < value[y * width + x]) min = value[y * width + x];

		}
	}
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			value[y * width + x] = value[y * width + x] - min / (max - min);
			result.at<uchar>(y, x) = value[y * width + x] * 255;
		}
	}
	
	cv::imshow("resul", result);
	cv::waitKey(0);
	
	delete[] newhisto;
	delete[] newdegree;
	delete[] newmagni;
	delete[] magniSize;
	delete[] degreeSize;
}

float* onlyHisto(int height, int width, float* magniSize, float* degreeSize)
{
	int degreebin = 9;
	int heightHistoSize = height / 6 - 1;
	int widthHistoSize = width / 6 - 1;
	int histoSize = heightHistoSize * widthHistoSize * degreebin;

	float* histo = new float[histoSize] {0};

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int hIndex = 0;
			int wIndex = 0;
			int degreeIndex = 0;
			degreeIndex = (int)(degreeSize[y * width + x] / 20);
			if (degreeIndex < 0 or degreeIndex >8) continue;

			if (x / 6 == 0) {
				wIndex = 0;
				if (y / 6 == 0) {
					hIndex = 0;
					histo[hIndex * widthHistoSize * degreebin +
						wIndex * degreebin +
						degreeIndex] += magniSize[y * width + x];
				}
				else if (y / 6 == heightHistoSize) {
					hIndex = heightHistoSize - 1;
					histo[hIndex * widthHistoSize * degreebin +
						wIndex * degreebin +
						degreeIndex] += magniSize[y * width + x];
				}
				else {
					hIndex = y / 6;
					histo[hIndex * widthHistoSize * degreebin +
						wIndex * degreebin +
						degreeIndex] += magniSize[y * width + x];
					histo[(hIndex - 1) * widthHistoSize * degreebin +
						wIndex * degreebin +
						degreeIndex] += magniSize[y * width + x];
				}
			}
			else if (x / 6 == widthHistoSize) {
				wIndex = widthHistoSize - 1;
				if (y / 6 == 0) {
					hIndex = 0;
					histo[hIndex * widthHistoSize * degreebin +
						wIndex * degreebin +
						degreeIndex] += magniSize[y * width + x];
				}
				else if (y / 6 == heightHistoSize) {
					hIndex = heightHistoSize - 1;
					histo[hIndex * widthHistoSize * degreebin +
						wIndex * degreebin +
						degreeIndex] += magniSize[y * width + x];
				}
				else {
					hIndex = y / 6;
					histo[hIndex * widthHistoSize * degreebin +
						wIndex * degreebin +
						degreeIndex] += magniSize[y * width + x];
					histo[(hIndex - 1) * widthHistoSize * degreebin +
						wIndex * degreebin +
						degreeIndex] += magniSize[y * width + x];
				}
			}
			else { // width is middle
				wIndex = x / 6;
				if (y / 6 == 0) {
					hIndex = 0;
					histo[hIndex * widthHistoSize * degreebin +
						wIndex * degreebin +
						degreeIndex] += magniSize[y * width + x];
					histo[hIndex * widthHistoSize * degreebin +
						(wIndex - 1) * degreebin +
						degreeIndex] += magniSize[y * width + x];
				}
				else if (y / 6 == heightHistoSize) {
					hIndex = heightHistoSize - 1;
					histo[hIndex * widthHistoSize * degreebin +
						wIndex * degreebin +
						degreeIndex] += magniSize[y * width + x];
					histo[hIndex * widthHistoSize * degreebin +
						(wIndex - 1) * degreebin +
						degreeIndex] += magniSize[y * width + x];
				}
				else { // include 4 blocks
					hIndex = y / 6;
					histo[hIndex * widthHistoSize * degreebin +
						wIndex * degreebin +
						degreeIndex] += magniSize[y * width + x];
					histo[(hIndex - 1) * widthHistoSize * degreebin +
						wIndex * degreebin +
						degreeIndex] += magniSize[y * width + x];
					histo[(hIndex - 1) * widthHistoSize * degreebin +
						(wIndex - 1) * degreebin +
						degreeIndex] += magniSize[y * width + x];
					histo[hIndex * widthHistoSize * degreebin +
						(wIndex - 1) * degreebin +
						degreeIndex] += magniSize[y * width + x];
				}
			}
		}
	}

	float sum = sumNorm(histo, histoSize);
	
	for (int i = 0; i < histoSize; i++) {
		histo[i] = histo[i] / sum;
	}
	return histo;
}
float* calHisto(Mat img)
{
	int height = img.rows;
	int width = img.cols;

	int xmask[9]{ -1, -1, -1, 0, 0, 0, 1, 1, 1 };
	int ymask[9]{ -1, 0, 1, -1, 0, 1, -1, 0, 1 };

	float* magniSize = new float[height * width] {0};
	float* degreeSize = new float[height * width] {0};

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			float fx = 0.0f;
			float fy = 0.0f;

			float magni = 0.0f;
			float degree = 0.0f;

			for (int my = 0; my <= 2; my++) {
				for (int mx = 0; mx <= 2; mx++) {
					if (y + (my - 1) >= 0 and y + (my - 1) < height and x + (mx - 1) >= 0 and x + (mx - 1) < width) {
						fx += img.at<uchar>(y + (my - 1), x + (mx - 1)) * xmask[my * 3 + mx] / 255.0;
						fy += img.at<uchar>(y + (my - 1), x + (mx - 1)) * ymask[my * 3 + mx] / 255.0;
					}

				}
			}

			//calculate magni
			magni = sqrt(fx * fx + fy * fy);
			magniSize[y * width + x] = magni;

			//calgulate degree
			degree = atan2(fy, fx) * 180.0 / 3.1415926535;
			if (degree < 0) degree += 180.0;
			degreeSize[y * width + x] = degree - 1;
		}

	}

	
	int degreebin = 9;
	int heightHistoSize = height / 6 - 1;
	int widthHistoSize = width / 6 - 1;
	int histoSize = heightHistoSize * widthHistoSize * degreebin;
	
	float* histo = new float[histoSize] {0};

	for (int y = 0; y < height; y++) { 
		for (int x = 0; x < width; x++) {
				int hIndex = 0;
				int wIndex = 0;
				int degreeIndex = 0;
				degreeIndex = (int)(degreeSize[y * width + x] / 20);
				if (degreeIndex < 0 or degreeIndex >8) continue;
			
				if (x / 6 == 0) {
					wIndex = 0;
					if (y / 6 == 0) {
						hIndex = 0;
						histo[hIndex * widthHistoSize * degreebin +
							wIndex * degreebin +
							degreeIndex] += magniSize[y * width + x];
					}
					else if (y / 6 == heightHistoSize) {
						hIndex = heightHistoSize - 1;
						histo[hIndex * widthHistoSize * degreebin +
							wIndex * degreebin +
							degreeIndex] += magniSize[y * width + x];
					}
					else {
						hIndex = y / 6;
						histo[hIndex * widthHistoSize * degreebin +
							wIndex * degreebin +
							degreeIndex] += magniSize[y * width + x];
						histo[(hIndex - 1) * widthHistoSize * degreebin +
							wIndex * degreebin +
							degreeIndex] += magniSize[y * width + x];
					}
				}
				else if (x / 6 == widthHistoSize) {
					wIndex = widthHistoSize - 1;
					if (y / 6 == 0) {
						hIndex = 0;
						histo[hIndex * widthHistoSize * degreebin +
							wIndex * degreebin +
							degreeIndex] += magniSize[y * width + x];
					}
					else if (y / 6 == heightHistoSize) {
						hIndex = heightHistoSize - 1;
						histo[hIndex * widthHistoSize * degreebin +
							wIndex * degreebin +
							degreeIndex] += magniSize[y * width + x];
					}
					else {
						hIndex = y / 6;
						histo[hIndex * widthHistoSize * degreebin +
							wIndex * degreebin +
							degreeIndex] += magniSize[y * width + x];
						histo[(hIndex - 1) * widthHistoSize * degreebin +
							wIndex * degreebin +
							degreeIndex] += magniSize[y * width + x];
					}
				}
				else { // width is middle
					wIndex = x / 6;
					if (y / 6 == 0) {
						hIndex = 0;
						histo[hIndex * widthHistoSize * degreebin +
							wIndex * degreebin +
							degreeIndex] += magniSize[y * width + x];
						histo[hIndex * widthHistoSize * degreebin +
							(wIndex - 1) * degreebin +
							degreeIndex] += magniSize[y * width + x];
					}
					else if (y / 6 == heightHistoSize) {
						hIndex = heightHistoSize - 1;
						histo[hIndex * widthHistoSize * degreebin +
							wIndex * degreebin +
							degreeIndex] += magniSize[y * width + x];
						histo[hIndex * widthHistoSize * degreebin +
							(wIndex - 1) * degreebin +
							degreeIndex] += magniSize[y * width + x];
					}
					else { // include 4 blocks
						hIndex = y / 6;
						histo[hIndex * widthHistoSize * degreebin +
							wIndex * degreebin +
							degreeIndex] += magniSize[y * width + x];
						histo[(hIndex - 1) * widthHistoSize * degreebin +
							wIndex * degreebin +
							degreeIndex] += magniSize[y * width + x];
						histo[(hIndex - 1) * widthHistoSize * degreebin +
							(wIndex - 1) * degreebin +
							degreeIndex] += magniSize[y * width + x];
						histo[hIndex * widthHistoSize * degreebin +
							(wIndex - 1) * degreebin +
							degreeIndex] += magniSize[y * width + x];
					}
				}
		}
	}
	float sum = sumNorm(histo, histoSize);
	for (int i = 0; i < histoSize; i++) {
		histo[i] = histo[i] / sum;
		//cout << histo[i] << endl;
	}
	delete[] magniSize;
	delete[] degreeSize;
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