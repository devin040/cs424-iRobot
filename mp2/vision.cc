#include "vision.hh"

void processImages(vector<Mat> images) {
    int counterimage = 0;
    RobotIdentification test;
    for (int i = 0; i < images.size(); i++) {
        test.runIdentify(images[i]);
    }
}

void robotCamera(Create& robot, raspicam::RaspiCam_Cv Camera, pthread_mutex_t *stream_mutex, vector<Mat> images, bool& end) {
    while (!end){
        pthread_mutex_lock(stream_mutex);
        int speed = 50;
        robot.sendDriveCommand(speed, Create::DRIVE_STRAIGHT);
        pthread_mutex_unlock(stream_mutex);
        this_thread::sleep_for(std::chrono::milliseconds(2000));
        Mat bgr_image;
        Camera.grab();
        Camera.retrieve (bgr_image);
        images.push_back(bgr_image);
        cout << "Took picture!" << endl;
    }
}
