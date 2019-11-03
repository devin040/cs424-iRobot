#include "RobotIdentification.hh"
#include "RobotIdentification.cc"
#include <dirent.h>

using namespace cv;
using namespace cv::xfeatures2d;
using namespace std;

using std::chrono::duration;
using std::chrono::steady_clock;
using std::cout;
using std::endl;
using std::string;
using std::vector;
int counterimage = 0;
vector<Mat> images;
int index= 0;
Mat bgr_image;



void vision (Create& robot, pthread_mutex_t* stream_mutex, bool& end)
{

  while (!end && counterimage < 10){
    pthread_mutex_lock(stream_mutex);
    speed = 50;
    robot.sendDriveCommand(speed, Create::DRIVE_STRAIGHT);
    this_thread::sleep_for(chrono::milliseconds(100));
    Camera.grab();
    Camera.retrieve (bgr_image);

    //concatenating the name of the image with the number to do not replace files


    images.push_back(bgr_image);

    pthread_mutex_unlock(stream_mutex); //NOT SURE ABOUT ARGUMENT
    this_thread::sleep_for(std::chrono::milliseconds(1000));
    counterimage++;
  }
}



////TO ADD IN THE MAIN CODE
int main()
{
while(index != counterimage)
{

img_scene = images[index]
index= index+ 1
runIdentify(img_scene) ///From ObjectIdentification

}

}
