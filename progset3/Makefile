# Detect OS
ifeq ($(OS),Windows_NT)
    EXECUTABLE = partition.exe
    RUN_PREFIX = .\
    RM = del /Q
else
    EXECUTABLE = partition
    RUN_PREFIX = ./
    RM = rm -f
endif

# Default target
$(EXECUTABLE): partition.cpp
	g++ -std=c++17 partition.cpp -o $(EXECUTABLE)

# Alias so 'make partition' works on grading server
partition: $(EXECUTABLE)

# Capture any arguments after "run"
ifeq (run,$(firstword $(MAKECMDGOALS)))
  RUN_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
  $(eval $(RUN_ARGS):;@:)
endif

run: $(EXECUTABLE)
	$(RUN_PREFIX)$(EXECUTABLE) $(RUN_ARGS)

clean:
	$(RM) $(EXECUTABLE)

.PHONY: run clean partition