#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;
using namespace cv;

class MyObject
{
public:
    int maxC;
    vector<Point> pos;
    Point getP()
    {
        Point sump = accumulate(pos.begin(), pos.end(), Point(0, 0));
        sump.x /= pos.size();
        sump.y /= pos.size();
        return sump;
    }
    void addP(Point p)
    {
        pos.push_back(p);
        if (pos.size() > maxC)
        {
            pos = vector<Point>(pos.begin() + 1, pos.end());
        }
    }
    void addEmpty()
    {
        if (pos.size() > 0)
        {
            pos = vector<Point>(pos.begin() + 1, pos.end());
        }
    }
};

int main( int argc, char** argv ) {
    int loRange[3] = {0, 120, 70};
    int hiRange[3] = {10, 255, 255};
    
    auto sub = 0;
        
    VideoCapture camera(0);
    
    MyObject myobj;
    MyObject myobj2;
    
    myobj.maxC = 10;
    myobj2.maxC = 10;
    
    while (waitKey(1) != 27)
    {
        int dilation_size = 5;
        auto structElem = getStructuringElement(MORPH_ELLIPSE,
                                                Size(2 * dilation_size + 1, 2 * dilation_size + 1),
                                                Point(dilation_size, dilation_size));
        Mat frame;
        Mat backgroundScaled;
        Mat frameMask, frameNegMask;
        Mat frameWithMask, backgroundScaledWithMask;
        Mat meinniceplace;
        
        camera >> frame;
        flip(frame, frame, 1);
        
        cvtColor(frame, frameMask, COLOR_BGR2HSV);
        inRange(frameMask, Scalar(loRange[0], loRange[1], loRange[2]),
                Scalar(hiRange[0], hiRange[1], hiRange[2]), frameNegMask);
        morphologyEx(frameNegMask, frameNegMask, MORPH_CLOSE, structElem);
        morphologyEx(frameNegMask, frameNegMask, MORPH_OPEN, structElem);
        imshow("dilate", frameNegMask);
        
        vector<vector<Point>> contours;
        findContours(frameNegMask, contours, RETR_LIST, CHAIN_APPROX_TC89_KCOS);
        sort(contours.begin(), contours.end(),
             [](auto &a, auto &b) {
                 return contourArea(a, false) > contourArea(b, false);
             });
        
        for (int i = 0; i < contours.size(); i++)
        {
            approxPolyDP(contours.at(i), contours.at(i), 10, true);

        }

        if (contours.size())
        {
            Point avg;
            Rect r = boundingRect(contours.at(0));
            avg.x = r.x + r.width / 2;
            avg.y = r.y + r.height / 2;
            myobj.addP(avg);

            if (contours.size() > 1) {
                Point avg2;
                Rect r2 = boundingRect(contours.at(1));
                avg2.x = r2.x + r2.width / 2;
                avg2.y = r2.y + r2.height / 2;
                myobj2.addP(avg2);
            }
        }
        
        else {
            myobj.addEmpty();
            myobj2.addEmpty();
        }

        if (myobj.pos.size() > 1 && myobj2.pos.size() > 1) {
            
            sub = abs(myobj.getP().y - myobj2.getP().y);
                
                if(sub + 30 <= 60){
                    putText(frame, "1", myobj.getP(), FONT_HERSHEY_PLAIN, 2, {0,0,0});
                    putText(frame, "2", myobj2.getP(), FONT_HERSHEY_PLAIN, 2, {0,0,0});
                    line(frame, myobj.getP(), myobj2.getP(), (255,0,0), 5);
                }
            putText(frame, format("abs(y1-y2):(%d)", sub), Point(20, 70), FONT_HERSHEY_DUPLEX, 2, CV_RGB(118, 185, 0), 2);
        }
        
        imshow("contours", frame);
    }
    return 0;
}
