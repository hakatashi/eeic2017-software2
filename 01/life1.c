#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define HEIGHT 40
#define WIDTH 70

int cell[HEIGHT][WIDTH];

void init_cells()
{
	int i, j;

	for (i = 0; i < HEIGHT; i++) {
		for (j = 0; j < WIDTH; j++) {
			cell[i][j] = 0;
		}
	}

	cell[20][31] = 1;
	cell[21][32] = 1;
	cell[22][30] = 1;
	cell[22][31] = 1;
	cell[22][32] = 1;
}

void print_cells(FILE *fp)
{
	int i, j, living_cells = 0;

	fprintf(fp, "----------\n");

	for (i = 0; i < HEIGHT; i++) {
		for (j = 0; j < WIDTH; j++) {
			const char c = (cell[i][j] == 1) ? '#' : ' ';
			fputc(c, fp);

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
			if (k == i && l == j) continue;
			if (l < 0 || l >= WIDTH) continue;
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

			if (cell[i][j] == 1 && n != 2 && n != 3) {
				cell_next[i][j] = 0;
			} else if (cell[i][j] == 0 && n == 3) {
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
