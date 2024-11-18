#include<opencv2/opencv.hpp>
#include<algorithm>

using namespace cv;
using namespace std;

string path = "C:/Users/woo12/OneDrive/Desktop/cpp/img/";


vector<pair<double, double>> calgrad(Mat img);
float* calhisto(int height, int width, vector<pair<double, double>>grad, int blocksize);
void norm(int size, float* arr);
double compare(float* tar, float* ref);
void nimmax(int size, double* arr);


int main(void)

{
	Mat ref = imread(path + "face_ref.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	Mat tar = imread(path + "face_tar.bmp", CV_LOAD_IMAGE_GRAYSCALE);

	Mat colorrest = imread(path + "face_tar.bmp", CV_LOAD_IMAGE_COLOR);
	
	
	vector<pair<double, double>> refGrad = calgrad(ref);
	
	float* refhisto = calhisto(ref.rows, ref.cols, refGrad, 12);
	
	vector<pair<double, double>> tarGrad = calgrad(tar);

	int winYhalf = ref.rows / 2;
	int winXhalf = ref.cols / 2;


	Mat ntar = Mat::zeros(ref.rows, ref.cols, CV_8UC1);
	Mat result = Mat::zeros(tar.rows, tar.cols, CV_8UC1);

	double* comres = new double[tar.rows * tar.cols] {0};

	float* tarhisto = new float[25] {0};


	float comparemax = 0.0f;
	
	for (int y = 0; y < tar.rows; y++) {
		for (int x = 0; x < tar.cols; x++) {
			vector<pair<double, double>> winGrad(ref.rows * ref.cols);
			int count = 0;
			int nx = 0;
			int ny = 0;
			
			for (int winy = y - (winYhalf-1); winy <= y + winYhalf; winy++) {
				for (int winx = x - (winXhalf-1); winx <= x + winXhalf; winx++) {
					if (winx >= 0 and winx < tar.cols and winy >= 0 and winy < tar.rows) {
						winGrad[ny * ref.cols + nx] = tarGrad[winy * tar.cols + winx];
						nx++;
						count++;
					}
					else {
						winGrad[ny * ref.cols + nx].first = 0;
						winGrad[ny * ref.cols + nx].second = 0;
						nx++;
					}
					
				}
				nx = 0;
				ny++;
			}
			//cout << "count = " << count << endl;
			//windowed
			tarhisto = calhisto(ref.rows, ref.cols, winGrad, 12);
			comres[y * tar.cols + x] = compare(tarhisto, refhisto);
			
			if (comparemax < comres[y * tar.cols + x]) {
				comparemax = comres[y * tar.cols + x];
			}
			
			
		}
	}

	for (int y = 0; y < tar.rows; y++) {
		for (int x = 0; x < tar.cols; x++) {
			if (comres[y * tar.cols + x] > comparemax * 0.7) {
				circle(colorrest, Point(x, y), 1, Scalar(0, 0, 255));
			}
		}
	}

	nimmax(tar.cols * tar.rows, comres);

	for (int i = 0; i < tar.rows; i++) {
		for (int j = 0; j < tar.cols; j++) {
			result.at<uchar>(i, j) = saturate_cast<uchar>(comres[i * tar.cols + j] * 255.0);
		}
	}


	imwrite(path + "result.bmp", colorrest);
	imwrite(path + "result1.bmp", result);
	cv::waitKey(0);

	delete[] comres;
	delete[] tarhisto;
	delete[] refhisto;



}

float* calhisto(int height, int width, vector<pair<double, double>>grad, int blocksize)
{
	int hblocksize = blocksize / 2;

	int blockwidth = ((width - hblocksize) / hblocksize);
	int blockheight = ((height - hblocksize) / hblocksize);
	int blockcount = blockheight * blockwidth;
	int bin = 9;
	int histosize = blockcount * bin;
	float* histo = new float[histosize] {0};

	int histoindex = 0;
	for (int i = 0; i < blockheight; i++) {
		for (int j = 0; j < blockwidth; j++) {
			for (int yy = i * hblocksize; yy <= i * hblocksize + 2 * hblocksize - 1; yy++) {
				for (int xx = j * hblocksize; xx <= j * hblocksize + 2 * hblocksize - 1; xx++) {
					int degreebin = grad[yy * width + xx].second / 20.0;
					histo[i * blockwidth * bin + j * bin + degreebin] += grad[yy * width + xx].first;
				}
			}
			// normalize
			float sum = 0.0f;
			for (int k = 0; k < bin; k++) {
				sum += histo[i * blockwidth * bin + j * bin + k];
			}
			for (int k = 0; k < bin; k++) {
				histo[i * blockwidth * bin + j * bin + k] /= sqrt(sum * sum);
			}
		}
	}

	norm(histosize, histo);
	return histo;

}

void norm(int size, float* arr)
{
	double sqrsum = 0;
	for (int i = 0; i < size; i++)
		sqrsum += arr[i] * arr[i];
	double denorm = sqrt(sqrsum);
	for (int i = 0; i < size; i++)
		arr[i] /= denorm;
}
void nimmax(int size, double* arr) {
	double max = 0.0;
	double min = 999999;
	for (int i = 0; i < size; i++) {
		if (arr[i] > max) max = arr[i];
		if (arr[i] < min) min = arr[i];
	}
	for (int i = 0; i < size; i++) {
		arr[i] = (arr[i] - min) / (max - min);
	}
}
double compare(float* tar, float* ref)
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
	return exp(5 * (nomi / denomi));
}

vector<pair<double, double>> calgrad(Mat img)
{

	int height = img.rows;
	int width = img.cols;

	int ymask[9]{ -1, -2, -1, 0, 0, 0, 1, 2, 1 };
	int xmask[9]{ -1, 0, 1, -2, 0, 2, -1, 0, 1 };

	int x, y, xx, yy;
	double conv_x, conv_y;


	vector<pair<double, double>> grad;
	double* allmagni = new double[height * width] {0};
	double* alldegree = new double[height * width] {0};

	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {

			conv_x = 0;
			conv_y = 0;

			for (yy = y - 1; yy <= y + 1; yy++) {
				for (xx = x - 1; xx <= x + 1; xx++) {
					if (xx >= 0 and xx < width && yy >= 0 and yy < height) {
						conv_x += img.at<uchar>(yy, xx) * ymask[(yy - y + 1) * 3 + (xx - x + 1)];
						conv_y += img.at<uchar>(yy, xx) * xmask[(yy - y + 1) * 3 + (xx - x + 1)];
					}
				}
			}
			conv_x /= 9.0;
			conv_y /= 9.0;



			//calculate magni
			double magni = sqrt(conv_x * conv_x + conv_y * conv_y);
			double degree = atan2(conv_y, conv_x) * 180.0 / CV_PI;
			if (degree < 0) degree += 180.0;
			degree -= 1;

			allmagni[y * width + x] = magni;
			alldegree[y * width + x] = degree;

		}

	}

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			grad.push_back({ allmagni[y * width + x], alldegree[y * width + x] });
		}
	}

	delete[] allmagni;
	delete[] alldegree;
	return grad;
}



