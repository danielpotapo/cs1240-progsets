progset1.exe: progset1.cpp
	g++ -std=c++17 progset1.cpp -o progset1.exe

# Capture any arguments after "run"
ifeq (run,$(firstword $(MAKECMDGOALS)))
  RUN_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
  $(eval $(RUN_ARGS):;@:)
endif

run: progset1.exe
	.\progset1.exe $(RUN_ARGS)