///////////////////////////////////////////////////////////////////////////////
//
// Advent of Code 2019
// Day 3a: Crossed wires, Manhattan distance
//
// E. Dronkert
// https://github.com/ednl/aoc2019
//
///////////////////////////////////////////////////////////////////////////////

#include <stdio.h>   // fopen, fgetc, printf
#include <stdlib.h>  // atoi

#define BUFLEN 16
#define WIRES 2

const char *inp = "inp03.txt";

typedef struct Line {
	int x1, y1, x2, y2;
} LINE, *PLINE;

int min(int a, int b)
{
	return a <= b ? a : b;
}

int max(int a, int b)
{
	return a >= b ? a : b;
}

int dist(PLINE w1, PLINE w2)
{
	int p, q;

	if (w1->y1 == w1->y2) {
		// w1 = horizontal
		if (w2->y1 == w2->y2) {
			// w2 = horizontal
			if (w1->y1 == w2->y1) {
				// same horizontal line
				p = max(w1->x1, w2->x1);
				q = min(w1->x2, w2->x2);
				if (p <= q) {
					printf("!!!");
					return min(abs(p), abs(q)) + w1->y1;
				}
			}
		} else {
			// w2 = vertical
			if (w1->y1 >= w2->y1 && w1->y1 <= w2->y2 && w1->x1 <= w2->x1 && w1->x2 >= w2->x1)
				return abs(w1->y1) + abs(w2->x1);
		}
	} else {
		// w1 = vertical
		if (w2->y1 == w2->y2) {
			// w2 = horizontal
			if (w1->x1 >= w2->x1 && w1->x1 <= w2->x2 && w1->y1 <= w2->y1 && w1->y2 >= w2->y1)
				return abs(w1->x1) + abs(w2->y1);
		} else {
			// w2 = vertical
			if (w1->x1 == w2->x1) {
				// same vertical line
				p = max(w1->y1, w2->y1);
				q = min(w1->y2, w2->y2);
				if (p <= q) {
					printf("!!!");
					return min(abs(p), abs(q)) + w1->x1;
				}
			}
		}
	}
	return -1;  // no overlap
}

int main(void)
{
	FILE *fp;
	char val[BUFLEN];
	int i, j, k, l, c, err = 0, x, y, dir;

	int len[WIRES];
	PLINE wire[WIRES];

	// Open input file
	if ((fp = fopen(inp, "r")) == NULL) {
		puts("File not found");
		err = 1;
		goto quit;
	}

	// Count CSV fields for all lines
	for (k = 0; k < WIRES; ++k) {
		len[k] = 1;
		while ((c = fgetc(fp)) != EOF && c != '\r' && c != '\n')
			if (c == ',')
				++len[k];
		if ((wire[k] = (PLINE)malloc(len[k] * sizeof(LINE))) == NULL) {
			puts("Out of memory");
			err = 2;
			goto quit;
		}
	}

	// Read data from file
	rewind(fp);
	for (k = 0; k < WIRES; ++k) {
		i = j = 0;
		x = y = dir = 0;
		while ((c = fgetc(fp)) != EOF && c != '\r' && c != '\n')
			if (c == 'L' || c == 'R' || c == 'U' || c == 'D')
				dir = c;
			else if (c >= '0' && c <= '9' && i < BUFLEN - 1)
				val[i++] = c;
			else {
				if (dir > 0 && i > 0 && j < len[k]) {
					val[i] = '\0';
					if ((l = atoi(val)) > 0)
						switch (dir) {
							case 'L': wire[k][j++] = (LINE){ x - l, y, x, y }; x -= l; break;
							case 'R': wire[k][j++] = (LINE){ x, y, x + l, y }; x += l; break;
							case 'U': wire[k][j++] = (LINE){ x, y, x, y + l }; y += l; break;
							case 'D': wire[k][j++] = (LINE){ x, y - l, x, y }; y -= l; break;
						}
				}
				i = 0;
				dir = 0;
			}
			if (dir > 0 && i > 0 && j < len[k]) {
				val[i] = '\0';
				if ((l = atoi(val)) > 0)
					switch (dir) {
						case 'L': wire[k][j++] = (LINE){ x - l, y, x, y }; x -= l; break;
						case 'R': wire[k][j++] = (LINE){ x, y, x + l, y }; x += l; break;
						case 'U': wire[k][j++] = (LINE){ x, y, x, y + l }; y += l; break;
						case 'D': wire[k][j++] = (LINE){ x, y - l, x, y }; y -= l; break;
					}
			}
	}
	fclose(fp);
	fp = NULL;

	// Check minimum distance
	k = -1;
	for (i = 0; i < len[0]; ++i)
		for (j = 0; j < len[1]; ++j)
			if ((i > 0 || j > 0) && (l = dist(&wire[0][i], &wire[1][j])) >= 0 && (l < k || k == -1))
				k = l;
	printf("%i\n", k);

quit:
	if (fp != NULL)
		fclose(fp);
	for (k = 0; k < WIRES; ++k)
		if (wire[k] != NULL)
			free(wire[k]);
	return err;
}
