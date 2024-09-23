main.exe: main.o route-records.o
	gcc main.o route-records.o -o main.exe
main.o: main.c route-records.h
	gcc -c main.c -o main.o
route-records.o: route-records.c route-records.h
	gcc -c route-records.c -o route-records.o
clean:
	rm -rf *.o
	rm main.exe