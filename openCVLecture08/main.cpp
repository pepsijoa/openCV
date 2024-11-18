#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/opencv.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/ml/ml.hpp>
using namespace cv;
using namespace cv::ml;
using namespace std;

vector<int> calLBP(Mat img);
void norm(int size, float* arr);
float* calhisto(int height, int width, vector<int>LBP, int blocksize);
double compare(float* tar, float* ref);
float* fcalhisto(int height, int width, vector<int>LBP, int blocksize);


VideoCapture capture(0);

Mat getrefface()
{
	CascadeClassifier cascade;
	cascade.load("C:/opencv/sources/data/lbpcascades/lbpcascade_frontalface_improved.xml");
	Mat frame;
	if (!capture.isOpened()) {
		cout << "can't open camera";
		return frame;
	}


	char keyPressed = '\0';
	while (true) {
		capture >> frame;
		Mat img = frame;



		vector<Rect> faces;
		cascade.detectMultiScale(img, faces, 1.1, 4, 0 | CV_HAAR_SCALE_IMAGE, Size(10, 10));

		Mat refFace;

		for (int y = 0; y < faces.size(); y++)
		{
			Point lb(faces[y].x + faces[y].width, faces[y].y + faces[y].height);
			Point tr(faces[y].x, faces[y].y);
			rectangle(img, lb, tr, Scalar(0, 255, 0), 3, 8, 0);


			Mat cref_face = Mat::zeros(faces[y].height, faces[y].width, CV_8UC3);
			for (int i = 0; i < faces[y].height; i++) {
				for (int j = 0; j < faces[y].width; j++) {
					cref_face.at<Vec3b>(i, j)[0] = img.at<Vec3b>(faces[y].y + i, faces[y].x + j)[0];
					cref_face.at<Vec3b>(i, j)[1] = img.at<Vec3b>(faces[y].y + i, faces[y].x + j)[1];
					cref_face.at<Vec3b>(i, j)[2] = img.at<Vec3b>(faces[y].y + i, faces[y].x + j)[2];
				}
			}

			Mat gref_face;
			cvtColor(cref_face, gref_face, COLOR_BGR2GRAY);
			Mat ref_face;
			resize(gref_face, ref_face, Size(128, 128));
			
			imshow("ref", ref_face);
			int key = waitKey(0);
			
			if (key == 'y' || key == 'Y') {
				keyPressed = 'y';
				cout << "CONFIRMED" << endl;
			}
			else {
				keyPressed = 'n';
				cout << "again" << endl;
			}
			if (keyPressed == 'y') {
				refFace = ref_face;
				return refFace;
			}
			else {
				continue;
			}
		}
		imshow("face", img);

		if (waitKey(30) >= 0) {
			destroyWindow("face");
			break;
		}
		
	}

}

