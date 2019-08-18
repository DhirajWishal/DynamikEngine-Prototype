.SUFFIXES: .cpp 
CXX=/usr/local/gcc4/bin/g++
#CXX=g++
CXX_FLAGS=-Wall -O2 -DNDEBUG

BOOST_INCLUDE_PATH=-I../../../..
LIBJPEG_INCLUDE_PATH=-I../../../../boost/gil/lib/libjpeg
LIBJPEG_LIB_PATH=-L../../../../boost/gil/lib/libjpeg

all: numeric
.cpp.o:
	${CXX} ${CXX_FLAGS} ${BOOST_INCLUDE_PATH} ${LIBJPEG_INCLUDE_PATH} -c $<
clean:
	-rm -f *.o *.exe 
numeric:	kernel.o main.o resample.o convolve.o
	${CXX} -o numeric ${CXX_FLAGS} kernel.o main.o resample.o convolve.o ${LIBJPEG_LIB_PATH} -ljpeg
