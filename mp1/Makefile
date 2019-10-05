all: mp1

<<<<<<< HEAD
mp1: mp1.cc irobot-create.o 
=======
irobot-create.o: mp1.cc irobot-create.o
>>>>>>> 73a08a93aa691f6c130f68be2e4f7ee51cca6afc
	g++ -std=c++11 -c mp1.cc
	g++ -std=c++11 -o mp1 mp1.o irobot-create.o -pthread -L/opt/vc/lib -lserial -lraspicam -lraspicam_cv -lmmal -lmmal_core -lmmal_util -lopencv_core -lopencv_imgcodecs -lopencv_highgui -lopencv_imgproc

irobot-create.o: irobot-create.cc
	g++ -std=c++11 -c irobot-create.cc

clean:
	rm -f mp1 *.o
