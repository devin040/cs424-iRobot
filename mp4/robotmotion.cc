#include <pthread.h>
#include "irobot-create.hh"
#include <chrono>
#include <thread>
#include <iostream>
#include "robotmotion.hh"

using namespace iRobot;
using namespace std;

#define TRAVELSPEED 100
#define SLEEP 200 
#define TSLEEP(x) this_thread::sleep_for(chrono::milliseconds(x))
void findMax(Create&);

void robotMotion(Create& robot, pthread_mutex_t* robomutex, pthread_mutex_t* cam_mutex, bool& end){
    cout << "In Motion Thread" << endl;
    bool enteredMaze = false;

    int speed = TRAVELSPEED;

    pthread_mutex_lock(robomutex);

    while(!robot.playButton()){
        vector<short> wallvec;
        robot.sendDriveCommand (speed, Create::DRIVE_STRAIGHT);
        this_thread::sleep_for(chrono::milliseconds(20));
        if (robot.bumpLeft () || robot.bumpRight () ) {
            robot.sendDriveCommand(0, Create::DRIVE_STRAIGHT);
            this_thread::sleep_for(chrono::milliseconds(15));

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
            
            while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count() < 2200){
                wallSignal = robot.wallSignal();
                //cout << "Wall signal: " << wallSignal << endl;
                if (wallSignal >= maxWallSignal ){
                    maxWallSignal = wallSignal;

                }
                this_thread::sleep_for(chrono::milliseconds(15));
            }

            cout << "MAX WALL SIGNAL: " << maxWallSignal << endl;
            robot.sendDriveCommand(speed, Create::DRIVE_INPLACE_CLOCKWISE);


            while ((wallSignal = robot.wallSignal()) < (maxWallSignal - 30)){

                std::this_thread::sleep_for(chrono::milliseconds(15));
            }

            robot.sendDriveCommand(0, Create::DRIVE_STRAIGHT);
            this_thread::sleep_for(chrono::milliseconds(50));
            robot.sendDriveCommand(speed, Create::DRIVE_INPLACE_COUNTERCLOCKWISE);
            this_thread::sleep_for(chrono::milliseconds(200));
            robot.sendDriveCommand(0, Create::DRIVE_STRAIGHT);
            this_thread::sleep_for(chrono::milliseconds(50));
            speed = TRAVELSPEED;
            robot.sendDriveCommand(TRAVELSPEED, Create::DRIVE_STRAIGHT);
            this_thread::sleep_for(chrono::milliseconds(50));
        } else {
 
            cout << "Continous wall sensor: " << robot.wallSignal() << endl;
            this_thread::sleep_for(chrono::milliseconds(15));
            short wallsig = robot.wallSignal();
            short prevWall = wallsig;
            
            robot.sendDriveCommand(TRAVELSPEED, Create::DRIVE_STRAIGHT);
            short desiredWallSigLow = 20; //40
            short desiredWallSigHigh = 40; //60 work good
            prevWall = wallsig;
            

            if (false){
                robot.sendDriveCommand(25, Create::DRIVE_STRAIGHT);
                
                while (true){
                    cout << "WOAH: WAll sig: " << robot.wallSignal() << endl;
                    TSLEEP(1000);
                }
                findMax(std::ref(robot));
                cout << "OKAY: Wall sig: " << robot.wallSignal() << endl;
                robot.sendDriveCommand(TRAVELSPEED, Create::DRIVE_STRAIGHT);
                TSLEEP(1000);
            } else {
                if (wallsig < desiredWallSigLow){
                    short radius = -100 + wallsig / (float) desiredWallSigLow * -750;
                    robot.sendDriveCommand(TRAVELSPEED, radius);
                    prevWall = wallsig;
                    cout << "Radius : " << radius << endl;
                    TSLEEP(15);
                }
                if ((wallsig) > desiredWallSigHigh){
                    //short radius = 1000 - (wallsig - desiredWallSigHigh)/ (float) desiredWallSigHigh * 750;
                    //short radius = 1000 - ( (wallsig - desiredWallSigHigh) / 100.0 ) * 900;
                    short radius = 300 - ( (wallsig - desiredWallSigHigh) / 50.0 ) * 299;
                    if (wallsig > 90){
                        radius = 5;
                    }
                    robot.sendDriveCommand(TRAVELSPEED, radius);
                    prevWall = wallsig;
                    cout << "Radius : " << radius << endl;
                    TSLEEP(200);
                    if (wallsig > 60){
                        findMax(robot);
                    }

                }
            }

            cout << "Wall Sig: " << wallsig << endl;
            
        }


        
        pthread_mutex_unlock(robomutex);

        //cout << "Running time motion: " << progTime << endl;
        TSLEEP(15); //fixed
        pthread_mutex_lock(robomutex);

    }

    std::cout << "Play button pressed, stopping Robot" << endl;
    robot.sendDriveCommand (0, Create::DRIVE_STRAIGHT);
    end = true;
    pthread_mutex_unlock(robomutex);

}

void findMax(Create& robot){
    robot.sendDriveCommand(0, Create::DRIVE_STRAIGHT);
        this_thread::sleep_for(chrono::milliseconds(50));

        short maxWallSignal = 0;
        short wallSignal = -1;

        short speed = 100;
        robot.sendDriveCommand(speed, Create::DRIVE_INPLACE_COUNTERCLOCKWISE);
        std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
        
        while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count() < 2000){
            wallSignal = robot.wallSignal();
            //cout << "Wall signal: " << wallSignal << endl;
            if (wallSignal >= maxWallSignal ){
                maxWallSignal = wallSignal;

            }
            this_thread::sleep_for(chrono::milliseconds(15));
        }

        cout << "MAX WALL SIGNAL: " << maxWallSignal << endl;
        robot.sendDriveCommand(speed, Create::DRIVE_INPLACE_CLOCKWISE);


        while ((wallSignal = robot.wallSignal()) < (maxWallSignal)){

            std::this_thread::sleep_for(chrono::milliseconds(15));
        }

        robot.sendDriveCommand(0, Create::DRIVE_STRAIGHT);
        this_thread::sleep_for(chrono::milliseconds(50));
        robot.sendDriveCommand(speed, Create::DRIVE_INPLACE_COUNTERCLOCKWISE);
        this_thread::sleep_for(chrono::milliseconds(50));
        robot.sendDriveCommand(0, Create::DRIVE_STRAIGHT);
        this_thread::sleep_for(chrono::milliseconds(50));
        speed = TRAVELSPEED;
        robot.sendDriveCommand(TRAVELSPEED, Create::DRIVE_STRAIGHT);
        this_thread::sleep_for(chrono::milliseconds(50));
        while (robot.wallSignal() > 40 ){
            robot.sendDriveCommand(TRAVELSPEED, 500);
            TSLEEP(15);
        }
        robot.sendDriveCommand(TRAVELSPEED, Create::DRIVE_STRAIGHT);
}
