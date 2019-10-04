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

void playSong(Create&, short);
void playLEDS();

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
    sensors.push_back(Create::SENSOR_DISTANCE);

    robot.sendStreamCommand (sensors);
    cout << "Sent Stream Command" << endl;
    // Let's turn!
    int speed = 287;

    robot.sendDriveCommand (speed, Create::DRIVE_STRAIGHT);
    cout << "Sent Drive Command" << endl;

    short wallSignal, prevWallSignal = 0;
    while (!robot.playButton ())
    {
       if (robot.bumpLeft () || robot.bumpRight ()) {
        cout << "Bump !" << endl;
        robot.sendDriveCommand(0, Create::DRIVE_STRAIGHT);
        std::thread leds(playLEDS, &robot);
        short d = robot.distance();
        while (d < 381 ){
            robot.sendDriveCommand(-165, Create::DRIVE_STRAIGHT);
            d += robot.distance();
        }
        robot.sendDriveCommand(0,Create::DRIVE_STRAIGHT);
        Camera.grab();
        Camera.retrieve (bgr_image);
        cv::cvtColor(bgr_image, rgb_image, CV_RGB2BGR);
        cv::imwrite("irobot_image.jpg", rgb_image);
        leds.join();

        short randAngle = short (rand() % 120 + 120);
        robot.sendDriveCommand(107, randAngle);
        robot.sendDriveCommand(speed, Create::DRIVE_STRAIGHT);

        }

      short wallSignal = robot.wallSignal();
      if (wallSignal > 0) {
        cout << "Wall signal " << robot.wallSignal() << endl;
        playSong(robot, wallSignal);
      }

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

void playSong(Create& robot, short wallsensorvalue){
    Create::note_t note1;
    note1.first = 64;
    note1.second = 16;
    Create::note_t note2;
    note2.first = 64;
    note2.second = (char) wallsensorvalue;
    Create::song_t song;
    song.push_back(note1);
    song.push_back(note2);
    robot.sendSongCommand(1, song);
    robot.sendPlaySongCommand(1);
}

void playLEDS(Create *robot){
    

    (*robot).sendLedCommand (Create::LED_PLAY, Create::LED_COLOR_GREEN, Create::LED_INTENSITY_FULL);
    this_thread::sleep_for(chrono::milliseconds(200));
    (*robot).sendLedCommand (Create::LED_ALL, 0, 0);
    this_thread::sleep_for(chrono::milliseconds(200));
    (*robot).sendLedCommand (Create::LED_ADVANCE, Create::LED_COLOR_RED, Create::LED_INTENSITY_FULL);
    this_thread::sleep_for(chrono::milliseconds(200));
    (*robot).sendLedCommand (Create::LED_PLAY, Create::LED_COLOR_RED, Create::LED_INTENSITY_FULL);
    this_thread::sleep_for(chrono::milliseconds(200));
    (*robot).sendLedCommand (Create::LED_ALL, 0, 0);
    this_thread::sleep_for(chrono::milliseconds(200));
    (*robot).sendLedCommand (Create::LED_ADVANCE, Create::LED_COLOR_GREEN, Create::LED_INTENSITY_FULL);
    this_thread::sleep_for(chrono::milliseconds(200));
       
}