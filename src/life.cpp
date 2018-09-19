#include "life.h"
Area::Area(int x_cells, int y_cells)
{
	if (x_cells > 0) this->w = x_cells;
	if (y_cells > 0) this->h = y_cells;
	sheet = new int[this->w * this->h];
	next_sheet = new int[this->w * this->h];
	for (int i = 0; i < this->h*this->w; i++)
	{
		sheet[i] = 0;
		next_sheet[i] = 0;
	}
}
void Area::change_cell_state(int x, int y, int state)
{
	if (state > -1 && state < 2 && x > -1 && x < WINDOW_WIDTH && y > -1 && y < WINDOW_HEIGHT)
	{
		//printf("click on (%d, %d), state %d\n", x, y, state);
		sheet[x + y * this->w] = state;
	}
}
int Area::get_cell_state(int x, int y)
{
	if (x > -1 && x < this->w && y > -1 && y < this->h)
	{
		return this->sheet[x + y * this->w];
	}
	else throw(0);
}
void Area::draw_cells()
{
	int x_current = 0, y_current = 0;
	glBegin(GL_QUADS);
	for (int i = 0; i < this->h * this->w; i++)
	{
		if (this->sheet[i] == 1)
		{
			//printf("draw (%d, %d), state %d\n", i - (this->h*this->w) / i, (this->h*this->w) / i, this->sheet[i]);
			x_current = i - (i / this->w) * this->w;
			y_current = i / this->w;
			glColor3f(1.0, 1.0, 1.0);
			glVertex2i(x_current * CELL_SIZE, y_current * CELL_SIZE);
			glVertex2i(x_current * CELL_SIZE + CELL_SIZE, y_current * CELL_SIZE);
			glVertex2i(x_current * CELL_SIZE + CELL_SIZE, y_current * CELL_SIZE + CELL_SIZE);
			glVertex2i(x_current * CELL_SIZE, y_current * CELL_SIZE + CELL_SIZE);
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
{
	int x = 0, y = 0, sum_current = 0;

	x = _i - (_i / this->w) * this->w;
	y = _i / this->w;
	if (y < 1)
	{//bottom
		if (x < 1)
		{//left
			sum_current = (
				this->get_cell_state(x + 1, y) +
				this->get_cell_state(x + 1, y + 1) +
				this->get_cell_state(x, y + 1)
				);
		}
		else if (x > this->h - 2)
		{//right
			sum_current = (
				this->get_cell_state(x - 1, y) +
				this->get_cell_state(x - 1, y + 1) +
				this->get_cell_state(x, y + 1)
				);
		}
		else
		{//center
			sum_current = (
				this->get_cell_state(x - 1, y) +
				this->get_cell_state(x + 1, y) +
				this->get_cell_state(x, y + 1) +
				this->get_cell_state(x + 1, y + 1) +
				this->get_cell_state(x - 1, y + 1)
				);
		}
	}
	else if (y > this->h - 2)
	{//top
		if (x < 1)
		{//left
			sum_current = (
				this->get_cell_state(x + 1, y) +
				this->get_cell_state(x, y - 1) +
				this->get_cell_state(x + 1, y - 1)
				);
		}
		else if (x > this->w - 2)
		{//right
			sum_current = (
				this->get_cell_state(x - 1, y) +
				this->get_cell_state(x, y - 1) +
				this->get_cell_state(x - 1, y - 1)
				);
		}
		else
		{//center
			sum_current = (
				this->get_cell_state(x - 1, y) +
				this->get_cell_state(x + 1, y) +
				this->get_cell_state(x - 1, y - 1) +
				this->get_cell_state(x, y - 1) +
				this->get_cell_state(x + 1, y - 1)
				);
		}
	}
	else
	{//center
		if (x < 1)
		{//left
			sum_current = (
				this->get_cell_state(x, y + 1) +
				this->get_cell_state(x, y - 1) +
				this->get_cell_state(x + 1, y - 1) +
				this->get_cell_state(x + 1, y) +
				this->get_cell_state(x + 1, y + 1)
				);
		}
		else if (x > this->w - 2)
		{//right
			sum_current = (
				this->get_cell_state(x, y + 1) +
				this->get_cell_state(x, y - 1) +
				this->get_cell_state(x - 1, y - 1) +
				this->get_cell_state(x - 1, y) +
				this->get_cell_state(x - 1, y + 1)
				);
		}
		else
		{//center
			sum_current = (
				this->get_cell_state(x, y + 1) +
				this->get_cell_state(x, y - 1) +
				this->get_cell_state(x - 1, y - 1) +
				this->get_cell_state(x - 1, y) +
				this->get_cell_state(x - 1, y + 1) +
				this->get_cell_state(x + 1, y - 1) +
				this->get_cell_state(x + 1, y) +
				this->get_cell_state(x + 1, y + 1)
				);
		}
	}
	if (this->sheet[_i] == 0)
	{
		if (sum_current == 3)
		{
			return 1;
		}
		return 0;
	}
	else if (this->sheet[_i] == 1)
	{
		//printf("%d, %d, %d\n", _i - (_i / this->w) * this->w,
		//	(_i - this->w) - ((_i - this->w) / this->w) * this->w, y);
		if (sum_current == 3 || sum_current == 2)
		{
			return 1;
		}
		return 0;
	}
}
void Area::apply_next_generation()
{
	for (int i = 0; i < this->h * this->w; i++)
	{
		if (this->get_cell_state_in_next_generation(i) == 0)
			this->next_sheet[i] = 0;
		else if (this->get_cell_state_in_next_generation(i) == 1)
			this->next_sheet[i] = 1;
	}
	for (int i = 0; i < this->h * this->w; i++)
	{
		this->sheet[i] = this->next_sheet[i];
	}
}
void Area::clear()
{
	for (int i = 0; i < this->h*this->w; i++)
	{
		this->sheet[i] = 0;
		this->next_sheet[i] = 0;
	}
}