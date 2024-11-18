#include<opencv2/opencv.hpp>
#include<string>

using namespace std;
using namespace cv;

int main(void)
{
	Mat img = imread("traffic_sign.bmp", CV_LOAD_IMAGE_COLOR);
	Mat tar = imread("traffic_sign.bmp", CV_LOAD_IMAGE_GRAYSCALE);

	//170을 기준으로 이진화를 수행.
	Mat thtar;
	threshold(tar, thtar, 170, 255, THRESH_BINARY_INV);
	imwrite("threshold.bmp", thtar);

	Mat close;
	morphologyEx(thtar, close, MORPH_CLOSE, Mat(), Point(-1, -1), 1);
	imwrite("CLOSE.bmp", close);

	//이진 영상 닫기를 수행.
	Mat taropen;
	morphologyEx(close, taropen, MORPH_OPEN, Mat(), Point(-1, -1), 3);
	imwrite("OPEN.bmp", taropen);

	// salt and pepper 문제 해결을 위해 median filter를 수행함.
	Mat median = taropen;
	Mat median2;
		
	for (int i = 1; i <= 4; i++) {
		medianBlur(median, median2, 5);
		medianBlur(median2, median, 5);
	}
	imwrite("median.bmp", median);


	//내부 도형을 채우기 위해 OPEN을 수행
	morphologyEx(median, taropen, MORPH_CLOSE, Mat(), Point(-1, -1), 6);	
	imwrite("OPEN2.bmp", taropen);

	vector<vector<Point>> contours;
	findContours(taropen, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);

	for (vector<Point>& pts : contours) {

		if (contourArea(pts) < 1600 or contourArea(pts) > 5980){
			continue;
	
		}
		if (contourArea(pts) > 6000) {
			
		}
		Rect rc = boundingRect(pts);
		rectangle(img, rc, Scalar(0, 255, 0), 3);
		cout << contourArea(pts) << endl;
		
	}
	imwrite("img.bmp", img);
	return 0;
}