void gettarface(vector<int>refLBP, vector<int>refLBP2, vector<int>refLBP3)
{
	CascadeClassifier cascade;
	cascade.load("C:/opencv/sources/data/lbpcascades/lbpcascade_frontalface_improved.xml");
	Mat frame;
	if (!capture.isOpened()) {
		cout << "can't open camera";
	}


	while (true) {
		capture >> frame;
		Mat img = frame;

		if (img.empty()) {
			continue;
		}

		vector<Rect> faces;
		cascade.detectMultiScale(img, faces, 1.1, 4, 0 | CV_HAAR_SCALE_IMAGE, Size(10, 10));

		for (int y = 0; y < faces.size(); y++)
		{
			Point lb(faces[y].x + faces[y].width, faces[y].y + faces[y].height);
			Point tr(faces[y].x, faces[y].y);
			

			//rectangle(img, lb, tr, Scalar(0, 255, 0), 3, 8, 0);


			Mat ctar_face = Mat::zeros(faces[y].height, faces[y].width, CV_8UC3);
			for (int i = 0; i < faces[y].height; i++) {
				for (int j = 0; j < faces[y].width; j++) {
					ctar_face.at<Vec3b>(i, j)[0] = img.at<Vec3b>(faces[y].y + i, faces[y].x + j)[0];
					ctar_face.at<Vec3b>(i, j)[1] = img.at<Vec3b>(faces[y].y + i, faces[y].x + j)[1];
					ctar_face.at<Vec3b>(i, j)[2] = img.at<Vec3b>(faces[y].y + i, faces[y].x + j)[2];
				}
			}

			Mat gtar_face;
			cvtColor(ctar_face, gtar_face, COLOR_BGR2GRAY);
			Mat tar_face;
			resize(gtar_face, tar_face, Size(128, 128));
			

			vector<int> tarLBP = calLBP(tar_face);
			if (tarLBP[0] == -1) continue;

			float* tarhisto = new float[15 * 15 * 256] {0};
			float* refhisto = new float[15 * 15 * 256] {0};
			float* refhisto2 = new float[15 * 15 * 256] {0};
			float* refhisto3 = new float[15 * 15 * 256] {0};

			tarhisto = fcalhisto(128, 128, tarLBP, 16);
			
			refhisto = fcalhisto(128, 128, refLBP, 16);
			refhisto2 = fcalhisto(128, 128, refLBP2, 16);
			refhisto3 = fcalhisto(128, 128, refLBP3, 16);

			double sim = compare(tarhisto, refhisto);
			double sim2 = compare(tarhisto, refhisto2);
			double sim3 = compare(tarhisto, refhisto3);

			
			float th = 40;
			if (sim > th) {
				rectangle(img, lb, tr, Scalar(0, 255, 0), 3, 8, 0);
				imwrite("correct.bmp", img);
				cout << "1" << endl;
			}
			else {
				if (sim2 > th) {
					rectangle(img, lb, tr, Scalar(0, 255, 0), 3, 8, 0);
					imwrite("correct.bmp", img);
					cout << "2" << endl;
				}
				else {
					if (sim3 > th) {
						rectangle(img, lb, tr, Scalar(0, 255, 0), 3, 8, 0);
						imwrite("correct.bmp", img);
						cout << "3" << endl;
					}
					else {
						rectangle(img, lb, tr, Scalar(0, 0, 255), 3, 8, 0);
						imwrite("wrong.bmp", img);
						waitKey(0);
						cout << sim << endl;
					}
				}
				
			}

			delete[] tarhisto;
			delete[] refhisto;
			delete[] refhisto2;
			delete[] refhisto3;
		}
		imshow("facerecog", img);
		if (waitKey(30) >= 0) break;
	}

}
void main() {

	Mat refFace = getrefface();
	Mat refFace2 = getrefface();
	Mat refFace3 = getrefface();

	imwrite("refface.bmp", refFace);
	imwrite("refface2.bmp", refFace2);
	imwrite("refface3.bmp", refFace3);
	//cout << " work" << endl;
	vector<int> refLBP = calLBP(refFace);
	vector<int> refLBP2 = calLBP(refFace2);
	vector<int> refLBP3 = calLBP(refFace3);
	
	
	gettarface(refLBP, refLBP2, refLBP3);

}

vector<int> calLBP(Mat img)
{

	int height = img.rows;
	int width = img.cols;
	vector<int> result;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			int val = 0;
			if (img.at<uchar>(i, j) < img.at<uchar>(i - 1, j)) val += 1;
			if (img.at<uchar>(i, j) < img.at<uchar>(i - 1, j+1)) val += 2;
			if (img.at<uchar>(i, j) < img.at<uchar>(i, j+1)) val += 4;
			if (img.at<uchar>(i, j) < img.at<uchar>(i + 1, j+1)) val += 8;
			if (img.at<uchar>(i, j) < img.at<uchar>(i + 1, j)) val += 16;
			if (img.at<uchar>(i, j) < img.at<uchar>(i + 1, j-1 )) val += 32;
			if (img.at<uchar>(i, j) < img.at<uchar>(i, j-1)) val += 64;
			if (img.at<uchar>(i, j) < img.at<uchar>(i - 1, j-1 )) val += 128;
			result.push_back(val);
		}
	}
	return result;
}

