all: mp1

mp1: mp1.cc irobot-create.o
	g++ -std=c++11 -c mp1.cc
	g++ -std=c++11 -o mp1 mp1.o irobot-create.o -pthread -L/opt/vc/lib -lserial -lraspicam -lraspicam_cv -lmmal -lmmal_core -lmmal_util -lopencv_core -lopencv_imgcodecs -lopencv_highgui -lopencv_imgproc

irobot-create.o: irobot-create.cc
	g++ -std=c++11 -c irobot-create.cc

clean:
	rm -f mp1 *.o
