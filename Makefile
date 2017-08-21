TARGETS= get_img background-match background-match-roomba \
	 gray-match-test colour-match-test test gosa-histogram \

#PROG=move_test

OBJS=serial.o roombacomm.o

OS=$(shell uname | cut -d_ -f1)

ifneq ($(OS),Cygwin)
CC=g++
CFLAGS=`pkg-config opencv --cflags` -DOS=$(OS) -DCAP_PARAM=0
CXXFLAGS=$(CFLAGS)
LDFLAGS=`pkg-config opencv --libs`
LDLIBS += -lpthread
endif

ifeq ($(OS),CYGWIN)
CC=g++
CFLAGS=-I/usr/local/OpenCV2.1/include/opencv -DOS=$(OS) -DCAP_PARAM=CV_CAP_VFW
CXXFLAGS=$(CFLAGS)
LDFLAGS=-L/usr/local/OpenCV2.1/bin -lcv210d -lcxcore210d -lhighgui210d -lcvaux2#10d
endif


CFLAGS+=-g
CFLAGS+=-O

%: %.cpp
	$(LINK.cpp) $^ $(LOADLIBES) $(LDLIBS) -o $@ $(LDFLAGS)
%: %.c
	$(LINK.C) $^ $(LOADLIBES) $(LDLIBS) -o $@ $(LDFLAGS)


all: $(TARGETS) $(PROG)
	@chmod a-x *.c*

$(TARGETS): $(OBJS)
$(PROG): $(OBJS)

clean:
	rm -f $(TARGETS)
	rm -f *.o


os:
	echo OS is $(OS)
