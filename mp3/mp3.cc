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
#include "vision.hh"

using namespace iRobot;
using namespace LibSerial;
using namespace std;


pthread_mutex_t mutex_robot = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t image_mutex = PTHREAD_MUTEX_INITIALIZER;

void *RobotMotion(void*);
void *RobotSafety(void*);
void *RobotVision(void*);
void *RobotImage(void*);

char serial_loc[] = "/dev/ttyUSB0";
SerialStream stream (serial_loc, LibSerial::SerialStreamBuf::BAUD_57600);
//cout << "Opened Serial Stream to" << serial_loc << endl;
//this_thread::sleep_for(chrono::milliseconds(1000));
Create robot(stream);

bool stop = false;
vector<Mat> images;

int main() {
    std::chrono::steady_clock::time_point progTimer0;
    std::chrono::steady_clock::time_point progTimer1;
    progTimer0 = std::chrono::steady_clock::now();
    try {
        cout << "Created iRobot Object" << endl;
        robot.sendFullCommand();
        cout << "Setting iRobot to Full Mode" << endl;
        cout << "Robot is ready" << endl;
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
        /**
        bool wheeldropright = false;
        bool wheeldropleft = false;
        bool wheeldropcaster = false;
        int cliff_left = 0;
        int cliff_right = 0;
        int cliff_frontleft = 0;
        int cliff_frontright = 0;
        bool leftwheelo = false;
        bool rightwheelo = false;
        while (true){
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
        this_thread::sleep_for(chrono::milliseconds(2000));
    }
    */
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
    paramMotion.sched_priority = 3;
    pthread_attr_setschedparam (&attrMotion, &paramMotion);

    pthread_attr_t attrVision;
    sched_param paramVision;
    pthread_attr_init (&attrVision);
    pthread_attr_getschedparam (&attrVision, &paramVision);
    paramVision.sched_priority = 2;
    pthread_attr_setschedparam (&attrVision, &paramVision);

    pthread_attr_t attrImage;
    sched_param paramImage;
    pthread_attr_init (&attrImage);
    pthread_attr_getschedparam (&attrImage, &paramImage);
    paramSafety.sched_priority = 2;
    pthread_attr_setschedparam (&attrImage, &paramImage);

    pthread_t thread_safety;
    pthread_create(&thread_safety, &attrSafety, RobotSafety, (void *)0);
    cout << "Safety Launced" << endl;

    pthread_t thread_motion;
    pthread_create(&thread_motion, &attrMotion, RobotMotion, (void *)0);
    cout << "Motion Launced" << endl;

    pthread_t thread_vision;
    pthread_create(&thread_vision, &attrVision, RobotVision, (void *)0);
    cout << "Camera Launced" << endl;

    pthread_t thread_image;
    pthread_create(&thread_image, &attrImage, RobotImage, (void *)0);
    cout << "Image Launced" << endl;

    progTimer1 = std::chrono::steady_clock::now();
    int progTime = std::chrono::duration_cast<std::chrono::milliseconds>(progTimer1-progTimer0).count();

    while (progTime < 120000) {
        progTime = std::chrono::duration_cast<std::chrono::milliseconds>(progTimer1-progTimer0).count();
    }

    stop = true;

    pthread_join(thread_motion, NULL);
    pthread_join(thread_safety, NULL);
    pthread_join(thread_vision, NULL);
    pthread_join(thread_image, NULL);

    // processImages(images);
    } catch (InvalidArgument& e) {
        cerr << e.what () << endl;
        return 3;
    } catch (CommandNotAvailable& e) {
        cerr << e.what () << endl;
        return 4;
    }
    return 0;
}

void *RobotMotion(void *x){
    robotMotion(std::ref(robot), &mutex_robot, std::ref(stop));
    cout << "END Motion!!!!!!!!!!!!!" << endl;
    pthread_exit(NULL);
}

void *RobotSafety(void *x){
    robotSafety(std::ref(robot), &mutex_robot, std::ref(stop));
    cout << "END Safety!!!!!!!!!!!!!" << endl;
    pthread_exit(NULL);
}

void *RobotVision(void *x){
    robotCamera(std::ref(robot), &mutex_robot, &image_mutex, std::ref(images), std::ref(stop));
    cout << "END Camera!!!!!!!!!!!!!" << endl;
    pthread_exit(NULL);
}

void *RobotImage(void *x) {
    robotImage(std::ref(robot), &mutex_robot, &image_mutex, std::ref(images), std::ref(stop));
    cout << "END Image!!!!!!!!!!!!!" << endl;
    pthread_exit(NULL);
}
