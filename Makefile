CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
TARGET   = queuing_sim
SRCDIR   = src
SRCS     = $(SRCDIR)/main.cpp \
           $(SRCDIR)/customerType.cpp \
           $(SRCDIR)/serverType.cpp \
           $(SRCDIR)/serverListType.cpp \
           $(SRCDIR)/validation.cpp \
           $(SRCDIR)/errorHandler.cpp

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET) simulation_log.txt

.PHONY: all clean
