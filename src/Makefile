SRC=$(wildcard *.cc)

all: main

main:*.cc
	g++ -O2 -std=c++0x -g -o main $(SRC)

test:main
	./main dd2380.csc.kth.se $(PORT) $(BOARD)

to_submit.zip:*.h *.cc
	mkdir to_submit
	cp $^ to_submit
	zip $@ to_submit/*
	rm -r to_submit

clean:
	rm main
	rm *.o
