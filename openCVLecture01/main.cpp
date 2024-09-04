#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;
using namespace std;

void main()
{
	String path = "C:/Users/woo12/OneDrive/Desktop/cpp2/openCVLecture01/img/";
	String img = path + "konkuk.jpg";
	Mat imgColor = imread(img, CV_LOAD_IMAGE_COLOR);
	Mat imgGrey = imread(img, CV_LOAD_IMAGE_GRAYSCALE);
	int height, width;
	width = imgColor.cols;
	height = imgColor.rows;
	//printf("%d", imgColor.at<Vec3b>(300, 200)[0]);
	

	//imread - > color = b ,g , r
	// grey = average three color
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			
			if (x >= 100 && y >= 100) {
				float r, g, b;
				b = imgColor.at<Vec3b>(y, x)[0];
				g = imgColor.at<Vec3b>(y, x)[1];
				r = imgColor.at<Vec3b>(y, x)[2];
				
				float color = (b + g + r) / 3.0;
				imgColor.at<Vec3b>(y, x)[0] = color;
				imgColor.at<Vec3b>(y, x)[1] = color;
				imgColor.at<Vec3b>(y, x)[2] = color;
			}
		}
	}
	
	printf("w = %d, h = %d", width, height);
	imwrite("./img/red.jpg", imgColor);
	imwrite("./img/grey.jpg", imgGrey);

}

