#include <pthread.h>
#include "irobot-create.hh"
#include <chrono>
#include <thread>
#include <iostream>
#include "robotmotion.hh"
#include "robotcontour.hh"



using namespace iRobot;
using namespace std;

void robotMotion(Create& robot, pthread_mutex_t* robomutex, bool& end){
    cout << "In Motion Thread" << endl;
    bool enteredMaze = false;
    int wallCount = 0;
    int wallSum = 0;
    int speed = 200;
    bool bumpTurn = false;
    bool lostTurn = false; 
    vector<float> distances;
    vector<float> angles;
    std::chrono::steady_clock::time_point distclock0;
    std::chrono::steady_clock::time_point distclock1;

    pthread_mutex_lock(robomutex);
    while(!robot.playButton()){
      //pthread_mutex_lock(robomutex);
      
      robot.sendDriveCommand (speed, Create::DRIVE_STRAIGHT);
      this_thread::sleep_for(chrono::milliseconds(20));
      if (robot.bumpLeft () || robot.bumpRight () || (robot.wallSignal() > 180)) {
              lostTurn = false;
              
              robot.sendDriveCommand(0, Create::DRIVE_STRAIGHT);
              this_thread::sleep_for(chrono::milliseconds(15));
              if (enteredMaze && !bumpTurn){
                  distclock1 = std::chrono::steady_clock::now();
                  int bumpclock = std::chrono::duration_cast<std::chrono::milliseconds>(distclock1-distclock0).count();
                  float distance = ((float) bumpclock / 1000.0 ) * (float) speed;
                  distances.push_back(distance);
                  angles.push_back(-1.5707);  
                  cout << "Wrote a left turn" << endl;
              }
              enteredMaze = true;
              
              robot.sendDriveCommand(-speed, Create::DRIVE_STRAIGHT);
              this_thread::sleep_for(chrono::milliseconds(15));
              robot.sendDriveCommand(0, Create::DRIVE_STRAIGHT);
              this_thread::sleep_for(chrono::milliseconds(200));
              
              short maxWallSignal = 0;
              short wallSignal = -1;

              speed = 100;
              robot.sendDriveCommand(speed, Create::DRIVE_INPLACE_COUNTERCLOCKWISE);
              std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
              std::chrono::steady_clock::time_point maxTime;
              while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count() < 2200){
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
              this_thread::sleep_for(chrono::milliseconds(100));
              robot.sendDriveCommand(speed, Create::DRIVE_INPLACE_COUNTERCLOCKWISE);
              this_thread::sleep_for(chrono::milliseconds(200));
              robot.sendDriveCommand(0, Create::DRIVE_STRAIGHT);
              this_thread::sleep_for(chrono::milliseconds(100));
              speed = 200;
              robot.sendDriveCommand(speed, Create::DRIVE_STRAIGHT);
              distclock0 = std::chrono::steady_clock::now();
              bumpTurn = true;
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
        bumpTurn = false;
        if (!lostTurn){
            distclock1 = std::chrono::steady_clock::now();
            int travTime = std::chrono::duration_cast<std::chrono::milliseconds>(distclock1-distclock0).count();
            float distance = ((float) travTime/ 1000.0 ) * (float) speed;
            distances.push_back(distance);
            angles.push_back(1.5707);
            cout << "Wrote a Write Turn" << endl;
        }
        
        
        this_thread::sleep_for(chrono::milliseconds(15));  

          
          short maxWallSignal = 0;
          short wallSignal = -1;

          speed = 200;
          
          robot.sendDriveCommand(speed, Create::DRIVE_INPLACE_CLOCKWISE);
          this_thread::sleep_for(chrono::milliseconds(1000));
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
          speed = 200;
          robot.sendDriveCommand(speed, Create::DRIVE_STRAIGHT);
          distclock0 = std::chrono::steady_clock::now();
          lostTurn = true;
          this_thread::sleep_for(chrono::milliseconds(50));   
      }
      pthread_mutex_unlock(robomutex);
      this_thread::sleep_for(chrono::milliseconds(200));
      pthread_mutex_lock(robomutex); 

    }
    distclock1 = std::chrono::steady_clock::now();
    int fintimer = std::chrono::duration_cast<std::chrono::milliseconds>(distclock1-distclock0).count();
    float distance = ((float) fintimer/ 1000 ) * (float) speed;
    distances.push_back(distance);
    angles.push_back(1.5707);
    robotContour(distances, angles);
    vector<float> distances2;
    vector<float> angles2;
    distances2.push_back(1000.0);
    distances2.push_back(1000.0);
    distances2.push_back(1000.0);
    distances2.push_back(1000.0);
    angles2.push_back(1.5707);
    angles2.push_back(-1.5707);
    angles2.push_back(1.5707);
    angles2.push_back(-1.5707);
    robotContour(distances2, angles2);

    std::cout << "Play button pressed, stopping Robot" << endl;
    robot.sendDriveCommand (0, Create::DRIVE_STRAIGHT);
    end = true;
    pthread_mutex_unlock(robomutex);

}

