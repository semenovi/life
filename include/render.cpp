#include "render.h"

int to_command_line::input_reaction(int _in)
{
	if (_in == 32)
	{ //space
		container_link->apply_next_generation();
		print_cells();
	}
	else if (_in == 97)
	{ //left
		if (y_cursor > 0)
			y_cursor--;
		print_cells();
	}
	else if (_in == 115)
	{ //down
		if (x_cursor < container_link->get_width() - 1)
			x_cursor++;
		print_cells();
	}
	else if (_in == 100)
	{ //right
		if (y_cursor < container_link->get_height() - 1)
			y_cursor++;
		print_cells();
	}
	else if (_in == 119)
	{ //up
		if (x_cursor > 0)
			x_cursor--;
		print_cells();
	}
	else if (_in == 120)
	{ //x
		if (container_link->get_cell_state(x_cursor, y_cursor))
			container_link->change_cell_state(x_cursor, y_cursor, 0);
		else
			container_link->change_cell_state(x_cursor, y_cursor, 1);
		print_cells();
	}
	else if (_in == 101)
	{ //e
		return 1;
	}
	return 0;
}
void to_command_line::print_cells()
{
	for (int i = 0; i < container_link->get_width(); i++)
	{
		for (int j = 0; j < container_link->get_height(); j++)
			if (i == x_cursor && j == y_cursor)
				std::cout << "_ ";
			else
				std::cout << (container_link->get_cell_state(i, j) ? "x " : "- ");
		std::cout << "\n";
	}
}
void to_command_line::start()
{
	int state = 0, in = 0;
	std::cout << "Press space to start game\n";
	std::cout << "(Use wasd to move, x to create/kill cell, space to live, e to exit)";
	while (getch() != 32) {}
	system("cls");
	/*
	in = getch();
	cout << in;
	in = getch();
	in = 0;
	*/
	print_cells();
	while (1)
	{
		in = getch();
		system("cls");
		state = input_reaction(in);
		if (state)
			break;
	}

}