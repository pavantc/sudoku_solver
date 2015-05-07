#include <stdio.h>

#define	MAX_SOLUTIONS	100

void solve_sudoku(int (*arr)[9][9], int filled_slots, int row, int col, int *sc);

void
print_arr(int (*arr)[9][9])
{
	int i, j;

	for (i = 0; i < 9; i++) {
		for (j = 0; j < 9; j++) {
			printf("%d  ", (*arr)[i][j]);
		}
		printf("\n");
	}
	printf("\n\n");
}

/*
 * Returns 1 if 'num' can be placed in (*arr)[row][col]
 * Else, returns 0
 */
int
can_place(int num, int (*arr)[9][9], int row, int col)
{
	int i, j;
	int bbr, bbc;	/* block begin row, block begin col */

	/* Make sure 'col' does not already have this number */
	for (i = 0; i < 9; i++) {
		if ((*arr)[i][col] == num) {
			return (0);
		}
	}

	/* Make sure 'row' does not already have this number */
	for (j = 0; j < 9; j++) {
		if ((*arr)[row][j] == num) {
			return (0);
		}
	}

	/* Make sure the 3X3 block containing this cell does not already have this number */
	bbr = (row/3) * 3;
	bbc = (col/3) * 3;
	for (i = bbr; i < bbr+3; i++) {
		for (j = bbc; j < bbc+3; j++) {
			if ((*arr)[i][j] == num) {
				return (0);
			}
		}
	}

	return (1);
}

void
solve_next_slot(int (*arr)[9][9], int filled_slots, int row, int col, int *sc)
{
	/* We move row-wise. If all elements of a row have been filled (i.e. when col == 9), we move to the next row */
	if (col + 1 == 9) {
		solve_sudoku(arr, filled_slots, row + 1, 0, sc);
	} else {
		solve_sudoku(arr, filled_slots, row, col + 1, sc);
	}
}

void
solve_sudoku(int (*arr)[9][9], int filled_slots, int row, int col, int *sc)
{
	int k;

	if (*sc == MAX_SOLUTIONS) {
		return;
	}

	if (row == 9 || col == 9) {
		return;
	}

	if ((*arr)[row][col] != 0) {
		/* Immutable entry, part of initial state. Skip over */
		solve_next_slot(arr, filled_slots, row, col, sc);
	} else {
		/* Try all digits from 1 to 9 for a given slot */
		for (k = 1; k <= 9; k++) {
			if (can_place(k, arr, row, col)) {
				(*arr)[row][col] = k;
				filled_slots++;
				if (filled_slots == 81) {
					/* Found a solution! */
					print_arr(arr);
					(*sc)++;
				} else {
					solve_next_slot(arr, filled_slots, row, col, sc);
				}
				/* Backtrack */
				(*arr)[row][col] = 0;
				filled_slots--;
			}
		}
	}
}

int
get_filled_slots(int (*arr)[9][9])
{
	int i, j, fs = 0;
	for (i = 0; i < 9; i++) {
		for (j = 0; j < 9; j++) {
			if ((*arr)[i][j]) {
				fs++;
			}
		}
	}
	return (fs);
}

int
main()
{
	int single_solution_game[9][9] = {
		{8, 3, 0,   1, 0, 0,   6, 0, 5},
		{0, 0, 0,   0, 0, 0,   0, 8, 0},
		{0, 0, 0,   7, 0, 0,   9, 0, 0},

		{0, 5, 0,   0, 1, 7,   0, 0, 0},
		{0, 0, 3,   0, 0, 0,   2, 0, 0},
		{0, 0, 0,   3, 4, 0,   0, 1, 0},

		{0, 0, 4,   0, 0, 8,   0, 0, 0},
		{0, 9, 0,   0, 0, 0,   0, 0, 0},
		{3, 0, 2,   0, 0, 6,   0, 4, 7},
	};
	int i, j, filled_slots = 0;
	int solution_count = 0;

	solve_sudoku(&single_solution_game, get_filled_slots(&single_solution_game), 0, 0, &solution_count);
	printf("Number of solutions = %d\n", solution_count);
	if (solution_count == MAX_SOLUTIONS) {
		printf("Stopping after %d solutions\n", MAX_SOLUTIONS);
	}
}
