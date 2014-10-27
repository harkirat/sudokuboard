/*
 * Reference: Algorithm Design Manual
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0
#define DEBUG 0
#define BASED      3            /* base dimension, 3*3 blocks */
#define DIMENSION  BASED*BASED      /* 9*9 board */

typedef struct {
    int m[DIMENSION][DIMENSION]; /* matrix of board contents */
    int freecount;           /* how many open element remain? */
} sudokuboard;

/*
 * print the solution 
 */
void print_board(sudokuboard *board) {
    int i,j;
    FILE *fp = NULL;

    if ((fp=fopen("solved.csv", "w")) == NULL) {
        perror("Fatal Error in File Creation");
        return;
    }
    
    for (i=0; i < DIMENSION; i++) {
        for (j=0; j < DIMENSION; j++) {
            if (!j) {
                printf("%d", board->m[i][j]);
                fprintf(fp, "%d", board->m[i][j]);
            } else {
                printf(",%d", board->m[i][j]);
                fprintf(fp, ",%d", board->m[i][j]);
            }
        }
        printf("\n");
        fprintf(fp, "\n");
    }
    fclose(fp);
}

/*
 * find first empty value
 */
int next_emptyvalue(sudokuboard *board, int *row, int *col) {

    for (*row = 0; *row < DIMENSION; (*row)++) {
        for (*col = 0; *col < DIMENSION; (*col)++) {
            if (board->m[*row][*col] == 0) {
                return TRUE;
            }
        }
    }
    return FALSE;
}

/*
 * a valid candidate is:
 * i) do not appear in row
 * ii) do not appear in col
 * iii) do not appear in 3x3 cell
 */
int is_valid_candidate(sudokuboard *board, int num, int row, int col) {

    int i, j;
    int rlow, clow;

    /* used in row */
    for (j=0; j < DIMENSION; j++) {
        if (board->m[row][j] == num) {
            return FALSE;
        } 
    }
    /* used in col */
    for (i=0; i < DIMENSION; i++) {
        if (board->m[i][col] == num) {
            return FALSE;
        } 
    }
    /*used in 3x3 base */
    rlow = BASED * ((int) (row / BASED));
    clow = BASED * ((int) (col / BASED));

    for (i=rlow; i<rlow+BASED; i++) {
        for (j=clow; j<clow+BASED; j++) {
            if (board->m[i][j] == num) { 
                return FALSE;
            }
        }
    }
    return TRUE;
}

/*
 * backtrack until free count remains 
 */
int backtrack(sudokuboard *board) {
    int r, c, num;      

    if (board->freecount == 0) { /* have a solution */
        print_board(board);
        return TRUE;    
    }
    if (next_emptyvalue(board, &r, &c) == FALSE) {
        printf("%s-%d: Error , there must be an unassigned value\n", __func__, __LINE__);
        return FALSE;
    }

    for (num = 1; num <= 9; num++) {
        if (is_valid_candidate(board, num, r, c)) {

            /* make move */
            board->m[r][c] = num;
            board->freecount--; 
            
            if (backtrack(board)) {
                return TRUE;
            }

            /*unmake the move */
            board->m[r][c] = 0;
            board->freecount++; 
        }
    }
    return FALSE;
}

/*
 * init board
 */
void init_board(sudokuboard *board) {

    int i=0, j=0;

    for (i=0; i < DIMENSION; i++) {
        for (j=0; j < DIMENSION; j++) {
            board->m[i][j] = 0;
        }
    }
    board->freecount = 0;
    return;
}

/*
 * Read input csv file representing sudoku
 * valid input values are [0..9]
 */
int read_board(sudokuboard *board, char *input_board) {

    FILE *fp = NULL;
    char *line, *tmp;
    char buffer[1024];
    int i=0, j=0, n;   
    

    init_board(board);

    if ((fp = fopen(input_board, "r")) == NULL) {
        perror("Input File Open Failed");
        return -1;
    }
    
    while((line=fgets(buffer,sizeof(buffer),fp)) != NULL) {
        j = 0;
        tmp = strtok(line, ",");
        while(tmp != NULL) {
            if (i >= DIMENSION || j >= DIMENSION) {
                printf("\ninvalid board, excedding dimensions %d x %d\n", i, j);
                return -1;
            }
            n = atoi(tmp);
            if (n >= 0 && n <= 9) {
                board->m[i][j++] = n;
#if DEBUG
                printf(" %d", n); 
#endif
                tmp = strtok(NULL, ",");
                if (!n) {
                    board->freecount++;
                }
            } else {
                printf("\ninvalid input board [%d]\n", n);
                return -1;
            }
        } 
#if DEBUG
        printf("\n");
#endif
        ++i;
    }

#if DEBUG
    printf("freecount %d\n", board->freecount);
#endif
    fclose(fp);
    return 0;

}

int main(int argc, char *argv[]) {

    sudokuboard board;        /* Sudoku board structure */

    if (argc != 2) {
        printf("\nUsage: sudoku board.csv\n");
        return 0;
    }
    if ( read_board(&board, argv[1]) != 0 ) {
        return 0;
    }
    if ( backtrack(&board) == FALSE ) {
        printf("\nProgram couldn't find soln..!\n");
        return 0;
    }
    return 0;
}
 
