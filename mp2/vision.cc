#include "./object-identification/RobotIdentification.hh"
#include <dirent.h>

using namespace cv;
using namespace cv::xfeatures2d;
using namespace std;

void processImages(vector<Mat>);
void vision (Create&, raspicam::RaspiCam_Cv, pthread_mutex_t *, vector<Mat>, bool&);

void processImages(vector<Mat> images) {
    int counterimage = 0;
    Mat bgr_image;
    RobotIdentification test;
    for (int i = 0; i < images.size(); i++) {
        test.runIdentify(images[i]);
    }
}

void robotCamera(Create& robot, raspicam::RaspiCam_Cv Camera, pthread_mutex_t *stream_mutex, vector<Mat> images, bool& end) {
    while (!end){
        pthread_mutex_lock(stream_mutex);
        speed = 50;
        robot.sendDriveCommand(speed, Create::DRIVE_STRAIGHT);
        pthread_mutex_unlock(stream_mutex);
        this_thread::sleep_for(std::chrono::milliseconds(2000));
        Camera.grab();
        Camera.retrieve (bgr_image);
        images.push_back(bgr_image);
    }
}
