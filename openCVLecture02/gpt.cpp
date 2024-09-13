#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

// Bilinear interpolation �Լ�
Vec3b bilinearInterpolate(const Mat& img, float x, float y) {
    // x�� y ��ǥ�� ���� �κа� �Ҽ� �κ��� �и�
    int x1 = static_cast<int>(x);
    int y1 = static_cast<int>(y);
    int x2 = min(x1 + 1, img.cols - 1);  // �̹��� ������ ���� �ʵ��� ����
    int y2 = min(y1 + 1, img.rows - 1);

    float a = x - x1;
    float b = y - y1;

    // �ȼ� �� �ҷ�����
    Vec3b p1 = img.at<Vec3b>(y1, x1); // Top-left
    Vec3b p2 = img.at<Vec3b>(y1, x2); // Top-right
    Vec3b p3 = img.at<Vec3b>(y2, x1); // Bottom-left
    Vec3b p4 = img.at<Vec3b>(y2, x2); // Bottom-right

    // ���� ���� ����
    Vec3b result;
    for (int i = 0; i < 3; ++i) {  // BGR ä�� ������ ���� ����
        result[i] = static_cast<uchar>(
            (1 - a) * (1 - b) * p1[i] +
            a * (1 - b) * p2[i] +
            (1 - a) * b * p3[i] +
            a * b * p4[i]
            );
    }

    return result;
}

// Bilinear interpolation�� ������ �̹��� �������� �Լ�
Mat resizeBilinear(const Mat& img, int newWidth, int newHeight) {
    Mat result(newHeight, newWidth, img.type());

    float x_ratio = static_cast<float>(img.cols) / newWidth;
    float y_ratio = static_cast<float>(img.rows) / newHeight;

    for (int y = 0; y < newHeight; ++y) {
        for (int x = 0; x < newWidth; ++x) {
            float srcX = x * x_ratio;
            float srcY = y * y_ratio;

            // Bilinear interpolation���� ���ο� �ȼ� ���� ����
            result.at<Vec3b>(y, x) = bilinearInterpolate(img, srcX, srcY);
        }
    }

    return result;
}

int main() {
    // �̹��� �ε�
    Mat img = imread("C:/Users/woo12/OneDrive/Desktop/cpp/img/konkuk.jpg");

    // �̹��� �ε� ���� üũ
    if (img.empty()) {
        cout << "Could not load image!" << endl;
        return -1;
    }

    // ���ο� ũ��� �̹��� ��������
    int newWidth = img.cols * 0.5;  // 2��� Ȯ��
    int newHeight = img.rows * 0.5;

    Mat resizedImage = resizeBilinear(img, newWidth, newHeight);

    // ��� �̹��� ǥ��
    imshow("Original Image", img);
    imshow("Resized Image (Bilinear Interpolation)", resizedImage);

    waitKey(0);
    return 0;
}
