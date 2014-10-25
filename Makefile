#make for sudoku
CC=gcc
CFALGS= -g -Wall


sudoku: sudoku.c
	$(CC) $(CFALGS) -o sudoku sudoku.c

clean:
	$(RM) sudoku
