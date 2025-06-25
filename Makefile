.PHONY : clean all

all : run.exe
  echo "all done."

run.exe : unit.o terrain.o field.o actions.o algorithms.o engine.o 
  g++ unit.o terrain.o field.o actions.o algorithms.o engine.o -o run.exe

unit.o : unit.cpp
  g++ -c unit.cpp

terrain.o : terrain.cpp
  g++ -c terrain.cpp

field.o : field.cpp
  g++ -c field.cpp

actions.o : actions.cpp
  g++ -c actions.cpp

engine.o : engine.cpp
  g++ -c engine.cpp

algorithms.o : algorithms.cpp
  g++ -c algorithms.cpp

main.o : main.cpp
  g++ -c main.cpp
