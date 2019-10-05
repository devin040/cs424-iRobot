#include <SerialStream.h>
#include "irobot-create.hh"
#include <ctime>
#include <iostream>
#include <chrono>
#include <thread>
#include <raspicam/raspicam_cv.h>
#include <opencv2/imgproc/imgproc.hpp>

using namespace iRobot;
using namespace LibSerial;
using namespace std;

void playSong(Create&, bool&, bool&);
void playLEDS(Create&, bool&);

int main ()
{
  char serial_loc[] = "/dev/ttyUSB0";

  try {
    raspicam::RaspiCam_Cv Camera;
    cv::Mat rgb_image, bgr_image;
    if (!Camera.open()) {
      cerr << "Error opening the camera" << endl;
      return -1;
    }
    cout << "Opened Camera" << endl;
    SerialStream stream (serial_loc, LibSerial::SerialStreamBuf::BAUD_57600);
    cout << "Opened Serial Stream to" << serial_loc << endl;
    this_thread::sleep_for(chrono::milliseconds(1000));
    Create robot(stream);
    cout << "Created iRobot Object" << endl;
    robot.sendFullCommand();
    cout << "Setting iRobot to Full Mode" << endl;
    this_thread::sleep_for(chrono::milliseconds(1000));
    cout << "Robot is ready" << endl;

    // Let's stream some sensors.
    Create::sensorPackets_t sensors;
    sensors.push_back(Create::SENSOR_BUMPS_WHEELS_DROPS);
    sensors.push_back(Create::SENSOR_WALL_SIGNAL);
    sensors.push_back (Create::SENSOR_BUTTONS);
  

    robot.sendStreamCommand (sensors);
    cout << "Sent Stream Command" << endl;

    // Let's turn!
    int speed = 287;
    short wallSignal = 0;
    short prevWallSignal = 0;
    int count = 0;
    bool runSong = true;
    bool isWall = true;
    robot.sendDriveCommand (speed, Create::DRIVE_STRAIGHT);
    cout << "Sent Drive Command" << endl;
    std::thread song(playSong, std::ref(robot), std::ref(runSong), std::ref(isWall));

    while (!robot.playButton ())
    {

       if (robot.bumpLeft () || robot.bumpRight ()) {
            isWall = false;
            speed = 0;
            robot.sendDriveCommand(speed, Create::DRIVE_STRAIGHT);
            this_thread::sleep_for(chrono::milliseconds(15));
            
            
            cout << "Bump : " << count++ << endl;
            
            bool pictureTaken = false;
            
            std::thread leds(playLEDS, std::ref(robot), std::ref(pictureTaken));
            
            speed = -165;  
            robot.sendDriveCommand(speed, Create::DRIVE_STRAIGHT);
            cout << " Leaving bump ! Drive Command Sent!" << endl;
            this_thread::sleep_for(chrono::milliseconds(3000));
            speed = 0;
            robot.sendDriveCommand(speed, Create::DRIVE_STRAIGHT);
            Camera.grab();
            Camera.retrieve (bgr_image);
            cv::cvtColor(bgr_image, rgb_image, CV_RGB2BGR);
            pictureTaken = cv::imwrite("irobot_image.jpg", rgb_image);
            cout << "picture taken: " << pictureTaken << endl;
            leds.join();
            cout << " Ending LEDs" << endl;

            short randAngle = short ((rand() % 120) + 120);
            cout << "Angle : " <<  randAngle << endl;
            speed = 107;
            robot.sendDriveCommand(speed, randAngle);
            this_thread::sleep_for(chrono::milliseconds(3000));
            speed = 287;
            robot.sendDriveCommand(speed, Create::DRIVE_STRAIGHT);
            cout << " Leaving bump ! Drive Command Sent!" << endl;
            wallSignal = 0;
            prevWallSignal = 0;
            isWall = true;
            this_thread::sleep_for(chrono::milliseconds(15));

        }
      /**  
      wallSignal = robot.wallSignal();  
      if (wallSignal > 0 && prevWallSignal == 0) {
        prevWallSignal = wallSignal;
        isWall = true;
      }
      */
      

    }
    runSong = false;
    
    cout << "Play button pressed, stopping Robot" << endl;
    robot.sendDriveCommand (0, Create::DRIVE_STRAIGHT);
    song.join();
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

void playSong(Create& robot, bool& run, bool& wall ){
    cout << "In the Song thread" << endl;
    char songFreq = 64;
    Create::note_t note1;
    note1.first = 100;
    note1.second = 32;
    Create::note_t note2;
    note2.first = 30;
    note2.second = songFreq;

    while (run){
        while (wall){
            Create::song_t song;
            song.push_back(note2);
            song.push_back(note1);
            robot.sendSongCommand(1, song);
            
            this_thread::sleep_for(chrono::milliseconds(15));
            robot.sendPlaySongCommand(1);
            
        }
        this_thread::sleep_for(chrono::milliseconds(500));   
    }    
}

void playLEDS(Create& robot, bool& term){
    cout << " starting LEDs" << endl;
    while (!term){
        
        robot.sendLedCommand (Create::LED_PLAY, Create::LED_COLOR_GREEN, Create::LED_INTENSITY_FULL);
        this_thread::sleep_for(chrono::milliseconds(200));
        robot.sendLedCommand (Create::LED_ALL, 0, 0);
        this_thread::sleep_for(chrono::milliseconds(200));
        robot.sendLedCommand (Create::LED_ADVANCE, Create::LED_COLOR_RED, Create::LED_INTENSITY_FULL);
        this_thread::sleep_for(chrono::milliseconds(200));
        robot.sendLedCommand (Create::LED_PLAY, Create::LED_COLOR_RED, Create::LED_INTENSITY_FULL);
        this_thread::sleep_for(chrono::milliseconds(200));
        robot.sendLedCommand (Create::LED_ALL, 0, 0);
        this_thread::sleep_for(chrono::milliseconds(200));
        robot.sendLedCommand (Create::LED_ADVANCE, Create::LED_COLOR_GREEN, Create::LED_INTENSITY_FULL);
        this_thread::sleep_for(chrono::milliseconds(200));
    }   
}