#include "./object-identification/RobotIdentification.hh"
#include <dirent.h>
#include <raspicam/raspicam_cv.h>
#include "irobot-create.hh"
#include <pthread.h>
#include <thread>
#include <chrono>

using namespace iRobot;
using namespace std;

void processImages(vector<Mat> images);
void robotCamera(Create& robot, raspicam::RaspiCam_Cv Camera, pthread_mutex_t *stream_mutex, vector<Mat> images, bool& end);
