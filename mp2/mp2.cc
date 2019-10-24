#include <SerialStream.h>
#include "irobot-create.hh"
#include <ctime>
#include <iostream>
#include <chrono>
#include <thread>
#include <raspicam/raspicam_cv.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <pthread.h>

using namespace iRobot;
using namespace LibSerial;
using namespace std;

int main ()
{
  char serial_loc[] = "/dev/ttyUSB0";

  try {
    //raspicam::RaspiCam_Cv Camera;
    //cv::Mat rgb_image, bgr_image;
    //if (!Camera.open()) {
    //  cerr << "Error opening the camera" << endl;
    //  return -1;
    //}
    //cout << "Opened Camera" << endl;
    SerialStream stream (serial_loc, LibSerial::SerialStreamBuf::BAUD_57600);
    cout << "Opened Serial Stream to" << serial_loc << endl;
    this_thread::sleep_for(chrono::milliseconds(1000));
    Create robot(stream);
    cout << "Created iRobot Object" << endl;
    robot.sendFullCommand();
    cout << "Setting iRobot to Full Mode" << endl;
    cout << "Robot is ready" << endl;
    int speed = 200;
    // Let's stream some sensors.
    Create::sensorPackets_t sensors;
    sensors.push_back(Create::SENSOR_BUMPS_WHEELS_DROPS);
    sensors.push_back(Create::SENSOR_WALL_SIGNAL);
    sensors.push_back (Create::SENSOR_BUTTONS);
    sensors.push_back(Create::SENSOR_CLIFF_LEFT_SIGNAL);
    sensors.push_back(Create::SENSOR_CLIFF_RIGHT_SIGNAL);
    sensors.push_back(Create::SENSOR_OVERCURRENTS);
    robot.sendStreamCommand(sensors);
    this_thread::sleep_for(chrono::milliseconds(1000));
    /**
    Create::song_t song;
    song.push_back(Create::note_t(100, 8));
    song.push_back(Create::note_t(90, 8));
    robot.sendSongCommand(1,song);
    */
    
    robot.sendDriveCommand (speed, Create::DRIVE_STRAIGHT);
    this_thread::sleep_for(chrono::milliseconds(100));

    cout << "Sent drive commnand" << endl;
    while(!robot.playButton()){
      //cout << "in the loop" << endl;
      
      if (robot.bumpLeft () || robot.bumpRight ()) {
              robot.sendDriveCommand(0, Create::DRIVE_STRAIGHT);
              this_thread::sleep_for(chrono::milliseconds(15));
              robot.sendDriveCommand(-speed, Create::DRIVE_STRAIGHT);
              this_thread::sleep_for(chrono::milliseconds(350));
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
                  this_thread::sleep_for(chrono::milliseconds(30));
              }
              std::chrono::steady_clock::time_point maxEnd = std::chrono::steady_clock::now();
              
              int time = std::chrono::duration_cast<std::chrono::milliseconds>(maxEnd - maxTime).count() - 60;
              cout << "MAX WALL SIGNAL: " << maxWallSignal << endl;
              std::chrono::steady_clock::time_point startReturn = std::chrono::steady_clock::now();
              robot.sendDriveCommand(speed, Create::DRIVE_INPLACE_CLOCKWISE);
              while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - startReturn).count() < time){
                  
                  std::this_thread::sleep_for(chrono::milliseconds(30));
              }
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
              this_thread::sleep_for(chrono::milliseconds(50));         
      }
      this_thread::sleep_for(chrono::milliseconds(15));
      cout << "Continous wall sensor: " << robot.wallSignal() << endl;
      this_thread::sleep_for(chrono::milliseconds(500));

    }
    cout << "Play button pressed, stopping Robot" << endl;
    robot.sendDriveCommand (0, Create::DRIVE_STRAIGHT);
  }
  catch (InvalidArgument& e)
  {
    cerr << e.what () << endl;
    return 3;
  }
  catch (CommandNotAvailable& e)
  {
    cerr << e.what () << endl;
    return 4;
  }

}
