#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

CascadeClassifier face_cascade;
CascadeClassifier eye_cascade;
CascadeClassifier profile_cascade;

vector<Point2f> detectAndDisplayProfile(Mat frame);

int j = 0;

int main(void) {

    VideoCapture capture;
    Mat frame;

    //-- 1. Load the cascade
    if(!face_cascade.load("/Users/darina/opencv-4.5.0/data/lbpcascades/lbpcascade_frontalface_improved.xml")) {
        return -9;
    };
    if( !profile_cascade.load(String("/Users/darina/opencv-4.5.0/data/haarcascades/haarcascade_profileface.xml"))) {
        return -7;
    };
    if( !eye_cascade.load(String("/Users/darina/opencv-4.5.0/data/haarcascades/haarcascade_eye.xml"))) {
        return -8;
    };

    capture.open(0);
    if (!capture.isOpened()) {
        return -6;
    }

    while (capture.read(frame)) {

        if(frame.empty()) {
            return -1;
        }

        flip(frame, frame, 1);

        detectAndDisplayProfile(frame);

        imshow("Frame", frame);

        if((waitKey(1)&0x0ff) == 27) {
            return 0;
        }
    }
    return 0;
}

vector<Point2f> detectAndDisplayProfile(Mat frame) {

    vector<Rect> faces;

    Mat frame_gray;
    Mat flipped;

    cvtColor( frame, frame_gray, COLOR_BGR2GRAY );
    equalizeHist( frame_gray, frame_gray );

    profile_cascade.detectMultiScale(frame_gray, faces, 1.3, 3, 0, Size(190, 190), Size(400,400));

    flip(frame_gray, flipped, 1);
    profile_cascade.detectMultiScale(flipped, faces, 1.3, 3, 0, Size(190, 190), Size(400,400));

    putText(frame, format("j:(%d)", j), Point(20, 70), FONT_HERSHEY_PLAIN, 2, {0,255,0});

    for( size_t i = 0; i < faces.size(); i++ ) {

        Mat faceROI = frame_gray( faces[i] );
        Mat faceROI_right = flipped( faces[i] );

        vector<Rect> eye;
        eye_cascade.detectMultiScale(faceROI, eye, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(5, 5));

        if(eye.size() < 2) {
//            Point center( faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5 );
//            ellipse( frame, center, Size( faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );
            imshow ( "ROI", faceROI );
            imshow ( "ROI", faceROI_right );
            j += 1;

            vector <Point2f> dst = {
                    Point2f(faces[i].x, faces[i].y + faces[i].height * 5 / 20),
                    Point2f(faces[i].x + faces[i].width, faces[i].y + faces[i].height * 5 / 20),
                    Point2f(faces[i].x + faces[i].width,
                            faces[i].y + faces[i].height * 5 / 20 + faces[i].height * 3 / 10),
                    Point2f(faces[i].x, faces[i].y + faces[i].height * 5 / 20 + faces[i].height * 3 / 10)
            };
            return dst;
        }
    }

    return {};
}
