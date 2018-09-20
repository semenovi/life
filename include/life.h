#pragma once
#include <GL/glut.h>
#include <stdio.h>
const int WINDOW_WIDTH = 600, WINDOW_HEIGHT = 300, CELL_SIZE = 12;
class Area
{
public:
	Area(int x_cells, int y_cells);
	void change_cell_state(int x, int y, int state);
	int get_cell_state(int x, int y);
	int get_width() { return w; };
	int get_height() { return h; };
	void draw_cells();
	void draw_grid();
	int get_cell_state_in_next_generation(int _i);
	void apply_next_generation();
	void clear();
	~Area() { delete[] sheet; };
private:
	int * sheet;
	int * next_sheet;
	int w;
	int h;
};