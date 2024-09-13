#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

// Bilinear interpolation 함수
Vec3b bilinearInterpolate(const Mat& img, float x, float y) {
    // x와 y 좌표의 정수 부분과 소수 부분을 분리
    int x1 = static_cast<int>(x);
    int y1 = static_cast<int>(y);
    int x2 = min(x1 + 1, img.cols - 1);  // 이미지 범위를 넘지 않도록 보정
    int y2 = min(y1 + 1, img.rows - 1);

    float a = x - x1;
    float b = y - y1;

    // 픽셀 값 불러오기
    Vec3b p1 = img.at<Vec3b>(y1, x1); // Top-left
    Vec3b p2 = img.at<Vec3b>(y1, x2); // Top-right
    Vec3b p3 = img.at<Vec3b>(y2, x1); // Bottom-left
    Vec3b p4 = img.at<Vec3b>(y2, x2); // Bottom-right

    // 선형 보간 수행
    Vec3b result;
    for (int i = 0; i < 3; ++i) {  // BGR 채널 각각에 대해 보간
        result[i] = static_cast<uchar>(
            (1 - a) * (1 - b) * p1[i] +
            a * (1 - b) * p2[i] +
            (1 - a) * b * p3[i] +
            a * b * p4[i]
            );
    }

    return result;
}

// Bilinear interpolation을 적용한 이미지 리사이즈 함수
Mat resizeBilinear(const Mat& img, int newWidth, int newHeight) {
    Mat result(newHeight, newWidth, img.type());

    float x_ratio = static_cast<float>(img.cols) / newWidth;
    float y_ratio = static_cast<float>(img.rows) / newHeight;

    for (int y = 0; y < newHeight; ++y) {
        for (int x = 0; x < newWidth; ++x) {
            float srcX = x * x_ratio;
            float srcY = y * y_ratio;

            // Bilinear interpolation으로 새로운 픽셀 값을 구함
            result.at<Vec3b>(y, x) = bilinearInterpolate(img, srcX, srcY);
        }
    }

    return result;
}

int main() {
    // 이미지 로드
    Mat img = imread("C:/Users/woo12/OneDrive/Desktop/cpp/img/konkuk.jpg");

    // 이미지 로드 실패 체크
    if (img.empty()) {
        cout << "Could not load image!" << endl;
        return -1;
    }

    // 새로운 크기로 이미지 리사이즈
    int newWidth = img.cols * 0.5;  // 2배로 확대
    int newHeight = img.rows * 0.5;

    Mat resizedImage = resizeBilinear(img, newWidth, newHeight);

    // 결과 이미지 표시
    imshow("Original Image", img);
    imshow("Resized Image (Bilinear Interpolation)", resizedImage);

    waitKey(0);
    return 0;
}
