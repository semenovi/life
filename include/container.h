#ifndef AREA_H
#define AREA_H

#include <stdio.h>
#include <algorithm>
class container
{
private:
	int ** sheet, ** next_sheet;
	int w, h;
	int get_cell_state_in_next_generation(int _x, int _y);
	int ** allocate_matrix_int(int _w, int _h);
	void deallocate_matrix_int(int ** _memory, int _w, int _h);
public:
	container(int _w, int _h);
	void change_cell_state(int _x, int _y, int _state);
	int get_cell_state(int _x, int _y);
	int get_width() { return w; };
	int get_height() { return h; };
	void apply_next_generation();
	~container() { 
		deallocate_matrix_int(sheet, w, h);
		deallocate_matrix_int(next_sheet, w, h);
	};
};
#endif