#include <pthread.h>
#include "irobot-create.hh"
#include <chrono>
#include <thread>
#include <iosteam>



using namespace iRobot;

void robotMotion(Create& robot, pthread_mutex_t& robomutex, bool end){

    int speed = 200;
    robot.sendDriveCommand (speed, Create::DRIVE_STRAIGHT);
    if (robot.bumpLeft () || robot.bumpRight ()) {
            speed = 0;
            robot.sendDriveCommand(speed, Create::DRIVE_STRAIGHT);
            this_thread::sleep_for(chrono::milliseconds(15));
            cout << "Bump : " << count++ << endl;
            speed = -152;
            robot.sendDriveCommand(speed, Create::DRIVE_STRAIGHT);
            this_thread::sleep_for(chrono::milliseconds(1500));
            speed = 0;
            robot.sendDriveCommand(speed, Create::DRIVE_STRAIGHT);
            short maxWallSignal = 0;
            bool localMaxFound = false;
            short wallSignal = -1;
            bool rotateRight = true;
            bool rotateLeft = false;
            bool sendRightCommand = true;
            this_thread::sleep_for(chrono::milliseconds(200));

            speed = 50;
            robot.sendDriveCommand(speed, Create::DRIVE_INPLACE_COUNTERCLOCKWISE);
            std::chrono::time_point start = std::chrono::steady_clock::now();
            while (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - start).count() < 3000){
                wallSignal = robot.wallSignal();
                if (wallSignal > maxWallSignal ){
                    maxWallSignal = wallSignal;
                }
                std::this_thread::sleep_for(chrono::milliseconds(20));
            }
            robot.sendDriveCommand(speed, Create::DRIVE_INPLACE_CLOCKWISE);
            while (robot.wallSignal() < maxWallSignal){
                std::this_thread::sleep_for(chrono::milliseconds(20));
            }
            speed = 0;
            robot.sendDriveCommand(speed, Create::DRIVE_STRAIGHT);
            std::this_thread::sleep_for(chrono::milliseconds(200));
            speed = 100;
            robot.sendDriveCommand(speed, Create::DRIVE_STRAIGHT);
                
    }

}

