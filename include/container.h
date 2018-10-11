// ! ! ! !
//
// WARNING
//
// this realization of a game logic is a very, very stupid and works slow.
// must to be rewritten.
//
// ! ! ! !

#ifndef AREA_H
#define AREA_H

class container
{
private:
	int ** sheet, ** next_sheet;
	int w, h;
	int get_cell_state_in_next_generation(int _x, int _y);
	int ** allocate_matrix_int(int _w, int _h);
	void deallocate_matrix_int(int ** _memory, int _w, int _h);
	int sum_to_conway(int _sum, int _current);
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