#fisier folosit pentru compilarea serverului concurent & clientului TCP

all:
	gcc server.c -o server -l sqlite3
	gcc Client.c -o client
clean:
	rm -f *~ client server
