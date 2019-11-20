#include "./object-identification/RobotIdentification.hh"
#include <dirent.h>
#include <raspicam/raspicam_cv.h>
#include "irobot-create.hh"
#include <pthread.h>
#include <thread>
#include <chrono>
#include <string>

using namespace iRobot;
using namespace std;

void robotImage(Create& robot, pthread_mutex_t *stream_mutex, pthread_mutex_t *image_mutex, vector<Mat>& images, bool& end);
void robotCamera(Create& robot, pthread_mutex_t *stream_mutex, pthread_mutex_t *image_mutex, vector<Mat>& images, bool& end);
