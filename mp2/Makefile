all: mp2

mp1: mp2.cc irobot-create.o
	g++ -std=c++11 -c mp2.cc
	g++ -std=c++11 -o mp2 mp2.o irobot-create.o -pthread -L/opt/vc/lib -lserial -lraspicam -lraspicam_cv -lmmal -lmmal_core -lmmal_util -lopencv_core -lopencv_imgcodecs -lopencv_highgui -lopencv_imgproc

irobot-create.o: irobot-create.cc
	g++ -std=c++11 -c irobot-create.cc

clean:
	rm -f mp2 *.o
