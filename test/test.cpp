#include <opencv2/opencv.hpp>

int main() {
    // 이미지를 읽어옵니다.
    cv::Mat img1 = cv::imread("C:/Users/woo12/OneDrive/Desktop/cpp/img/desk.jpg");
    cv::Mat img2 = cv::imread("C:/Users/woo12/OneDrive/Desktop/cpp/img/desk2.jpg");

    // 두 이미지가 올바르게 로드되었는지 확인합니다.
    if (img1.empty() || img2.empty()) {
        std::cout << "이미지를 불러올 수 없습니다!" << std::endl;
        return -1;
    }

    // 두 이미지의 세로 크기를 맞춥니다.
    int height = std::max(img1.rows, img2.rows);  // 두 이미지 중 더 큰 세로 크기 선택
    int totalWidth = img1.cols + img2.cols;  // 두 이미지의 가로 크기 합산

    // 두 이미지를 붙이기 위한 새로운 이미지 생성 (높이: 최대값, 너비: 두 이미지의 합)
    cv::Mat combinedImage(height, totalWidth, img1.type());

    // 각 이미지를 결합 이미지의 좌우에 복사
    img1.copyTo(combinedImage(cv::Rect(0, 0, img1.cols, img1.rows)));  // 왼쪽에 이미지1
    img2.copyTo(combinedImage(cv::Rect(img1.cols, 0, img2.cols, img2.rows)));  // 오른쪽에 이미지2

    // 창에 결합된 이미지 표시
    cv::imshow("Combined Image", combinedImage);
    cv::waitKey(0);  // 키 입력을 기다립니다.

    return 0;
}
