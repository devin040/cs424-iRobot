#include <pthread.h>
#include "irobot-create.hh"
#include <chrono>
#include <thread>
#include <iosteam>


using namespace std;
using namespace iRobot;

void robotContour(Create& robot, pthread_mutex_t& robomutex){
    vector<Point2f> waypoints;

    while (1) {
        Point2f lastWayPoint = waypoints.back();
        float distTraveled = robot.distance();


        //request distance()
    }


    /**
    adding a point:

    Point2f pt(10,10);
    way.points.push_back(pt);

    OR

    waypoints.push_back(Point2f(10,10));

    printing points:
    for (auto point: waypoints)
        cout << point << endl;

    **/


}
