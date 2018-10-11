#include "container.h"
int ** container::allocate_matrix_int(int _w, int _h)
{
	if (_w < 0 || _h < 0) throw 0;
	int ** _memory = new int*[_w];
	for (int i = 0; i < _w; i++)
	{
		_memory[i] = new int[_h];
		for (int j = 0; j < _h; j++)
			_memory[i][j] = 0;
	}
	return _memory;
}
void container::deallocate_matrix_int(int ** _memory, int _w, int _h)
{
	if (_w < 0 || _h < 0 || _memory == nullptr) throw 0;
	for (int i = 0; i < _w; i++)
		delete[] _memory[i];
	delete[] _memory;
}
int container::sum_to_conway(int _sum, int _current)
{
	switch (_current)
	{
	case 0:
		if (_sum == 3)
			return 1;
		break;
	case 1:
		if (_sum == 2 || _sum == 3)
			return 1;
		break;
	}
	return 0;
}
container::container(int _w, int _h)
{
	if (_w < 0 || _h < 0) throw 0;
	w = _w;
	h = _h;
	sheet = allocate_matrix_int(w, h);
	next_sheet = allocate_matrix_int(w, h);
}
void container::change_cell_state(int _x, int _y, int _state)
{
	if (_state < 0 || _state > 1 || _x > w || _y > h || _x < 0 || _y < 0) throw 0;
	sheet[_x][_y] = _state;
}
int container::get_cell_state(int _x, int _y)
{
	if (_x > w || _y > h || _x < 0 || _y < 0) throw 0;
	return sheet[_x][_y];
}
int container::get_cell_state_in_next_generation(int _x, int _y)
{//stupid, but work
	int sum_current = 0;
	if (_y < 1)
	{//top
		if (_x < 1)
		{//left
			sum_current = (
				get_cell_state(_x + 1, _y) +
				get_cell_state(_x + 1, _y + 1) +
				get_cell_state(_x, _y + 1)
				);
		}
		else if (_x > w - 2)
		{//right
			sum_current = (
				get_cell_state(_x - 1, _y) +
				get_cell_state(_x - 1, _y + 1) +
				get_cell_state(_x, _y + 1)
				);
		}
		else
		{//center
			sum_current = (
				get_cell_state(_x - 1, _y) +
				get_cell_state(_x + 1, _y) +
				get_cell_state(_x, _y + 1) +
				get_cell_state(_x + 1, _y + 1) +
				get_cell_state(_x - 1, _y + 1)
				);
		}
	}
	else if (_y > h - 2)
	{//bottom
		if (_x < 1)
		{//left
			sum_current = (
				get_cell_state(_x + 1, _y) +
				get_cell_state(_x, _y - 1) +
				get_cell_state(_x + 1, _y - 1)
				);
		}
		else if (_x > w - 2)
		{//right
			sum_current = (
				get_cell_state(_x - 1, _y) +
				get_cell_state(_x, _y - 1) +
				get_cell_state(_x - 1, _y - 1)
				);
		}
		else
		{//center
			sum_current = (
				get_cell_state(_x - 1, _y) +
				get_cell_state(_x + 1, _y) +
				get_cell_state(_x - 1, _y - 1) +
				get_cell_state(_x, _y - 1) +
				get_cell_state(_x + 1, _y - 1)
				);
		}
	}
	else
	{//center
		if (_x < 1)
		{//left
			sum_current = (
				get_cell_state(_x, _y + 1) +
				get_cell_state(_x, _y - 1) +
				get_cell_state(_x + 1, _y - 1) +
				get_cell_state(_x + 1, _y) +
				get_cell_state(_x + 1, _y + 1)
				);
		}
		else if (_x > w - 2)
		{//right
			sum_current = (
				get_cell_state(_x, _y + 1) +
				get_cell_state(_x, _y - 1) +
				get_cell_state(_x - 1, _y - 1) +
				get_cell_state(_x - 1, _y) +
				get_cell_state(_x - 1, _y + 1)
				);
		}
		else
		{//center
			sum_current = (
				get_cell_state(_x, _y + 1) +
				get_cell_state(_x, _y - 1) +
				get_cell_state(_x - 1, _y - 1) +
				get_cell_state(_x - 1, _y) +
				get_cell_state(_x - 1, _y + 1) +
				get_cell_state(_x + 1, _y - 1) +
				get_cell_state(_x + 1, _y) +
				get_cell_state(_x + 1, _y + 1)
				);
		}
	}
	return(sum_to_conway(sum_current, sheet[_x][_y]));
}
void container::apply_next_generation()
{
	for (int i = 0; i < w; i++)
		for (int j = 0; j < h; j++)
			next_sheet[i][j] = get_cell_state_in_next_generation(i, j);
	for (int i = 0; i < w; i++)
		for (int j = 0; j < h; j++)
			sheet[i][j] = next_sheet[i][j];
}