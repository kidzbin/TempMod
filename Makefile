# TempMod compile test

all:
	gcc TempMod.c DS1820/DS1820.c AM2302/AM2302.c -o TempMod -lwiringPi -lsqlite3

clean:
	rm -f TempMod
