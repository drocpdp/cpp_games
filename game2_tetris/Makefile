clean:
	rm *.exe
	rm -rf bin

test:
	rm -rf bin
	mkdir -p bin
	g++ -std=c++11 -c main.cpp
	g++ main.o -o ./bin/main.exe -lsfml-graphics -lsfml-window -lsfml-system
	./bin/main.exe
