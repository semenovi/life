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
	area * area_link;
	void draw_cells();
	void draw_grid();
public:
	using_opengl(area * _area_link) :area_link(_area_link)
	{};
};
class to_command_line
{
private:
	area * area_link;
	void draw_cells(int _cursor);
public:
	printer(area * _area_link) :area_link(_area_link) {};
};
#endif