#include <pthread.h>
#include "irobot-create.hh"
#include <chrono>
#include <thread>
#include <iostream>
#include <ctime>
#include "robotsafety.hh"
#include <unistd.h>


using namespace iRobot;
using namespace std;

void robotSafety(Create& robot, pthread_mutex_t* robomutex, bool& stop){
    cout << "In safety thread" << endl;
    bool wheeldropright = false;
    bool wheeldropleft = false;
    bool wheeldropcaster = false;
    int cliffleft = 12;
    int cliffright = 12;
    int clifffrontleft = 12;
    int clifffrontright = 12;
    bool leftwheelo = false;
    bool rightwheelo = false;
    while(!stop){
        pthread_mutex_lock(robomutex);
        if ((wheeldropleft = robot.wheeldropLeft()) || 
            (wheeldropright = robot.wheeldropRight()) || 
            (wheeldropcaster = robot.wheeldropCaster()) ||
            (cliffleft = robot.cliffLeftSignal()) < 10 ||
            
            (cliffright = robot.cliffRightSignal()) < 10 || 
            
            (leftwheelo = robot.leftWheelOvercurrent() )|| 
            (rightwheelo = robot.rightWheelOvercurrent())) {
            //stop
            robot.sendDriveCommand(0, Create::DRIVE_STRAIGHT);
            cout <<  wheeldropleft << endl;
            cout << wheeldropright << endl;
            cout << wheeldropcaster << endl; 
            cout << cliffleft  << endl;
            cout << clifffrontleft << endl;
            cout << cliffright << endl;
            cout << clifffrontright << endl;
            cout << leftwheelo << endl;
            cout << rightwheelo << endl;

            //play song
            while (!robot.advanceButton()) {
                robot.sendPlaySongCommand(1);
                //this_thread::sleep_for(chrono::milliseconds(500));
                sleep(1);
            }  

            //start again
            //robot.sendDriveCommand(200, Create::DRIVE_STRAIGHT);
            
        }
        pthread_mutex_unlock(robomutex);
        this_thread::sleep_for(chrono::milliseconds(1000));
    } 
}


// (clifffrontleft = robot.cliffFrontLeftSignal()) < 10 ||
//(clifffrontright = robot.cliffFrontRightSignal()) < 10 ||
 