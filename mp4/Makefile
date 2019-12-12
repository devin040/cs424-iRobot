all: mp4

mp4: mp4.cc irobot-create.o robotmotion.o robotsafety.o
	g++ -std=c++11 -c mp4.cc
	g++ -std=c++11 -o mp4 mp4.o irobot-create.o robotmotion.o robotsafety.o -pthread -L/opt/vc/lib -lopencv_xfeatures2d -lopencv_calib3d -lopencv_features2d -lserial -lraspicam -lraspicam_cv -lmmal -lmmal_core -lmmal_util -lopencv_core -lopencv_imgcodecs -lopencv_highgui -lopencv_imgproc

irobot-create.o: irobot-create.cc
	g++ -std=c++11 -c irobot-create.cc

robotmotion.o: robotmotion.cc
	g++ -std=c++11 -c robotmotion.cc

robotsafety.o: robotsafety.cc
	g++ -std=c++11 -c robotsafety.cc


clean:
	rm -f mp4 *.o
