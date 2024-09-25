#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

// HOG�� ����� �̹��� �� �Լ�
double compareHOG(Mat& img1, Mat& img2) {
    // HOGDescriptor ��ü ����
    HOGDescriptor hog;

    // HOGDescriptor �⺻ ���� ���
    // �̹��� ũ�⸦ ���� (HOG�� ������ ũ���� �̹����� �۾��ϴ� ���� �� �Ϲ���)
    resize(img1, img1, Size(64, 128));
    resize(img2, img2, Size(64, 128));

    // HOG Ư¡ ���� ������ ����
    vector<float> descriptors1, descriptors2;

    // �̹������� HOG Ư¡ ���� ���
    hog.compute(img1, descriptors1);
    hog.compute(img2, descriptors2);

    // Ư¡ ������ ���絵�� ��� (��: L2 �븧 ���)
    return norm(descriptors1, descriptors2, NORM_L2);
}

int main() {
    string path = "C:/Users/woo12/OneDrive/Desktop/cpp/img/";
    // �̹����� �о���� (������� ��ȯ)
    Mat img1 = imread(path + "lecture3.bmp", IMREAD_GRAYSCALE);
    Mat img2 = imread(path + "compare1.bmp", IMREAD_GRAYSCALE);
    Mat img3 = imread(path + "compare2.bmp", IMREAD_GRAYSCALE);

    if (img1.empty() || img2.empty()) {
        cout << "�̹����� �ҷ����� ���߽��ϴ�." << endl;
        return -1;
    }

    // HOG ������� �� �̹��� ��
    double similarity = compareHOG(img1, img2);
    double sim2 = compareHOG(img1, img3);
    cout << "�� �̹����� HOG ��� ���絵: " << similarity << '\n' << sim2 << endl;

    return 0;
}
