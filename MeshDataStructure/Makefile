CC := g++
FLAGS := -std=c++11 -w

main: Mesh.o DataStructGMsh.o ExecManager.o FileManager.o User.o
	$(CC) $(FLAGS) $^ -o main

DataStructGMsh.o: DataStructGMsh.cpp DataStructGMsh.hpp
	$(CC) $(FLAGS) -c $< 

ExecManager.o: ExecManager.cpp ExecManager.hpp
	$(CC) $(FLAGS) -c $<

FileManager.o: FileManager.cpp ExecManager.hpp
	$(CC) $(FLAGS) -c $<

User.o: User.cpp User.hpp
	$(CC) $(FLAGS) -c $<

Mesh.o: Mesh.cpp
	$(CC) $(FLAGS) -c $<

clean:
	@rm -f *.o
	@rm -f *.gch
	@rm -f main
