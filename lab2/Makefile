APP=$(shell ls | grep cpp | sed s/.cpp//g)
all: $(APP)

$(APP): $(APP).o
	g++ $(APP).o -o $(APP) `pkg-config --libs opencv`

$(APP).o: $(APP).cpp
	g++ -std=c++11 `pkg-config --cflags opencv` $(APP).cpp -c

clean:
	rm -f $(APP