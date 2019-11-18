#include <pthread.h>
#include "irobot-create.hh"
#include <chrono>
#include <thread>
#include <iostream>
#include <ctime>
#include "robotsafety.hh"
#include <unistd.h>
#include <stdlib.h>


using namespace iRobot;
using namespace std;

void robotSafety(Create& robot, pthread_mutex_t* robomutex, bool& stop){
    cout << "In safety thread" << endl;
    std::chrono::steady_clock::time_point progTimer0;
    std::chrono::steady_clock::time_point progTimer1;

    bool wheeldropright = false;
    bool wheeldropleft = false;
    bool wheeldropcaster = false;
    bool bumpLeft = false;
    bool bumpRight= false;
    int cliff_left = 100;
    int cliff_right = 100;
    int cliff_frontleft = 0;
    int cliff_frontright = 0;
    bool leftwheelo = false;
    bool rightwheelo = false;
    int overcurrent = 0;
    int speed = 0;

    while(!stop){
        pthread_mutex_lock(robomutex);
        progTimer0 = std::chrono::steady_clock::now();


        if ((wheeldropleft = robot.wheeldropLeft()) ||
            (wheeldropright = robot.wheeldropRight()) ||
            (wheeldropcaster = robot.wheeldropCaster()) ||
            (cliff_left > robot.cliffLeftSignal())||
            (cliff_right > robot.cliffRightSignal())||
            overcurrent >= 3)

            {
            //stop
            robot.sendDriveCommand(0, Create::DRIVE_STRAIGHT);
            cout << "wheel drop left : " << wheeldropleft << endl;
            cout << "wheel drop right: " << wheeldropright << endl;
            cout << "wheel drop caster: " << wheeldropcaster << endl;
            cout << "cliff left: " << cliff_left  << endl;
            cout << "cliff front left: " << cliff_frontleft << endl;
            cout << "cliff right: " << cliff_right << endl;
            cout << "cliff front right: " << cliff_frontright << endl;
            cout << "left wheel over : " << leftwheelo << endl;
            cout << "right wheel over: " << rightwheelo << endl;
            cout << "over current counter : " << overcurrent << endl;

            //play song
            while (!robot.advanceButton()) {
                robot.sendPlaySongCommand(1);
                wheeldropleft = robot.wheeldropLeft();
                wheeldropright = robot.wheeldropRight();
                wheeldropcaster = robot.wheeldropCaster();
                cliff_left = robot.cliffLeftSignal();
                cliff_frontright = robot.cliffFrontRightSignal();
                cliff_frontleft = robot.cliffFrontLeftSignal();
                cliff_right = robot.cliffRightSignal();
                leftwheelo = robot.leftWheelOvercurrent();
                rightwheelo = robot.rightWheelOvercurrent();
                cout << "wheel drop left : " << wheeldropleft << endl;
                cout << "wheel drop right: " << wheeldropright << endl;
                cout << "wheel drop caster: " << wheeldropcaster << endl;
                cout << "cliff left: " << cliff_left  << endl;
                cout << "cliff front left: " << cliff_frontleft << endl;
                cout << "cliff right: " << cliff_right << endl;
                cout << "cliff front right: " << cliff_frontright << endl;
                cout << "left wheel over : " << leftwheelo << endl;
                cout << "right wheel over: " << rightwheelo << endl;
                //this_thread::sleep_for(chrono::milliseconds(500));
                sleep(2);
            }

            //start again
            //robot.sendDriveCommand(200, Create::DRIVE_STRAIGHT);

        }
        if ((leftwheelo = robot.leftWheelOvercurrent() )||
            (rightwheelo = robot.rightWheelOvercurrent() )) {
                overcurrent++;
                cout << "over current counter : " << overcurrent << endl;
        } else {
            overcurrent = 0;
        }
        pthread_mutex_unlock(robomutex);
        progTimer1 = std::chrono::steady_clock::now();
        int progTime = std::chrono::duration_cast<std::chrono::milliseconds>(progTimer1-progTimer0).count();
        //cout << "Safety Thread Timer: " << progTime << endl;
        this_thread::sleep_for(chrono::milliseconds(300));
    }
}


// (clifffrontleft = robot.cliffFrontLeftSignal()) < 10 ||
//(clifffrontright = robot.cliffFrontRightSignal()) < 10 ||
//(cliffleft = robot.cliffLeftSignal()) < 10 ||
//(cliffright = robot.cliffRightSignal()) < 10 ||
