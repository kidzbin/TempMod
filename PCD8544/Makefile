# DetectT compile test

LDFLAGS = -L/usr/local/lib
LDLIBS    = -lwiringPi -lwiringPiDev -lpthread -lm

all:
	gcc PCD8544Test.c PCD8544.c -o PCD8544 $(LDFLAGS) $(LDLIBS)

clean:
	rm -f LCDx2