# DetectT compile test

LDFLAGS = -L/usr/local/lib
LDLIBS    = -lwiringPi -lwiringPiDev -lpthread -lm

all:
	gcc LCDx2.c -o LCDx2 $(LDFLAGS) $(LDLIBS)

clean:
	rm -f LCDx2