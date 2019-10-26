#include <pthread.h>
#include "irobot-create.hh"
#include <chrono>
#include <thread>
#include <iostream>
#include <ctime>
#include "robotsafety.hh"


using namespace iRobot;
using namespace std;

void robotSafety(Create& robot, pthread_mutex_t& robomutex, bool& stop){
    cout << "In safety thread" << endl;
    while(!stop){
        pthread_mutex_lock(&robomutex);
        if (robot.wheeldropLeft() || robot.wheeldropRight() || robot.wheeldropCaster() ||
                robot.cliffLeftSignal() < 10 || robot.cliffFrontLeftSignal() < 10 ||
                robot.cliffRightSignal() < 10 || robot.cliffFrontRightSignal() < 10 ||
                robot.leftWheelOvercurrent() || robot.rightWheelOvercurrent()) {
            //stop
            robot.sendDriveCommand(0, Create::DRIVE_STRAIGHT);

            //play song
            while (!robot.advanceButton()) {
                robot.sendPlaySongCommand(1);
                this_thread::sleep_for(chrono::milliseconds(200));

            }

            //start again
            //robot.sendDriveCommand(200, Create::DRIVE_STRAIGHT);
            
        }
        pthread_mutex_unlock(&robomutex);
        this_thread::sleep_for(chrono::milliseconds(400));
    } 
}
