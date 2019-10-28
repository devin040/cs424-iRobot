#include <pthread.h>
#include "irobot-create.hh"
#include <chrono>
#include <thread>
#include <iostream>
#include "robotmotion.hh"



using namespace iRobot;
using namespace std;

void robotMotion(Create& robot, pthread_mutex_t* robomutex, bool& end){
    cout << "In Motion Thread" << endl;
    bool enteredMaze = false;
    int wallCount = 0;
    int wallSum = 0;
    int speed = 200;
    pthread_mutex_lock(robomutex);
    while(!robot.playButton()){
      //pthread_mutex_lock(robomutex);
      
      robot.sendDriveCommand (speed, Create::DRIVE_STRAIGHT);
      this_thread::sleep_for(chrono::milliseconds(20));
      if (robot.bumpLeft () || robot.bumpRight () || (robot.wallSignal() > 180)) {
              enteredMaze = true;
              robot.sendDriveCommand(0, Create::DRIVE_STRAIGHT);
              this_thread::sleep_for(chrono::milliseconds(15));
              robot.sendDriveCommand(-speed, Create::DRIVE_STRAIGHT);
              this_thread::sleep_for(chrono::milliseconds(5));
              robot.sendDriveCommand(0, Create::DRIVE_STRAIGHT);
              this_thread::sleep_for(chrono::milliseconds(200));
              
              short maxWallSignal = 0;
              short wallSignal = -1;

              speed = 100;
              robot.sendDriveCommand(speed, Create::DRIVE_INPLACE_COUNTERCLOCKWISE);
              std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
              std::chrono::steady_clock::time_point maxTime;
              while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count() < 2500){
                  wallSignal = robot.wallSignal();
                  cout << "Wall signal: " << wallSignal << endl;
                  if (wallSignal >= maxWallSignal ){
                      maxWallSignal = wallSignal;
                      maxTime = std::chrono::steady_clock::now();
                  }
                  this_thread::sleep_for(chrono::milliseconds(15));
              }
              std::chrono::steady_clock::time_point maxEnd = std::chrono::steady_clock::now();
              
              int time = std::chrono::duration_cast<std::chrono::milliseconds>(maxEnd - maxTime).count() - 100;
              cout << "MAX WALL SIGNAL: " << maxWallSignal << endl;
              std::chrono::steady_clock::time_point startReturn = std::chrono::steady_clock::now();
              robot.sendDriveCommand(speed, Create::DRIVE_INPLACE_CLOCKWISE);
              /**
              while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - startReturn).count() < time){
                  
                  std::this_thread::sleep_for(chrono::milliseconds(30));
              }
              */
              
              while ((wallSignal = robot.wallSignal()) < (maxWallSignal - 30)){
                 cout << "Looking for max curr at :" << wallSignal << endl;
                  std::this_thread::sleep_for(chrono::milliseconds(15));
              }
              
              robot.sendDriveCommand(0, Create::DRIVE_STRAIGHT);
              this_thread::sleep_for(chrono::milliseconds(200));
              speed = 300;
              robot.sendDriveCommand(speed, Create::DRIVE_STRAIGHT);
              this_thread::sleep_for(chrono::milliseconds(50));         
      }
      int wallAvg = 10;
      wallSum += robot.wallSignal();
      cout << "Lost wall  Wall sum: " << wallSum << endl;
      wallCount++;
      if (wallCount == 6){
          wallAvg = wallSum / wallCount;
          wallCount = 0;
          wallSum = 0;
      }

      this_thread::sleep_for(chrono::milliseconds(15));
      //cout << "Continous wall sensor: " << robot.wallSignal() << endl;
      if (enteredMaze && wallAvg < 2){
          robot.sendDriveCommand(0, Create::DRIVE_STRAIGHT);
          this_thread::sleep_for(chrono::milliseconds(15));
          
          short maxWallSignal = 0;
          short wallSignal = -1;

          speed = 100;
          
          robot.sendDriveCommand(speed, Create::DRIVE_INPLACE_CLOCKWISE);
          this_thread::sleep_for(chrono::milliseconds(2000));
          /**
          while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - startReturn).count() < time){
              
              std::this_thread::sleep_for(chrono::milliseconds(30));
          }
          */
          /**
          while ((wallSignal = robot.wallSignal()) < maxWallSignal){
              cout << "Looking for max curr at :" << wallSignal << endl;
              std::this_thread::sleep_for(chrono::milliseconds(30));
          }
          */
          
          speed = 0;
          robot.sendDriveCommand(speed, Create::DRIVE_STRAIGHT);
          this_thread::sleep_for(chrono::milliseconds(200));
          speed = 300;
          robot.sendDriveCommand(speed, Create::DRIVE_STRAIGHT);
          this_thread::sleep_for(chrono::milliseconds(50));   
      }
      pthread_mutex_unlock(robomutex);
      this_thread::sleep_for(chrono::milliseconds(200));
      pthread_mutex_lock(robomutex); 

    }
    cout << "Play button pressed, stopping Robot" << endl;
    robot.sendDriveCommand (0, Create::DRIVE_STRAIGHT);
    end = true;

}

