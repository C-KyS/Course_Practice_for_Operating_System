all: mypipe mysize
mypipe:mypipe.cc
	g++ -o $@ $^
mysize:pipesize.cc
	g++ -o $@ $^	

.PHONY: clean
clean:
	rm -f mypipe mysize