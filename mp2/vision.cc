#include "vision.hh"

void processImages(vector<Mat> images) {
    cout << "Starting processImages" << endl;
    RobotIdentification test;
    cout << "image size " << images.size() << endl;
    for (int i = 0; i < images.size(); i++) {
        cout << "Image #" << i << endl;
        test.runIdentify(images[i]);
    }
    cout << "Ending processImages" << endl;
}

void robotCamera(Create& robot, pthread_mutex_t *stream_mutex, vector<Mat> images, bool& end) {
    raspicam::RaspiCam_Cv Camera;
    if (!Camera.open()) {
     cerr << "Error opening the camera" << endl;
     return -1;
    }
    cout << "Opened Camera" << endl;
    
    while (!end){
        // pthread_mutex_lock(stream_mutex);
        // int speed = 50;
        // robot.sendDriveCommand(speed, Create::DRIVE_STRAIGHT);
        // pthread_mutex_unlock(stream_mutex);
        this_thread::sleep_for(std::chrono::milliseconds(2000));
        Mat bgr_image;
        Camera.grab();
        Camera.retrieve(bgr_image);
        images.push_back(bgr_image);
        cout << "image size " << images.size() << endl;
        cout << "Took picture!" << endl;
    }
}
