#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define HEIGHT 40
#define WIDTH 70
#define MAX_BUF 1000

#ifndef MIN
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#endif

int cell[HEIGHT][WIDTH];

void init_cells()
{
	int i = 0, j;
	FILE *fp;
	char buf[MAX_BUF];
	char path[MAX_BUF] = __FILE__;

	strcat(path, "/../pattern.txt");
	puts(path);

	if ((fp = fopen(path, "r")) == NULL) {
		printf("error: can't open %s\n", path);
		return;
	};

	while (fgets(buf, MAX_BUF, fp) != NULL) {
		size_t len = strlen(buf) - 1;

		for (j = 0; j < MIN(len, WIDTH); j++) {
			cell[i][j] = (buf[j] == ' ') ? 0 : 1;
		}

		i++;
	}
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
