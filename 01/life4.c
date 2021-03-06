#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define HEIGHT 40
#define WIDTH 50

int cell[HEIGHT][WIDTH];

void init_cells()
{
	int i, j;

	for (i = 0; i < HEIGHT; i++) {
		for (j = 0; j < WIDTH; j++) {
			cell[i][j] = (rand() % 6 == 0) ? 1 : 0;
		}
	}
}

void print_cells(FILE *fp)
{
	int i, j, living_cells = 0;

	fprintf(fp, "----------\n");

	for (i = 0; i < HEIGHT; i++) {
		if (i % 2 == 0) {
			fputc(' ', fp);
		}

		for (j = 0; j < WIDTH; j++) {
			const char c = (cell[i][j] == 1) ? '#' : '.';
			fputc(c, fp);
			fputc(' ', fp);

			if (cell[i][j] == 1) {
				living_cells++;
			}
		}

		fputc('\n', fp);
	}

	fprintf(fp, "Total living cells: %d\n", living_cells);

	fflush(fp);

	sleep(1);
}

int count_adjacent_cells(int i, int j)
{
	int n = 0;
	int k, l;
	for (k = i - 1; k <= i + 1; k++) {
		if (k < 0 || k >= HEIGHT) continue;

		for (l = j - 1; l <= j + 1; l++) {
			if (k == i && l == j) {
				continue;
			}

			if (l < 0 || l >= WIDTH) {
				continue;
			}

			// 六角形拡張
			if (i % 2 == 0 && l == j - 1 && k != i) {
				continue;
			}

			if (i % 2 == 1 && l == j + 1 && k != i) {
				continue;
			}

			n += cell[k][l];
		}
	}
	return n;
}

void update_cells()
{
	int i, j;
	int cell_next[HEIGHT][WIDTH];

	for (i = 0; i < HEIGHT; i++) {
		for (j = 0; j < WIDTH; j++) {
			cell_next[i][j] = cell[i][j];
			const int n = count_adjacent_cells(i, j);

			if (cell[i][j] == 1 && n != 2) {
				cell_next[i][j] = 0;
			} else if (cell[i][j] == 0 && n == 2) {
				cell_next[i][j] = 1;
			}
		}
	}

	for (i = 0; i < HEIGHT; i++) {
		for (j = 0; j < WIDTH; j++) {
			cell[i][j] = cell_next[i][j];
		}
	}
}


int main()
{
	int gen;
	FILE *fp;

	srand((unsigned)time(NULL));

	if ((fp = fopen("cells.txt", "a")) == NULL) {
		fprintf(stderr, "error: cannot open a file.\n");
		return 1;
	}

	init_cells();
	print_cells(fp);

	for (gen = 1; ; gen++) {
		printf("generation = %d\n", gen);
		update_cells();
		print_cells(fp);
	}

	fclose(fp);

	return 0;
}
