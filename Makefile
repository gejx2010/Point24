objects = ComputePoint.h ComputePoint.cpp main.cpp
main: $(objects)
	g++ -o main $(objects)
clean:
	rm -f main
