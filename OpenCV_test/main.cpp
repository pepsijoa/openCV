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
    // ī�޶� ��ü ����
    cv::VideoCapture cap(0); // 0�� ī�޶� ���ϴ�. �ٸ� ī�޶� ����Ϸ��� �ε����� �����մϴ�.

    // ī�޶� ���ȴ��� Ȯ��
    if (!cap.isOpened()) {
        std::cerr << "Error: ī�޶� �� �� �����ϴ�." << std::endl;
        return -1;
    }

    // â ����
    cv::namedWindow("Camera", cv::WINDOW_AUTOSIZE);

    while (true) {
        cv::Mat frame;

        // ī�޶�κ��� �������� ĸó
        cap >> frame;

        // �������� ����ִ��� Ȯ��
        if (frame.empty()) {
            std::cerr << "Error: �� �������� ĸó�Ǿ����ϴ�." << std::endl;
            break;
        }

        
        
        // ������ ǥ��
        cv::imshow("Camera", detectAruco(frame));

        // 'q' Ű�� ������ ���� ����
        if (cv::waitKey(10) == 'q') {
            break;
        }
    }

    // ī�޶� ������ �� â �ݱ�
    cap.release();
    cv::destroyAllWindows();

    return 0;
}

int main() {
    camera();
}
