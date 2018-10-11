#ifndef DRAWER_H
#define DRAWER_H

#include "container.h"

// for opengl
#include <windows.h>
#include <tchar.h>
#include <gl\gl.h>
#include <gl\glu.h>

// for cmd
#include <conio.h>
#include <iostream>

class using_opengl
{
private:

public:
	using_opengl() {};
};
class to_command_line
{
private:
	container * container_link;
	int x_cursor, y_cursor;
	int input_reaction(int _in);
	void print_cells();
public:
	to_command_line(container * link) : container_link(link), x_cursor(0), y_cursor(0) {};
	void start();
};
#endif