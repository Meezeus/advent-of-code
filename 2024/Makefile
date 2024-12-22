CXX = g++
CXXFLAGS = -std=c++20 -g -Wall -Wextra
INCLUDE_PATHS = -I.

SHARED_HDR = Helper.h
SHARED_SRC = $(SHARED_HDR:.h=.cpp)
SHARED_OBJ = $(SHARED_SRC:.cpp=.o)


DAY_NUMBERS = 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25
$(info DAY_NUMBERS: $(DAY_NUMBERS))

# day1, day2, etc.
DAYS = $(addprefix day, $(DAY_NUMBERS))
$(info DAYS: $(DAYS))

$(info )


PART_NUMBERS = 1 2
$(info PART_NUMBERS: $(PART_NUMBERS))

# part1, part2, etc.
PARTS = $(addprefix part, $(PART_NUMBERS))
$(info PARTS: $(PARTS))

$(info )


# day1/part1, day1/part2, day2/part1, day2/part2, etc.
PATHS = $(foreach day, $(DAYS), $(foreach part, $(PARTS), $(day)/$(part)))
$(info PATHS: $(PATHS))

$(info )


# day1/part1/Solution.o, day1/part2/Solution.o, day2/part1/Solution.o, day2/part2/Solution.o, etc.
OBJ = $(addsuffix /Solution.o, $(PATHS))
$(info OBJ: $(OBJ))

# day1/part1/Solution, day1/part2/Solution, day2/part1/Solution, day2/part2/Solution, etc.
EXECUTABLES = $(addsuffix /Solution.exe, $(PATHS))
$(info EXECUTABLES: $(EXECUTABLES))

$(info )


all: $(EXECUTABLES)

# Rule to link object files for a specific day and part, plus the shared object
# files, into an executable
$(EXECUTABLES): %.exe: %.o $(SHARED_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Rule to compile object files for a specific day and part
$(OBJ): %.o: %.cpp $(SHARED_HDR)
	$(CXX) $(CXXFLAGS) $(INCLUDE_PATHS) -c $< -o $@

# Rule to compile shared object files
$(SHARED_OBJ): $(SHARED_SRC) $(SHARED_HDR)
	$(CXX) $(CXXFLAGS) -c $< -o $@


ANSIEscapeCodes: ANSIEscapeCodes.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@.exe


# Rule to remove all executables and object files
clean:
	rm -f $(EXECUTABLES) $(OBJ) $(SHARED_OBJ)
