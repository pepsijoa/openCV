#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

// HOG를 사용한 이미지 비교 함수
double compareHOG(Mat& img1, Mat& img2) {
    // HOGDescriptor 객체 생성
    HOGDescriptor hog;

    // HOGDescriptor 기본 설정 사용
    // 이미지 크기를 조정 (HOG는 고정된 크기의 이미지로 작업하는 것이 더 일반적)
    resize(img1, img1, Size(64, 128));
    resize(img2, img2, Size(64, 128));

    // HOG 특징 벡터 저장할 변수
    vector<float> descriptors1, descriptors2;

    // 이미지에서 HOG 특징 벡터 계산
    hog.compute(img1, descriptors1);
    hog.compute(img2, descriptors2);

    // 특징 벡터의 유사도를 계산 (예: L2 노름 사용)
    return norm(descriptors1, descriptors2, NORM_L2);
}

int main() {
    string path = "C:/Users/woo12/OneDrive/Desktop/cpp/img/";
    // 이미지를 읽어오기 (흑백으로 변환)
    Mat img1 = imread(path + "lecture3.bmp", IMREAD_GRAYSCALE);
    Mat img2 = imread(path + "compare1.bmp", IMREAD_GRAYSCALE);
    Mat img3 = imread(path + "compare2.bmp", IMREAD_GRAYSCALE);

    if (img1.empty() || img2.empty()) {
        cout << "이미지를 불러오지 못했습니다." << endl;
        return -1;
    }

    // HOG 기반으로 두 이미지 비교
    double similarity = compareHOG(img1, img2);
    double sim2 = compareHOG(img1, img3);
    cout << "두 이미지의 HOG 기반 유사도: " << similarity << '\n' << sim2 << endl;

    return 0;
}
