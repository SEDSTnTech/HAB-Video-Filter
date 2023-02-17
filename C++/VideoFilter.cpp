#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <cmath>

using namespace std;
using namespace cv;

string printTime(double seconds);

int main() {

    time_t timer1, timer2;
    time(&timer1);

    cout << "OpenCV Version: " << CV_VERSION << endl;
    VideoCapture capture("test_drive.mp4");

    if (!capture.isOpened()) {
        cout << "Could not open file" << endl;
        return -1;
    }
    int width = capture.get(3);
    int height = capture.get(4);
    double fps = capture.get(5);
    int frame_count = capture.get(7);
    cout << "Frame Count:" << frame_count << endl;

    int totalNumPixels = width * height;

    double duration = frame_count / fps;
    float threshold = 0.4;

    cout << "*******************************************" << endl;
    printf( "Video Dimensions: %d x %d \n", width, height );
    printf( "Video Framerate: %f \n", fps );
    printf( "Video Duration: %s \n", printTime(duration).c_str());
    cout << "*******************************************" << endl;

    int codec = VideoWriter::fourcc('m', 'p', '4', 'v');
    cout << codec << endl;
    VideoWriter output("output.mp4" , codec, fps, Size(width, height));

    if (!output.isOpened()) {
        cout << "Could not open output file" << endl;
        return -1;
    }

    int cutCount = 0;
    int frameCount = 0;
    int failedFrames = 0;
    bool lastRet = false;
    int counter = 0;

    while (capture.isOpened()) {
        count++;

        Mat frame, frame_HSV, frame_threshold;
        bool isSuccess = capture.read(frame);
        capture >> frame;
        if (frame.empty()) {
            cout << "frame empty" << endl;
            break;
        }

        if (isSuccess) {
            frameCount++;

            cvtColor( frame, frame_HSV, COLOR_BGR2HSV);
            inRange(frame_HSV, Scalar(100, 50, 50), Scalar(130, 255, 255), frame_threshold);

            imshow("Original", frame);
            imshow("Masked", frame_threshold);

            double numWhitePixels = countNonZero(frame_threshold);
            double numBlackPixels = totalNumPixels - numWhitePixels;

            cout << "Num Black Pixels: " << numBlackPixels << endl;
            cout << "Num White Pixels: " << numWhitePixels << endl;
            cout << endl;

            cout << numWhitePixels / totalNumPixels << endl;
            if ((numWhitePixels / totalNumPixels) > threshold) {
                cout << "Frame " << frameCount << " Cut" << endl;

                cutCount++;
                cout << cutCount << endl;
                lastRet = true;
            }
            else {
                cout << "Frame " << frameCount << " Retained" << endl;

                output.write(frame);

                lastRet = false;
            }

        }
        if (!isSuccess) {
            cout << "Video has ended" << endl;
            break;
        }

        int key = waitKey(20);
        if (key == 'q') {
            cout << "Process ended by user" << endl;
            break;
        }

    }

    output.release();
    capture.release();
    destroyAllWindows();

    cout << "Video Cutting Complete" << endl;
    printf("%d of %d total frames (%05.2f%%) were cut \n", cutCount, frameCount, (static_cast<float>(cutCount)/frameCount)*100.0);

    time(&timer2);
    float elapsedTime = difftime(timer2, timer1);

    cout << "ELAPSED: " << printTime(elapsedTime) << endl;
    cout << "count: " << counter;

    return 0;

}

string printTime(double seconds) {
    return format("%02d:%02d:%05.2f", (static_cast<int>(seconds / 3600)), (static_cast<int>(seconds / 60)), fmod(seconds , 60.0) );
}
