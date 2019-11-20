#include "vision.hh"
/*
 * from mp2

void processImages(vector<Mat> images) {
    cout << images.size() << " images taken." << endl;
    cout << "Starting processImages" << endl;
    RobotIdentification test;
    for (int i = 0; i < images.size(); i++) {
        cout << "runIdentify on Image #" << i << endl;
        test.runIdentify(images[i]);
    }
    cout << "Ending processImages" << endl;
}
*/

void robotImage(Create& robot, pthread_mutex_t *stream_mutex, pthread_mutex_t *image_mutex, vector<Mat>& images, bool& end) {
    bool usedWeapon = false;
    RobotIdentification test;
    vector<Mat> rest_images;
    while (!end) {
        this_thread::sleep_for(std::chrono::milliseconds(500));
        if (!usedWeapon) {
            if (images.size() > 0) {
                pthread_mutex_lock(image_mutex);
                Mat image = images[0];
                images.erase(images.begin());
                pthread_mutex_unlock(image_mutex);
                cout << "Comparing to Magic Lamp..." << endl;
                if (test.isMagicLamp(image)) {
                    pthread_mutex_lock(stream_mutex);
                    robot.sendLedCommand(Create::LED_NONE, Create::LED_COLOR_RED, Create::LED_INTENSITY_FULL);
                    this_thread::sleep_for(chrono::milliseconds(2000));
                    robot.sendLedCommand(Create::LED_NONE, 0, 0);
                    pthread_mutex_unlock(stream_mutex);
                    usedWeapon = true;
                }
                rest_images.push_back(image);
            } else {
                if (rest_images.size() > 0) {
                    cout << "Comparing to other objects..." << endl;
                    Mat image = rest_images[0];
                    rest_images.erase(rest_images.begin());
                    test.runIdentify(image);
                }
            }
        } else {
            if (rest_images.size() > 0) {
                cout << "Lamp Found; Comparing to other objects..." << endl;
                Mat image = rest_images[0];
                rest_images.erase(rest_images.begin());
                test.runIdentify(image);
            } else {
                if (images.size() > 0) {
                    cout << "Lamp Found; Comparing to other objects..." << endl;
                    pthread_mutex_lock(image_mutex);
                    Mat image = images[0];
                    images.erase(images.begin());
                    pthread_mutex_unlock(image_mutex);
                    test.runIdentify(image);
                }
            }
        }

    }
}

void robotCamera(Create& robot, pthread_mutex_t *stream_mutex, pthread_mutex_t *image_mutex, vector<Mat>& images, bool& end) {
    raspicam::RaspiCam_Cv Camera;
    if (!Camera.open()) {
     cerr << "Error opening the camera" << endl;
     return;
    }
    cout << "Opened Camera" << endl;

    int count = 1;

    while (!end){
        this_thread::sleep_for(std::chrono::milliseconds(300));
        Mat bgr_image;
        pthread_mutex_lock(stream_mutex);
        Camera.grab();
        Camera.retrieve(bgr_image);
        pthread_mutex_unlock(stream_mutex);
        pthread_mutex_lock(image_mutex);
        images.push_back(bgr_image);
        pthread_mutex_unlock(image_mutex);
        cv::imwrite("irobot_image" + to_string(count++) + ".jpg", bgr_image);
        cout << "Image count: " << count << endl;
    }
}
