CXX      = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -pedantic

TARGET = biblioteka
SRCS   = main.cpp functions.cpp
OBJS   = $(SRCS:.cpp=.o)

# linkowanie plikow .o w jeden plik wykonywalny
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^
	@echo "  --> Linkowanie: $(OBJS) => $(TARGET)"

# kompilacja kazdego .cpp osobno do pliku .o
%.o: %.cpp functions.h
	$(CXX) $(CXXFLAGS) -c $<
	@echo "  --> Skompilowano: $< => $@"

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
