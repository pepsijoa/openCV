#include<opencv2/opencv.hpp>
#include<iostream>

using namespace std;

string path = "C:/Users/woo12/OneDrive/Desktop/cpp/img/";
int main(void)
{
    // 비디오 파일 열기 (또는 웹캠을 사용하려면 0을 입력)
    cv::VideoCapture cap(path + "desk.mp4"); // 파일 경로 또는 장치 ID 사용

    // 비디오 파일이 열렸는지 확인
    if (!cap.isOpened()) {
        std::cout << "Error: Could not open the video file!" << std::endl;
        return -1;
    }

    // 프레임별로 비디오 처리
    cv::Mat frame;
    int frame_count = 0;

    while (true) {
        // 비디오에서 한 프레임 읽기
        bool success = cap.read(frame);

        // 더 이상 프레임이 없으면 루프 종료
        if (!success) {
            std::cout << "End of video stream" << std::endl;
            break;
        }

        // 프레임 처리 (예: 화면에 표시)
        cv::imshow("Video Frame", frame);

        // 프레임 번호 출력
        std::cout << "Processing frame: " << frame_count << std::endl;
        frame_count++;

        // 'q' 키를 누르면 중단
        if (cv::waitKey(30) == 'q') {
            break;
        }
    }

    // 리소스 해제
    cap.release();
    cv::destroyAllWindows();
}