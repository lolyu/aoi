CXX = g++-10
# CXXFLAGS = -Wall -Werror -Wextra -pedantic -std=c++20 -g
# LDFLAGS =  -fsanitize=address
CXXFLAGS = -std=c++20 -g -pthread

SRC = LogStream.cpp LogFile.cpp main.cpp
OBJ = $(SRC:.cpp=.o)
EXEC = main

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) $(LDFLAGS) -o $@ $(OBJ) $(LBLIBS) -std=c++20 -pthread

clean:
	rm -rf $(OBJ) $(EXEC)