#include <pthread.h>
#include "irobot-create.hh"
#include <chrono>
#include <thread>
#include <iosteam>

#define IMAGE_HEIGHT 1200
#define IMAGE_WIDTH 1600

using namespace std;
using namespace iRobot;

Poin2f getImageCoordinates(Point2f pos) {
    int x = IMAGE_WIDTH/2 + pos.x;
    int y = IMAGE_HEIGHT/2 + pos.y;
    return Point2f(x, y);
}

Point2f getNextPosition(Point2f currPos, float distance, float angle) {
    float newX = distance*cos(angle) + currPos.x;
    float newY = distance*sin(angle) + currPos.y;
    return Point2f(newX, newY);
}

void robotContour(vector<float> distances, vector<float> angles){
    vector<Point2f> waypoints;
    Point2f currPos(0,0);
    waypoints.push_back(currPos);

    // found under mp2_hints
    Mat img_output(IMAGE_HEIGHT, IMAGE_WIDTH, CV_8UC3, Scalar(255, 255, 255));
    Scalar lineColor(255, 0, 0);

    int lineWidth = 1;
    int radius = 3;

    float currAngle = 0;

    Point2f currImgCoord = getImageCoordinates(currPos);
    Point2f nextImgCoord;

    //robotmotion will keep track of the angle and distances
    //assume 90 degree corners!!
    for (int i = 0; i < distances.size(); i++) {
        Point2f nextPos = getNextPosition(currPos, distances[i], currAngle);
        waypoints.push_back(nextPos);
        nextImgCoord = getImageCoordinates(nextPos);
        line(img_output, currImgCoord, nextImgCoord, lineColor, lineWidth, CV_AA);
        circle(img_output, currImgCoord, radius, lineColor, CV_FILLED, CV_AA);

        currPos = nextPos;
        currImgCoord = nextImgCoord;

        if (i < distances.size() - 1) {
            currAngle += angles[i];
        }
    }

    Rect bound = boundingRect(waypoints);
    rectangle(img_output, bound, Scalar(0, 165, 255));

    imwrite("irobot_plot.png", img_output);


}
