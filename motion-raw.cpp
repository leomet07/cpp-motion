#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <cstdlib>

using namespace cv;
using namespace std;


int main()
{

    try {
        cout << "OPENCV version: " << CV_VERSION << endl;
        Mat frame;
        //--- INITIALIZE VIDEOCAPTURE
        VideoCapture cap;
        // open the default camera using default API
        // cap.open(0);
        // OR advance usage: select any API backend
        int deviceID = 0;             // 0 = open default camera
        int apiID = cv::CAP_ANY;      // 0 = autodetect default API
        // open selected camera using selected API
        // cap.open(deviceID);
        cap.open(0);
        // check if we succeeded
        if (!cap.isOpened()) {
            cerr << "ERROR! Unable to open camera\n";
            return -1;
        }
        //--- GRAB AND WRITE LOOP
        cout << "Start grabbing" << endl
            << "Press any key to terminate" << endl;
        double frame_id = 0;
        Mat LastFrame;
        for (;;)
        {
            // wait for a new frame from camera and store it into 'frame'
            cap.read(frame);
            // check if we succeeded
            if (frame.empty()) {
                cerr << "ERROR! blank frame grabbed\n";
                break;
            }

            if (frame_id == 0) {
                LastFrame = frame;
                frame_id++;
                continue;
            }
            frame_id++;
            // cv::resize(frame, frame, { 1280, 720 });
            int width = frame.size().width;
            int height = frame.size().height;
            // cout << "Width : " << width << endl;
            // cout << "Height: " << height << endl;




            double totalrdiff = 0;
            double totalgdiff = 0;
            double totalbdiff = 0;
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {

                    uchar bC = frame.data[frame.channels() * (frame.cols * y + x) + 0];
                    uchar gC = frame.data[frame.channels() * (frame.cols * y + x) + 1];
                    uchar rC = frame.data[frame.channels() * (frame.cols * y + x) + 2];

                    uchar bP = LastFrame.data[LastFrame.channels() * (LastFrame.cols * y + x) + 0];
                    uchar gP = LastFrame.data[LastFrame.channels() * (LastFrame.cols * y + x) + 1];
                    uchar rP = LastFrame.data[LastFrame.channels() * (LastFrame.cols * y + x) + 2];


                    /*
                    Point3_<uchar>* C = frame.ptr<Point3_<uchar> >(y, x);

                    Point3_<uchar>* P = LastFrame.ptr<Point3_<uchar> >(y, x);

                    int rC = C->z; //R
                    int gC = C->y; //G
                    int bC = C->x; //B



                    int rP = P->z; //R
                    int gP = P->y; //G
                    int bP = P->x; //B
                    */

                    float rDiff = (float)abs(rC - rP) / (float)255;
                    float gDiff = (float)abs(gC - gP) / (float)255;
                    float bDiff = (float)abs(bC - bP) / (float)255;

                    // cout << rDiff << gDiff << bDiff << endl;
                    totalrdiff += rDiff;
                    totalgdiff += gDiff;
                    totalbdiff += bDiff;



                }
            }
            long total_pixels = width * height;
            totalrdiff /= total_pixels;
            totalgdiff /= total_pixels;
            totalbdiff /= total_pixels;

            float diff = (totalrdiff + totalgdiff + totalbdiff) / 3;


            cout << "Diff: " << round(diff * 100) << "%" << endl;

            // show live and wait for a key with timeout long enough to show images

            imshow("Live", frame);
            if (waitKey(1) >= 0) {
                break;
            }



            LastFrame = frame.clone();
        }
    }
    catch (cv::Exception& e)
    {
        cerr << e.msg << endl; // output exception message
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}