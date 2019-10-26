#include <SerialStream.h>
#include "irobot-create.hh"
#include <ctime>
#include <iostream>
#include <chrono>
#include <thread>
#include <raspicam/raspicam_cv.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <pthread.h>
#include "robotmotion.hh"
#include "robotsafety.hh"

using namespace iRobot;
using namespace LibSerial;
using namespace std;


pthread_mutex_t mutex_robot;
void *RobotMotion(void*);
void *RobotSafety(void*);

char serial_loc[] = "/dev/ttyUSB0";
SerialStream stream (serial_loc, LibSerial::SerialStreamBuf::BAUD_57600);
//cout << "Opened Serial Stream to" << serial_loc << endl;
//this_thread::sleep_for(chrono::milliseconds(1000));
Create robot(stream);
bool stop = false;



int main ()
{
  
  try {
    //raspicam::RaspiCam_Cv Camera;
    //cv::Mat rgb_image, bgr_image;
    //if (!Camera.open()) {
    //  cerr << "Error opening the camera" << endl;
    //  return -1;
    //}
    //cout << "Opened Camera" << endl;

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
    cout << "Sensors Pushed" << endl;
    this_thread::sleep_for(chrono::milliseconds(1000));
    
    Create::song_t song;
    song.push_back(Create::note_t(100, 8));
    song.push_back(Create::note_t(90, 8));
    robot.sendSongCommand(1,song);

    pthread_attr_t attrSafety;
		sched_param paramSafety;
		pthread_attr_init (&attrSafety);
		pthread_attr_getschedparam (&attrSafety, &paramSafety);
		paramSafety.sched_priority = 4;
		pthread_attr_setschedparam (&attrSafety, &paramSafety); 
    
		pthread_attr_t attrMotion;
		sched_param paramMotion;
		pthread_attr_init (&attrMotion);
		pthread_attr_getschedparam (&attrMotion, &paramMotion);
		paramMotion.sched_priority = 4;
		pthread_attr_setschedparam (&attrMotion, &paramMotion); 

    pthread_t thread_motion;
		pthread_create(&thread_motion, &attrMotion, RobotMotion, (void *)0);
    cout << "Motion Launced" << endl;  

    pthread_t thread_safety;
    pthread_create(&thread_motion, &attrSafety, RobotSafety, (void *)0);
    cout << "Safety Launced" << endl;


    pthread_join(thread_motion, NULL);
    pthread_join(thread_safety, NULL);

    
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

void *RobotMotion(void *x){
    robotMotion(std::ref(robot), std::ref(mutex_robot), std::ref(stop));
    cout << "END MOTION!!!!!!!!!!!!!" << endl;
    pthread_exit(NULL);
}

void *RobotSafety(void *x){
    robotSafety(std::ref(robot), std::ref(mutex_robot), std::ref(stop));
    cout << "END Safety!!!!!!!!!!!!!" << endl;
    pthread_exit(NULL);
}
