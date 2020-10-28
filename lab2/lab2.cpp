#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"


using namespace std;
using namespace cv;

int main( int argc, char** argv ) {
    bool capturing = true;
    
    VideoCapture cap(0);
    
    if ( !cap.isOpened() ) {
        cerr << "error opening frames source" << endl;
        return -1;
    }
    
    int H_low = 0, H_high = 179;
    int S_low = 0 ,S_high = 255;
    int V_low = 0, V_high = 255;
    
    namedWindow("frame_inRange", WINDOW_AUTOSIZE);
    
    createTrackbar("H_low", "frame_inRange", &H_low, 179);
    createTrackbar("H_High", "frame_inRange", &H_high, 179);
    createTrackbar("S_low", "frame_inRange", &S_low, 255);
    createTrackbar("S_high", "frame_inRange", &S_high, 255);
    createTrackbar("V_low", "frame_inRange", &V_low, 255);
    createTrackbar("V_high", "frame_inRange", &V_high, 255);
    
    do {
        
        Mat pic;
        Mat mirror;
        Mat blur;
        Mat frame_HSV;
        Mat frame_inRange;
        
        if ( cap.read( mirror ) ) {
            flip(mirror, mirror, 1);
            //            imshow("Mirror", mirror);
            
            GaussianBlur( mirror, blur, Size( 15, 15 ), 0, 0 );
            imshow("Blurred",blur);
            
            cvtColor(mirror, frame_HSV, COLOR_BGR2HSV);
            imshow("HSV", frame_HSV);
            
            Scalar min = Scalar(H_low, S_low, V_low);
            Scalar max = Scalar(H_high, S_high, V_high);
            
            inRange(mirror, min, max, frame_inRange);
            
            imshow("frame_inRange", frame_inRange);
            
        } else {
            
            capturing = false;
        }
        
        if((waitKey( 1000.0/60.0 )&0x0ff) == 27 ){
            capturing = false;
        }
        
//        if((waitKey( 1000.0/60.0 )&0x0ff) == 32 ) {
//
//            Rect r = selectROI("frame_inRange");
//            Mat roi = frame_inRange(r);
//
//            imwrite("./testimage.jpg", roi);
//
//        }
    } while( capturing );
    return 0;
}


