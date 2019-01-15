assembler: assembler.o helper.o parser.o code.o symbolTable.o includes.h
	gcc -o assembler assembler.o helper.o parser.o code.o symbolTable.o

assembler.o: assembler.c helper.h parser.h code.h symbolTable.h includes.h
	gcc -c assembler.c

helper.o: helper.c helper.h parser.h code.h symbolTable.h includes.h
	gcc -c helper.c

parser.o: parser.c helper.h parser.h includes.h
	gcc -c parser.c

code.o: code.c helper.h code.h includes.h
	gcc -c code.c

symbolTable.o: symbolTable.c helper.h symbolTable.h includes.h
	gcc -c symbolTable.c

debug:
	gcc -g assembler.c helper.c parser.c code.c symbolTable.c

clean:
	rm *.o assembler || del /Q /S *.o assembler.exe