float* fcalhisto(int height, int width, vector<int>LBP, int blocksize)
{
	int hblocksize = blocksize / 2;

	int blockwidth = ((width - hblocksize) / hblocksize);
	int blockheight = ((height - hblocksize) / hblocksize);
	int blockcount = blockheight * blockwidth;
	int bin = 256;
	int histosize = blockcount * bin;
	float* histo = new float[histosize] {0};

	for (int i = 0; i < blockheight; i++) {
		for (int j = 0; j < blockwidth; j++) {
			for (int yy = i * hblocksize; yy <= i * hblocksize + 2 * hblocksize - 1; yy++) {
				for (int xx = j * hblocksize; xx <= j * hblocksize + 2 * hblocksize - 1; xx++) {
					int binindex = LBP[yy * width + xx];
					histo[i * blockwidth * bin + j * bin + binindex] += binindex;
				}
			}

			// normalize
			float sum = 0.0f;
			for (int k = 0; k < bin; k++) {
				sum += histo[i * blockwidth * bin + j * bin + k];
			}
			for (int k = 0; k < bin; k++) {
				histo[i * blockwidth * bin + j * bin + k] /= sqrt(sum*sum);
			}
		}
	}
	

	return histo;
}
float* calhisto(int height, int width, vector<int>LBP, int blocksize)
{
	int cellsize = blocksize / 2;
	int widthHistoSize = (width - cellsize) / cellsize;
	int heightHistoSize = (height - cellsize) / cellsize;
	int lbpsize = 256;
	int histosize = widthHistoSize * heightHistoSize * lbpsize;

	float* histo = new float[histosize] {0};
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int hIndex = 0;
			int wIndex = 0;
			int lbpindex = LBP[y * width + x];

			if (x / cellsize == 0) {
				wIndex = 0;
				if (y / cellsize == 0) {
					hIndex = 0;
					histo[hIndex * widthHistoSize * lbpsize +
						wIndex * lbpsize +
						lbpindex] += LBP[y * width + x];
				}
				else if (y / cellsize == heightHistoSize) {
					hIndex = heightHistoSize - 1;
					histo[hIndex * widthHistoSize * lbpsize +
						wIndex * lbpsize +
						lbpindex] += LBP[y * width + x];
				}
				else {
					hIndex = y / cellsize;
					histo[hIndex * widthHistoSize * lbpsize +
						wIndex * lbpsize +
						lbpindex] += LBP[y * width + x];

					histo[(hIndex - 1) * widthHistoSize * lbpsize +
						wIndex * lbpsize +
						lbpindex] += LBP[y * width + x];
				}
			}
			else if (x / cellsize == widthHistoSize) {
				wIndex = widthHistoSize - 1;
				if (y / cellsize == 0) {
					hIndex = 0;
					histo[hIndex * widthHistoSize * lbpsize +
						wIndex * lbpsize +
						lbpindex] += LBP[y * width + x];
				}
				else if (y / cellsize == heightHistoSize) {
					hIndex = heightHistoSize - 1;
					histo[hIndex * widthHistoSize * lbpsize +
						wIndex * lbpsize +
						lbpindex] += LBP[y * width + x];
				}
				else {
					hIndex = y / cellsize;
					histo[hIndex * widthHistoSize * lbpsize +
						wIndex * lbpsize +
						lbpindex] += LBP[y * width + x];
					histo[(hIndex - 1) * widthHistoSize * lbpsize +
						wIndex * lbpsize +
						lbpindex] += LBP[y * width + x];
				}
			}
			else { // width is middle
				wIndex = x / cellsize;
				if (y / cellsize == 0) {
					hIndex = 0;
					histo[hIndex * widthHistoSize * lbpsize +
						wIndex * lbpsize +
						lbpindex] += LBP[y * width + x];
					histo[hIndex * widthHistoSize * lbpsize +
						(wIndex - 1) * lbpsize +
						lbpindex] += LBP[y * width + x];
				}
				else if (y / cellsize == heightHistoSize) {
					hIndex = heightHistoSize - 1;
					histo[hIndex * widthHistoSize * lbpsize +
						wIndex * lbpsize +
						lbpindex] += LBP[y * width + x];
					histo[hIndex * widthHistoSize * lbpsize +
						(wIndex - 1) * lbpsize +
						lbpindex] += LBP[y * width + x];
				}
				else { // include 4 blocks
					hIndex = y / cellsize;
					histo[hIndex * widthHistoSize * lbpsize +
						wIndex * lbpsize +
						lbpindex] += LBP[y * width + x];
					histo[(hIndex - 1) * widthHistoSize * lbpsize +
						wIndex * lbpsize +
						lbpindex] += LBP[y * width + x];

					histo[(hIndex - 1) * widthHistoSize * lbpsize +
						(wIndex - 1) * lbpsize +
						lbpindex] += LBP[y * width + x];

					histo[hIndex * widthHistoSize * lbpsize +
						(wIndex - 1) * lbpsize +
						lbpindex] += LBP[y * width + x];
				}
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