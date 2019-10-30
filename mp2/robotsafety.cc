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
    std::chrono::steady_clock::time_point progTimer0;
    std::chrono::steady_clock::time_point progTimer1;

    bool wheeldropright = false;
    bool wheeldropleft = false;
    bool wheeldropcaster = false;
    int cliffleft = 12;
    int cliffright = 12;
    int clifffrontleft = 12;
    int clifffrontright = 12;
    bool leftwheelo = false;
    bool rightwheelo = false;
    int overcurrent = 0;
    while(!stop){
        pthread_mutex_lock(robomutex);
        progTimer0 = std::chrono::steady_clock::now();
        if ((wheeldropleft = robot.wheeldropLeft()) || 
            (wheeldropright = robot.wheeldropRight()) || 
            (wheeldropcaster = robot.wheeldropCaster()) ||
            overcurrent >= 3) {
            //stop
            robot.sendDriveCommand(0, Create::DRIVE_STRAIGHT);
            cout << "wheel drop left : " << wheeldropleft << endl;
            cout << "wheel drop right: " << wheeldropright << endl;
            cout << "wheel drop caster: " << wheeldropcaster << endl; 
            cout << "cliff left: " << cliffleft  << endl;
            cout << "cliff front left: " << clifffrontleft << endl;
            cout << "cliff right: " << cliffright << endl;
            cout << "cliff front right: " << clifffrontright << endl;
            cout << "left wheel over : " << leftwheelo << endl;
            cout << "right wheel over: " << rightwheelo << endl;
            cout << "over current counter : " << overcurrent << endl;

            //play song
            while (!robot.advanceButton()) {
                robot.sendPlaySongCommand(1);
                wheeldropleft = robot.wheeldropLeft(); 
                wheeldropright = robot.wheeldropRight();
                wheeldropcaster = robot.wheeldropCaster();
                cliffleft = robot.cliffLeftSignal();
                clifffrontright = robot.cliffFrontRightSignal();
                clifffrontleft = robot.cliffFrontLeftSignal();
                cliffright = robot.cliffRightSignal();
                leftwheelo = robot.leftWheelOvercurrent(); 
                rightwheelo = robot.rightWheelOvercurrent();
                cout << "wheel drop left : " << wheeldropleft << endl;
                cout << "wheel drop right: " << wheeldropright << endl;
                cout << "wheel drop caster: " << wheeldropcaster << endl; 
                cout << "cliff left: " << cliffleft  << endl;
                cout << "cliff front left: " << clifffrontleft << endl;
                cout << "cliff right: " << cliffright << endl;
                cout << "cliff front right: " << clifffrontright << endl;
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
        cout << "Safety Thread Timer: " << progTime << endl;
        this_thread::sleep_for(chrono::milliseconds(300));
    } 
}


// (clifffrontleft = robot.cliffFrontLeftSignal()) < 10 ||
//(clifffrontright = robot.cliffFrontRightSignal()) < 10 ||
//(cliffleft = robot.cliffLeftSignal()) < 10 ||         
//(cliffright = robot.cliffRightSignal()) < 10 || 
 