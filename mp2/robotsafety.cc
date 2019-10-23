#include <pthread.h>
#include "irobot-create.hh"
#include <chrono>
#include <thread>
#include <iosteam>



using namespace iRobot;

void robotSafety(Create& robot, pthread_mutex_t& robomutex){

    if (robot.wheeldropLeft() || robot.wheeldropRight() || robot.wheeldropCaster() ||
            robot.cliffLeftSignal() >= 10 || robot.cliffFrontLeftSignal() >= 10 ||
            robot.cliffRightSignal() >= 10 || robot.cliffFrontRightSignal() >= 10 ||
            robot.leftWheelOvercurrent() || robot.rightWheelOvercurrent()) {
        //stop
        robot.sendDriveCommand(0, Create::DRIVE_STRAIGHT);

        //play song
        while (!robot.advanceButton()) {
            robot.sendPlaySongCommand(1);
        }

        //start again
        robot.sendDriveCommand(200, Create::DRIVE_STRAIGHT);
    }
}
