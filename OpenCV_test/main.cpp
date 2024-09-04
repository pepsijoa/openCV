#include <opencv2/opencv.hpp>
#include <opencv2/objdetect/aruco_detector.hpp>

#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

int arucosize = aruco::DICT_5X5_100;

void createAruco(int amount, int size)
{
	Mat markerImage;
	aruco::Dictionary dictionary = aruco::getPredefinedDictionary(arucosize);
	
	for (int i = 0; i <= amount; i++)
	{
		aruco::generateImageMarker(dictionary, i, size, markerImage, 1);
		string imgname = to_string(size)+ "_" + to_string(i) + ".png";
		// cout << imgname << endl;
		imwrite(imgname, markerImage);
	}
}

Mat detectAruco(Mat inputImage)
{
	vector<int> markerIds;
	vector<vector<Point2f>> markerCorners, rejectedCandidates;

	aruco::DetectorParameters detectorParams = aruco::DetectorParameters();

	aruco::Dictionary dictionary = aruco::getPredefinedDictionary(arucosize);

	aruco::ArucoDetector detector(dictionary, detectorParams);

	detector.detectMarkers(inputImage, markerCorners, markerIds, rejectedCandidates);
    if (!markerIds.empty()) {
        aruco::drawDetectedMarkers(inputImage, markerCorners, markerIds);
        cout << "check";
    }
    
    return inputImage;
}

int camera()
{
    // 카메라 객체 생성
    cv::VideoCapture cap(0); // 0번 카메라를 엽니다. 다른 카메라를 사용하려면 인덱스를 변경합니다.

    // 카메라가 열렸는지 확인
    if (!cap.isOpened()) {
        std::cerr << "Error: 카메라를 열 수 없습니다." << std::endl;
        return -1;
    }

    // 창 생성
    cv::namedWindow("Camera", cv::WINDOW_AUTOSIZE);

    while (true) {
        cv::Mat frame;

        // 카메라로부터 프레임을 캡처
        cap >> frame;

        // 프레임이 비어있는지 확인
        if (frame.empty()) {
            std::cerr << "Error: 빈 프레임이 캡처되었습니다." << std::endl;
            break;
        }

        
        
        // 프레임 표시
        cv::imshow("Camera", detectAruco(frame));

        // 'q' 키를 누르면 루프 종료
        if (cv::waitKey(10) == 'q') {
            break;
        }
    }

    // 카메라 릴리즈 및 창 닫기
    cap.release();
    cv::destroyAllWindows();

    return 0;
}

int main() {
    camera();
}
