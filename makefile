.ONESHELL:
CC = g++ -g
all: clean binary run
binary:
	$(CC) producer.cpp -o producer
	$(CC) consumer.cpp -o consumer
run: binary
	@ ./run.sh
clean:
	rm -f producer
	rm -f consumer