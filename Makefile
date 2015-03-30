# TempMod compile test

LDFLAGS = -L/usr/local/lib
LDLIBS    = -lwiringPi -lwiringPiDev -lpthread -lm -lsqlite3

all:
	gcc TempMod.c Max6675/Max6675.c DS1820/DS1820.c AM2302/AM2302.c PCD8544/PCD8544.c -o TempMod $(LDFLAGS) $(LDLIBS)

MonitorT:
	rm -rf MonitorT
	gcc MonitorHome.c AM2302/AM2302.c PCD8544/PCD8544.c -o MonitorT $(LDFLAGS) $(LDLIBS)

clean:
	rm -rf MonitorT
