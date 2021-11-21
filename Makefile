all: schedule

schedule: schedule.c
	gcc schedule.c -o schedule

clean:
	rm -fr schedule *~ *.o   core*
