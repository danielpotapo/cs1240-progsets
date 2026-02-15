progset1.exe: progset1.cpp
	g++ -std=c++17 progset1.cpp -o progset1.exe

run: progset1.exe
	.\progset1.exe
