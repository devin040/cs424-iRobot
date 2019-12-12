#include <pthread.h>
#include "irobot-create.hh"
#include <chrono>
#include <thread>
#include <iostream>
#include "robotmotion.hh"
#include "robotcontour.hh"

using namespace iRobot;
using namespace std;

#define SPEED 150 //200
#define SLEEP 200 //200

void robotMotion(Create& robot, pthread_mutex_t* robomutex, pthread_mutex_t* cam_mutex, bool& end){
    cout << "In Motion Thread" << endl;
    bool enteredMaze = false;
    int wallCount = 0;
    int wallSum = 0;
    int speed = SPEED;
    bool bumpTurn = false;
    bool lostTurn = false;
    vector<float> distances;
    vector<float> angles;
    float angle = 1.5707;
    int lostWallAdjustmentCounter = 0;
    bool tryAdjust = true;
    int numTurns = 0;
    int numAdjust = 0;
    std::chrono::steady_clock::time_point distclock0;
    std::chrono::steady_clock::time_point distclock1;
    std::chrono::steady_clock::time_point progTimer0;
    std::chrono::steady_clock::time_point progTimer1;

    pthread_mutex_lock(robomutex);
    pthread_mutex_lock(cam_mutex);
    while(!robot.playButton()){
        progTimer0 = std::chrono::steady_clock::now();

        robot.sendDriveCommand (speed, Create::DRIVE_STRAIGHT);
        this_thread::sleep_for(chrono::milliseconds(20));
        if (robot.bumpLeft () || robot.bumpRight () ) {
            lostTurn = false;
            lostWallAdjustmentCounter = 0;
            tryAdjust = true;
            numTurns = 0;
            numAdjust = 0;
            robot.sendDriveCommand(0, Create::DRIVE_STRAIGHT);
            this_thread::sleep_for(chrono::milliseconds(15));
            if (enteredMaze && !bumpTurn){
                distclock0 = std::chrono::steady_clock::now();
            }
            if (enteredMaze && bumpTurn){
                distclock1 = std::chrono::steady_clock::now();
                int bumpclock = std::chrono::duration_cast<std::chrono::milliseconds>(distclock1-distclock0).count();
                float distance = ((float) bumpclock / 1000.0 ) * (float) speed;
                if (distance > 400){
                    distances.push_back(distance / 8);
                    angles.push_back(angle);
                    angle = -angle;
                    cout << "Wrote a left turn distance: " << distance << endl;
                }
            }
            enteredMaze = true;

            robot.sendDriveCommand(-200, Create::DRIVE_STRAIGHT);
            this_thread::sleep_for(chrono::milliseconds(15));
            robot.sendDriveCommand(0, Create::DRIVE_STRAIGHT);
            this_thread::sleep_for(chrono::milliseconds(50));

            short maxWallSignal = 0;
            short wallSignal = -1;

            speed = 100;
            robot.sendDriveCommand(speed, Create::DRIVE_INPLACE_COUNTERCLOCKWISE);
            std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
            std::chrono::steady_clock::time_point maxTime;
            while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count() < 2200){
                wallSignal = robot.wallSignal();
                //cout << "Wall signal: " << wallSignal << endl;
                if (wallSignal >= maxWallSignal ){
                    maxWallSignal = wallSignal;

                }
                pthread_mutex_unlock(cam_mutex);
                this_thread::sleep_for(chrono::milliseconds(15));
                pthread_mutex_lock(cam_mutex);
            }

            cout << "MAX WALL SIGNAL: " << maxWallSignal << endl;

            robot.sendDriveCommand(speed, Create::DRIVE_INPLACE_CLOCKWISE);


            while ((wallSignal = robot.wallSignal()) < (maxWallSignal - 30)){
                //cout << "Looking for max curr at :" << wallSignal << endl;
                std::this_thread::sleep_for(chrono::milliseconds(15));
            }

            robot.sendDriveCommand(0, Create::DRIVE_STRAIGHT);
            this_thread::sleep_for(chrono::milliseconds(50));
            robot.sendDriveCommand(speed, Create::DRIVE_INPLACE_COUNTERCLOCKWISE);
            this_thread::sleep_for(chrono::milliseconds(200));
            robot.sendDriveCommand(0, Create::DRIVE_STRAIGHT);
            this_thread::sleep_for(chrono::milliseconds(50));
            speed = SPEED;
            robot.sendDriveCommand(speed, Create::DRIVE_STRAIGHT);
            distclock0 = std::chrono::steady_clock::now();
            bumpTurn = true;
            this_thread::sleep_for(chrono::milliseconds(50));
        }
        int wallAvg = 10;
        wallSum += robot.wallSignal();
        //cout << "Lost wall  Wall sum: " << wallSum << endl;
        wallCount++;

        if (wallCount == 6){
            wallAvg = wallSum / wallCount;
            wallCount = 0;
            wallSum = 0;
        }
        this_thread::sleep_for(chrono::milliseconds(15));

        cout << "Continous wall sensor: " << robot.wallSignal() << endl;
        this_thread::sleep_for(chrono::milliseconds(15));

        if (enteredMaze && (robot.wallSignal() < 3) && numTurns == 0 && numAdjust < 2){
            tryAdjust = false;
            numAdjust++;
            robot.sendDriveCommand(200, Create::DRIVE_INPLACE_CLOCKWISE);
            this_thread::sleep_for(chrono::milliseconds(100));
            robot.sendDriveCommand(0, Create::DRIVE_STRAIGHT);
            this_thread::sleep_for(chrono::milliseconds(15));
            robot.sendDriveCommand(SPEED, Create::DRIVE_STRAIGHT);
            this_thread::sleep_for(chrono::milliseconds(100));
            lostWallAdjustmentCounter++;
        }
        this_thread::sleep_for(chrono::milliseconds(15));
        if (enteredMaze && (robot.wallSignal() > 90) ){
            lostWallAdjustmentCounter = 0;
            robot.sendDriveCommand(200, Create::DRIVE_INPLACE_COUNTERCLOCKWISE);
            this_thread::sleep_for(chrono::milliseconds(50));
            robot.sendDriveCommand(SPEED, Create::DRIVE_STRAIGHT);
            this_thread::sleep_for(chrono::milliseconds(50));
            tryAdjust = true;
            numTurns = 0;
            numAdjust = 0;
        }
        if (enteredMaze && (wallAvg < 2 || lostWallAdjustmentCounter > 3 ) && numTurns < 2 ){

            numTurns++;
            lostWallAdjustmentCounter = 0;
            tryAdjust = true;
            robot.sendDriveCommand(0, Create::DRIVE_STRAIGHT);
            bumpTurn = false;
            bool recordTime = false;
            if (numTurns == 1){
                distclock1 = std::chrono::steady_clock::now();
                int travTime = std::chrono::duration_cast<std::chrono::milliseconds>(distclock1-distclock0).count() - 1000;
                float distance = ((float) travTime/ 1000.0 ) * (float) speed;
                if (distance > 400){
                    distances.push_back(distance / 8 );
                    angles.push_back(angle);
                    angle = -angle;
                    recordTime = true;
                    cout << "Wrote a Right Turn distance: " << distance << endl;
                }

            }
            this_thread::sleep_for(chrono::milliseconds(15));

            short maxWallSignal = 0;
            short wallSignal = -1;

            speed = 200;

            robot.sendDriveCommand(speed, Create::DRIVE_INPLACE_CLOCKWISE);
            //pthread_mutex_unlock(cam_mutex);
            this_thread::sleep_for(chrono::milliseconds(1000));
            //pthread_mutex_lock(cam_mutex);

            speed = 0;
            robot.sendDriveCommand(speed, Create::DRIVE_STRAIGHT);
            this_thread::sleep_for(chrono::milliseconds(100));
            speed = SPEED;
            robot.sendDriveCommand(speed, Create::DRIVE_STRAIGHT);
            if (recordTime){
                distclock0 = std::chrono::steady_clock::now();
            }
            lostTurn = true;
            this_thread::sleep_for(chrono::milliseconds(50));
        }

        //pthread_mutex_unlock(robomutex);
        pthread_mutex_unlock(robomutex);
        progTimer1 = std::chrono::steady_clock::now();
        int progTime = std::chrono::duration_cast<std::chrono::milliseconds>(progTimer1-progTimer0).count();
        //cout << "Running time motion: " << progTime << endl;
        this_thread::sleep_for(chrono::milliseconds(SLEEP)); //fixed
        pthread_mutex_lock(robomutex);

    }
    distclock1 = std::chrono::steady_clock::now();
    int fintimer = std::chrono::duration_cast<std::chrono::milliseconds>(distclock1-distclock0).count();
    float distance = ((float) fintimer/ 1000 ) * (float) speed;
    distances.push_back(distance / 4);
    //angles.push_back(1.5707);
    robotContour(distances, angles);


    std::cout << "Play button pressed, stopping Robot" << endl;
    robot.sendDriveCommand (0, Create::DRIVE_STRAIGHT);
    end = true;
    pthread_mutex_unlock(robomutex);

}
