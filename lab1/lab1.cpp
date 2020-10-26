#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

int main( int argc, char** argv ) {
    bool capturing = true;
    // Question for you
    // cv::VideoCapture cap( "szukaj_zielonego.webm" );
    VideoCapture cap(0);
    
    if ( !cap.isOpened() ) {
        cerr << "error opening frames source" << endl;
        return -1;
    }
    cout << "Video size: " << cap.get( CV_CAP_PROP_FRAME_WIDTH )
    << "x" << cap.get( CV_CAP_PROP_FRAME_HEIGHT ) << endl;
    do {
        Mat frame;
        Mat mirror;
        
        if ( cap.read( frame ) ) {
            flip(frame, mirror, 1);
            imshow("Not mirror", frame );
            imshow("Mirror", mirror);
        } else {
            // stream finished
            capturing = false;
        }
        //czekaj na klawisz, sprawdz czy to jest 'esc'
        if( (waitKey( 1000.0/60.0 )&0x0ff) == 27 ) capturing = false;
    } while( capturing );
    return 0;
}

