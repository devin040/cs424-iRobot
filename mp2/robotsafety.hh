#include <pthread.h>
#include "irobot-create.hh"
#include <chrono>
#include <thread>
#include <iostream>
#include <ctime>


using namespace iRobot;
using namespace std;

void robotSafety(Create&, pthread_mutex_t&, bool&);
