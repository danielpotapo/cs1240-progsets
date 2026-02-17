randmst.exe: randmst.cpp
	g++ -std=c++17 randmst.cpp -o randmst.exe

# Capture any arguments after "run"
ifeq (run,$(firstword $(MAKECMDGOALS)))
  RUN_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
  $(eval $(RUN_ARGS):;@:)
endif

run: randmst.exe
	.\randmst.exe $(RUN_ARGS)