#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main() {

    VideoCapture capture("test_drive.mp4");

    if (!capture.isOpened()) {
        cout << "Could not open file" << endl;
        return -1;
    }
    else {
        int fps = capture.get(5);
        int frame_count = capture.get(7);
    }

    while (capture.isOpened()) {
        Mat frame;
        bool isSuccess = capture.read(frame);

        if (isSuccess) {
            imshow("Frame:", frame);


        }
        if (!isSuccess) {
            cout << "Video has ended" << endl;
            break;
        }

        int key = waitKey(20);
        if (key == 'q') {
            cout << "Ending video" << endl;
            break;
        }

    }
    capture.release();
    destroyAllWindows();
    return 0;

}
