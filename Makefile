# This is the makefile for the compiler
compiler: main.o ASTNode.o Compiler.o File.o TokenLexer.o TokenSegment.o Token.o Module.o fail.o ASTTree.o parsing.o
	clang++ -o Compiler  main.o ASTNode.o Compiler.o File.o TokenLexer.o TokenSegment.o Token.o Module.o fail.o ASTTree.o parsing.o  -g
main.o: src/Program/main.cpp
	clang++ -c -o main.o src/Program/main.cpp --std=c++1y  -g
ASTNode.o: src/Structures/ASTNode.cpp Include/ASTNode.h
	clang++ -c -o ASTNode.o src/Structures/ASTNode.cpp --std=c++1y  -g
Compiler.o: src/Structures/Compiler.cpp Include/Compiler.h
	clang++ -c -o Compiler.o src/Structures/Compiler.cpp --std=c++1y  -g
File.o: src/Structures/File.cpp Include/File.h
	clang++ -c -o File.o src/Structures/File.cpp --std=c++1y  -g
TokenLexer.o: src/Structures/TokenLexer.cpp Include/TokenLexer.h
	clang++ -c -o TokenLexer.o src/Structures/TokenLexer.cpp --std=c++1y  -g
TokenSegment.o: src/Structures/TokenSegment.cpp Include/TokenSegment.h
	clang++ -c -o TokenSegment.o src/Structures/TokenSegment.cpp --std=c++1y  -g
Token.o: src/Structures/Token.cpp Include/Token.h
	clang++ -c -o Token.o src/Structures/Token.cpp --std=c++1y  -g
Module.o: src/Structures/Module.cpp Include/Module.h
	clang++ -c -o Module.o src/Structures/Module.cpp --std=c++1y  -g
fail.o: src/Structures/fail.cpp Include/fail.h
	clang++ -c -o fail.o src/Structures/fail.cpp --std=c++1y  -g
ASTTree.o: src/Structures/ASTTree.cpp Include/ASTTree.h
	clang++ -c -o ASTTree.o src/Structures/ASTTree.cpp --std=c++1y  -g
parsing.o: src/Compiler/parsing.cpp Include/parsing.h
	clang++ -c -o parsing.o src/Compiler/parsing.cpp --std=c++1y  -g
run_example:
	./Compiler compile -source ./Testing/testfile.txt -out testing/output.cpp
test_example:
	./Compiler compile -source ./Testing/testfile.txt -out testing/output.cpp -debug
clean:
	rm *.o
quick_test:
	make compiler;make run_example
memcheck:
	valgrind --leak-check=full ./Compiler compile -source ./Testing/testfile.txt
all:
	cd src; clang++ -o ../bin/Debug/Compiler Structures/ASTNode.cpp Structures/ASTTree.cpp Structures/Compiler.cpp Structures/File.cpp Structures/Token.cpp Structures/TokenLexer.cpp Structures/TokenSegment.cpp Structures/Module.cpp Structures/fail.cpp Program/main.cpp -std=c++11 -Wall -stdlib=libstdc++;cd ..
