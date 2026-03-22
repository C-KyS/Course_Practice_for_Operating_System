all: myshell cmd1 cmd2 cmd3

myshell: main.cc
	g++ -o $@ $^
cmd1: cmd1.cc
	g++ -o $@ $^
cmd2: cmd2.cc
	g++ -o $@ $^
cmd3: cmd3.cc
	g++ -o $@ $^

.PHONY: all clean
clean:
	rm -f myshell cmd1 cmd2 cmd3