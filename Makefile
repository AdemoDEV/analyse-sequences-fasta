CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -D_GNU_SOURCE -Werror=all -O2 -Iinclude

TARGET := compare_strings

SRCS := compare_strings.cpp top.cpp region.cpp utils_fasta.cpp utils_csv.cpp

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)
run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all clean