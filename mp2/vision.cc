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
vector<Mat> images
int intdex= 0


try {
  raspicam::RaspiCam_Cv Camera;
  cv::Mat rgb_image, bgr_image;
  if (!Camera.open()) {
    cerr << "Error opening the camera" << endl;
    return -1;
}

bool vision::vision ()
{

  pthread_mutex_lock(stream_mutex); /// NOT SURE ABOUT ARGUMENT
  speed = 50;
  robot.sendDriveCommand(speed, Create::DRIVE_STRAIGHT);
  this_thread::sleep_for(chrono::milliseconds(100));
  Camera.grab();
  Camera.retrieve (bgr_image);

  //concatenating the name of the image with the number to do not replace files
  std::string imagename = "irobot_image " + std::to_string(counterimage)

  camera.imwrite(imagename, bgr_image)
  images.push_back(img_scene)

  pthread_mutex_unlock(stream_mutex); //NOT SURE ABOUT ARGUMENT

  counterimage= 1+ counterimage
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
