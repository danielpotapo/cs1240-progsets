# Detect OS
ifeq ($(OS),Windows_NT)
    EXECUTABLE = randmst.exe
    RUN_PREFIX = .\
    RM = del /Q
else
    EXECUTABLE = randmst
    RUN_PREFIX = ./
    RM = rm -f
endif

# Default target
$(EXECUTABLE): randmst.cpp
	g++ -std=c++17 randmst.cpp -o $(EXECUTABLE)

# Alias so 'make randmst' works on grading server
randmst: $(EXECUTABLE)

# Capture any arguments after "run"
ifeq (run,$(firstword $(MAKECMDGOALS)))
  RUN_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
  $(eval $(RUN_ARGS):;@:)
endif

run: $(EXECUTABLE)
	$(RUN_PREFIX)$(EXECUTABLE) $(RUN_ARGS)

clean:
	$(RM) $(EXECUTABLE)

.PHONY: run clean randmst