.PHONY:clean

CC = g++
CFLAGS = -Wall -g
OBJ = main.o

main: $(OBJ)
	$(CC) $(CFLAGS) $^ -o build/campus-navigation

.cc.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf *.o test/* build/*