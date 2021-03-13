CXXFLAGS = -std=c++17
STATIC_CHECK_FLAGS = -Wall -Wextra -pedantic
OBJS = main.o


%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<


main: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

debug: $(OBJS)
	$(CXX) $(CXXFLAGS) $(STATIC_CHECK_FLAGS) -g -o $@ $^


.PHONY: clean

clean:
	rm *.o main debug

