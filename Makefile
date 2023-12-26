TARGET=main
OBJ=main.o PCBManager.o
HEADERS=PCB.hpp PCBManager.h interface.hpp
$(TARGET) : $(OBJ)
	g++ -o $@ $^ -std=c++11 -lpthread

%.o : %.cc $(HEADERS)
	g++ -o $@ -c $< -std=c++11 -lpthread

.PHONY:clean
clean:
	rm -rf $(TARGET) *.o