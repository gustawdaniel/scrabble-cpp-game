build:
	g++ -lncursesw -o main main.cpp

run: build
	./main

clean:
	$(RM) main