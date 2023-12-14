# Variables
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17
CLIENT_TARGET = client
SERVER_TARGET = server

# Client and Server Sources
CLIENT_SRCS = client.cpp
SERVER_SRCS = server.cpp

# Object Files
CLIENT_OBJS = $(CLIENT_SRCS:.cpp=.o)
SERVER_OBJS = $(SERVER_SRCS:.cpp=.o)

# Default target
all: $(CLIENT_TARGET) $(SERVER_TARGET)

# Client build rules
$(CLIENT_TARGET): $(CLIENT_OBJS)
	$(CXX) $(CXXFLAGS) -o $(CLIENT_TARGET) $(CLIENT_OBJS)

# Server build rules
$(SERVER_TARGET): $(SERVER_OBJS)
	$(CXX) $(CXXFLAGS) -o $(SERVER_TARGET) $(SERVER_OBJS)

# Generic rule for object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

# Clean up
clean:
	rm -f $(CLIENT_OBJS) $(SERVER_OBJS) $(CLIENT_TARGET) $(SERVER_TARGET)
