#include <pthread.h>
#include "irobot-create.hh"
#include <chrono>
#include <thread>
#include <iosteam>

Poin2f getImageCoordinates(Point2f);

Point2f getNextPosition(Point2f, float, float);

void robotContour(vector<float>, vector<float>;