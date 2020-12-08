APP=$(shell ls | grep cpp | sed s/.cpp//g)
all: $(APP)

$(APP): $(APP).o
	g++ $(APP).o -o $(APP)

$(APP).o: $(APP).cpp
	g++ -std=c++14  $(APP).cpp -c

clean:
	rm -f $(APP