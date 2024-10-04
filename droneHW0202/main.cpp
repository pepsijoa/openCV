#include <opencv2/opencv.hpp>
#include <iostream>
#include <mutex>

using namespace std;
using namespace cv;

void on_mouse(int event, int x, int y, int flags, void*);

Point ptOld;
Point ptNew;
Rect ROI;

Mat imgOriginal, temp;
bool isDrawing = false;

int main(void)
{
    imgOriginal = imread("Lenna.png", IMREAD_COLOR);
    temp = imgOriginal.clone();

    if (imgOriginal.empty()) {
        cout << "Error loading image" << endl;
        return -1;
    }

    namedWindow("img");
    setMouseCallback("img", on_mouse);

    imshow("img", imgOriginal);
    waitKey(0);
    return 0;
}

void on_mouse(int event, int x, int y, int flags, void*)
{
    int x1 = min(ptOld.x, ptNew.x);
    int y1 = min(ptOld.y, ptNew.y);
    int x2 = max(ptOld.x, ptNew.x);
    int y2 = max(ptOld.y, ptNew.y);

    Rect rect(Point(x1, y1), Point(x2, y2));

    Mat roi = imgOriginal(rect);
    switch (event) {
    case EVENT_LBUTTONDOWN:
        isDrawing = true;
        ptOld = Point(x, y);
        break;

    case EVENT_LBUTTONUP:
        isDrawing = false;
        ptNew = Point(x, y); 

        bitwise_not(roi, roi);

        imshow("img", imgOriginal);
        break;

    case EVENT_MOUSEMOVE:
        if (isDrawing == true) {
            temp = imgOriginal.clone();
            ptNew = Point(x, y);
            rectangle(temp, ptOld, ptNew, Scalar(0, 255, 255), 2); 
            imshow("img", temp);
        }
        break;
    }
}
