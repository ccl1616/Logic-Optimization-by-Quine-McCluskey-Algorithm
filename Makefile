# sample Makefile

RES = src/main.cpp
EXE = pa1
all:
	g++ -std=c++11 $(RES) -o $(EXE)

clean:
	rm $(EXE)
	rm *.out
	
# RES = src/main.cpp
# EXE = pa1
# all:
# 	g++ -std=c++11 $(RES) -o $(EXE)

# clean:
# 	rm $(EXE)