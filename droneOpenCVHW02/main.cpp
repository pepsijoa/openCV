#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

void blueChange(int pos, void* userdata);
void greenChange(int pos, void* userdata);
void redChange(int pos, void* userdata);
void contrastChange(int pos, void* userdata);

Mat imgOriginal;

int main(void)
{
  
    imgOriginal = imread("Lenna.png", IMREAD_COLOR);  

    namedWindow("dst", WINDOW_AUTOSIZE);
    imshow("dst", imgOriginal);

    createTrackbar("Blue", "dst", 0, 100, blueChange, nullptr);
    setTrackbarPos("Blue", "dst", 50);

    createTrackbar("Green", "dst", 0, 100, greenChange, nullptr);
    setTrackbarPos("Green", "dst", 50);

    createTrackbar("Red", "dst", 0, 100, redChange, nullptr);
    setTrackbarPos("Red", "dst", 50);

    createTrackbar("Contrast", "dst", 0, 100, contrastChange, nullptr);
    setTrackbarPos("Contrast", "dst", 50);

    waitKey(0);
    return 0;
}

void updateImage(Mat& img, int blueOffset, int greenOffset, int redOffset, float alpha) {
    
    vector<Mat> colorChannels;
    split(img, colorChannels);

    for (int y = 0; y < img.rows; y++) {
        for (int x = 0; x < img.cols; x++) {
            colorChannels[0].at<uchar>(y, x) = saturate_cast<uchar>(colorChannels[0].at<uchar>(y, x) + blueOffset);
            colorChannels[1].at<uchar>(y, x) = saturate_cast<uchar>(colorChannels[1].at<uchar>(y, x) + greenOffset);
            colorChannels[2].at<uchar>(y, x) = saturate_cast<uchar>(colorChannels[2].at<uchar>(y, x) + redOffset);
        }
    }
   
    // Apply contrast
    // TODO : saturate
    Mat adjustedImg;
    merge(colorChannels, adjustedImg);
    for (int y = 0; y < img.rows; y++) {
        for (int x = 0; x < img.cols; x++) {
            for (int i = 0; i < 3; i++) adjustedImg.at<Vec3b>(y, x)[i] = 
                saturate_cast<uchar>(adjustedImg.at<Vec3b>(y, x)[i] + (adjustedImg.at<Vec3b>(y, x)[i] - 128) * alpha);
        }
    }
    
    imshow("blue", colorChannels[0]);
    imshow("green", colorChannels[1]);
    imshow("red", colorChannels[2]);
    imshow("dst", adjustedImg);   
}

void blueChange(int pos, void* userdata) {
    int green = getTrackbarPos("Green", "dst") - 50;
    int red = getTrackbarPos("Red", "dst") - 50;
    int blue = pos - 50;
    int contrast = getTrackbarPos("Contrast", "dst");
    float alpha = (contrast - 50) / 50.0;
    updateImage(imgOriginal, blue, green, red, alpha);
}

void greenChange(int pos, void* userdata) {
    int blue = getTrackbarPos("Blue", "dst") - 50;
    int red = getTrackbarPos("Red", "dst") - 50;
    int green = pos - 50;
    int contrast = getTrackbarPos("Contrast", "dst");
    float alpha = (contrast - 50) / 50.0;
    updateImage(imgOriginal, blue, green, red, alpha);
}

void redChange(int pos, void* userdata) {
    int blue = getTrackbarPos("Blue", "dst") - 50;
    int green = getTrackbarPos("Green", "dst") - 50 ;
    int red = pos - 50;
    int contrast = getTrackbarPos("Contrast", "dst");
    float alpha = (contrast - 50) / 50.0;
    updateImage(imgOriginal, blue, green, red, alpha);
}

void contrastChange(int pos, void* userdata) {
    int blue = getTrackbarPos("Blue", "dst") - 50;
    int green = getTrackbarPos("Green", "dst") - 50;
    int red = getTrackbarPos("Red", "dst") - 50;
    float alpha = (pos - 50) / 50.0;    
    updateImage(imgOriginal, blue, green, red, alpha);
}
