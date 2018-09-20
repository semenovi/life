#include "life.h"
Area::Area(int x_cells, int y_cells)
{
	if (x_cells > 0) w = x_cells;
	if (y_cells > 0) h = y_cells;
	sheet = new int[w * h];
	next_sheet = new int[w * h];
	for (int i = 0; i < h*w; i++)
	{
		sheet[i] = 0;
		next_sheet[i] = 0;
	}
}
void Area::change_cell_state(int x, int y, int state)
{
	if (state > -1
		&& state < 2
		&& x > -1
		&& x < (WINDOW_WIDTH / CELL_SIZE)
		&& y > -1
		&& y < (WINDOW_HEIGHT / CELL_SIZE))
		sheet[x + y * w] = state;
	else throw(0);
}
int Area::get_cell_state(int x, int y)
{
	if (x > -1 && x < w && y > -1 && y < h)
		return sheet[x + y * w];
	else throw(0);
}
void Area::draw_cells()
{
	int x_current = 0, y_current = 0;
	glBegin(GL_QUADS);
	for (int i = 0; i < h * w; i++)
	{
		if (sheet[i] == 1)
		{
			x_current = i - (i / w) * w;
			y_current = i / w;
			glColor3f(1.0, 1.0, 1.0);
			glVertex2i(
				x_current * CELL_SIZE,
				y_current * CELL_SIZE);
			glVertex2i(
				x_current * CELL_SIZE + CELL_SIZE,
				y_current * CELL_SIZE);
			glVertex2i(
				x_current * CELL_SIZE + CELL_SIZE,
				y_current * CELL_SIZE + CELL_SIZE);
			glVertex2i(
				x_current * CELL_SIZE,
				y_current * CELL_SIZE + CELL_SIZE);
		}
	}
	glEnd();
}
void Area::draw_grid()
{
	glBegin(GL_LINES);
	glColor3f(0.5, 0.5, 0.5);
	for (int i = CELL_SIZE; i < WINDOW_WIDTH; i += CELL_SIZE)
	{
		glVertex2i(i, 0);
		glVertex2i(i, WINDOW_HEIGHT);
	}
	for (int i = CELL_SIZE; i < WINDOW_HEIGHT; i += CELL_SIZE)
	{
		glVertex2i(0, i);
		glVertex2i(WINDOW_WIDTH, i);
	}
	glEnd();
}
int Area::get_cell_state_in_next_generation(int _i)
{//stupid, but work
	int x = 0, y = 0, sum_current = 0;
	x = _i - (_i / w) * w;
	y = _i / w;
	if (y < 1)
	{//bottom
		if (x < 1)
		{//left
			sum_current = (
				get_cell_state(x + 1, y) +
				get_cell_state(x + 1, y + 1) +
				get_cell_state(x, y + 1)
				);
		}
		else if (x > h - 2)
		{//right
			sum_current = (
				get_cell_state(x - 1, y) +
				get_cell_state(x - 1, y + 1) +
				get_cell_state(x, y + 1)
				);
		}
		else
		{//center
			sum_current = (
				get_cell_state(x - 1, y) +
				get_cell_state(x + 1, y) +
				get_cell_state(x, y + 1) +
				get_cell_state(x + 1, y + 1) +
				get_cell_state(x - 1, y + 1)
				);
		}
	}
	else if (y > h - 2)
	{//top
		if (x < 1)
		{//left
			sum_current = (
				get_cell_state(x + 1, y) +
				get_cell_state(x, y - 1) +
				get_cell_state(x + 1, y - 1)
				);
		}
		else if (x > w - 2)
		{//right
			sum_current = (
				get_cell_state(x - 1, y) +
				get_cell_state(x, y - 1) +
				get_cell_state(x - 1, y - 1)
				);
		}
		else
		{//center
			sum_current = (
				get_cell_state(x - 1, y) +
				get_cell_state(x + 1, y) +
				get_cell_state(x - 1, y - 1) +
				get_cell_state(x, y - 1) +
				get_cell_state(x + 1, y - 1)
				);
		}
	}
	else
	{//center
		if (x < 1)
		{//left
			sum_current = (
				get_cell_state(x, y + 1) +
				get_cell_state(x, y - 1) +
				get_cell_state(x + 1, y - 1) +
				get_cell_state(x + 1, y) +
				get_cell_state(x + 1, y + 1)
				);
		}
		else if (x > w - 2)
		{//right
			sum_current = (
				get_cell_state(x, y + 1) +
				get_cell_state(x, y - 1) +
				get_cell_state(x - 1, y - 1) +
				get_cell_state(x - 1, y) +
				get_cell_state(x - 1, y + 1)
				);
		}
		else
		{//center
			sum_current = (
				get_cell_state(x, y + 1) +
				get_cell_state(x, y - 1) +
				get_cell_state(x - 1, y - 1) +
				get_cell_state(x - 1, y) +
				get_cell_state(x - 1, y + 1) +
				get_cell_state(x + 1, y - 1) +
				get_cell_state(x + 1, y) +
				get_cell_state(x + 1, y + 1)
				);
		}
	}
	if (sheet[_i] == 0)
	{
		if (sum_current == 3)
			return 1;
		return 0;
	}
	else if (sheet[_i] == 1)
	{
		if (sum_current == 3 || sum_current == 2)
			return 1;
		return 0;
	}
}
void Area::apply_next_generation()
{
	for (int i = 0; i < h * w; i++)
	{
		if (get_cell_state_in_next_generation(i) == 0)
			next_sheet[i] = 0;
		else if (get_cell_state_in_next_generation(i) == 1)
			next_sheet[i] = 1;
	}
	for (int i = 0; i < h * w; i++)
	{
		sheet[i] = next_sheet[i];
	}
}
void Area::clear()
{
	for (int i = 0; i < h*w; i++)
	{
		sheet[i] = 0;
		next_sheet[i] = 0;
	}
}