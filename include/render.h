#ifndef DRAWER_H
#define DRAWER_H

#include "area.h"

#pragma comment(lib, "Opengl32.lib")
#include <gl/GL.h>
#include <iostream>
#include <conio.h>
#include <Windows.h>
class using_opengl
{
private:
	int left_button_state,
		right_button_state,
		x_last_cell,
		y_last_cell,
		cell_size,
		window_width,
		window_height,
		Angle;
	area * area_link;
	TCHAR *szWndClass;
	HWND hWnd;
	HDC hDC;
	HGLRC hRC;
	void r(int w, int h);
	void d();
	void left(int _x, int _y);
	void right(int _x, int _y);
	void m(int button, int state, int x, int y);
	void mm(int x, int y);
	void k(unsigned char key, int x, int y);
	void draw_cells();
	void draw_grid();
public:
	drawer(area * _area_link) :area_link(_area_link)
	{
		left_button_state = 0;
		right_button_state = 0;
		x_last_cell = -1;
		y_last_cell = -1;
		cell_size = 12;
		window_width = 600;
		window_height = 300;
	};
	void start(int &argc, char * argv[]);
};
class to_command_line
{
private:
	area * area_link;
	int input_reaction(int _in);
	void draw_cells(int _cursor);
public:
	printer(area * _area_link) :area_link(_area_link) {};
	void start();
};
#endif