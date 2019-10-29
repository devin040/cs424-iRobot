#include <pthread.h>
#include "irobot-create.hh"
#include <chrono>
#include <thread>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
#include <math.h>

Point2f getImageCoordinates(Point2f);

Point2f getNextPosition(Point2f, float, float);

void robotContour(vector<float>, vector<float>;
