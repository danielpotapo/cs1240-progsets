# Detect OS
ifeq ($(OS),Windows_NT)
    EXECUTABLE = strassen.exe
    RUN_PREFIX = .\
    RM = del /Q
else
    EXECUTABLE = strassen
    RUN_PREFIX = ./
    RM = rm -f
endif

# Default target
$(EXECUTABLE): strassen.cpp
	g++ -std=c++17 strassen.cpp -o $(EXECUTABLE)

# Alias so 'make strassen' works on grading server
strassen: $(EXECUTABLE)

# Capture any arguments after "run"
ifeq (run,$(firstword $(MAKECMDGOALS)))
  RUN_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
  $(eval $(RUN_ARGS):;@:)
endif

run: $(EXECUTABLE)
	$(RUN_PREFIX)$(EXECUTABLE) $(RUN_ARGS)

clean:
	$(RM) $(EXECUTABLE)

.PHONY: run clean strassen