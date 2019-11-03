all: mp2

mp2: mp2.cc irobot-create.o robotmotion.o robotsafety.o robotcontour.o vision.o RobotIdentification.o
	g++ -std=c++11 -c mp2.cc
	g++ -std=c++11 -o mp2 mp2.o irobot-create.o robotmotion.o robotsafety.o vision.o robotcontour.o RobotIdentification.o -pthread -L/opt/vc/lib -lopencv_xfeatures2d -lopencv_calib3d -lopencv_features2d -lserial -lraspicam -lraspicam_cv -lmmal -lmmal_core -lmmal_util -lopencv_core -lopencv_imgcodecs -lopencv_highgui -lopencv_imgproc

irobot-create.o: irobot-create.cc
	g++ -std=c++11 -c irobot-create.cc

robotmotion.o: robotmotion.cc
	g++ -std=c++11 -c robotmotion.cc

vision.o: vision.cc
	g++ -std=c++11 -c vision.cc

robotsafety.o: robotsafety.cc
	g++ -std=c++11 -c robotsafety.cc

robotcontour.o: robotcontour.cc
	g++ -std=c++11 -c robotcontour.cc

RobotIdentification.o: ./object-identification/RobotIdentification.cc
	g++ -std=c++11 -c ./object-identification/RobotIdentification.cc

clean:
	rm -f mp2 *.o
