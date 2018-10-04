#include "showers.h"
void drawer::draw_cells()
{
	int x_current = 0, y_current = 0;
	glBegin(GL_QUADS);
	for (int i = 0; i < area_link->get_width(); i++)
	{
		for (int j = 0; j < area_link->get_height(); j++)
		if (area_link->get_cell_state(i, j) == 1)
		{
			glColor3f(1.0, 1.0, 1.0);
			glVertex2i(
				i * cell_size,
				j * cell_size);
			glVertex2i(
				i * cell_size + cell_size,
				j * cell_size);
			glVertex2i(
				i * cell_size + cell_size,
				j * cell_size + cell_size);
			glVertex2i(
				i * cell_size,
				j * cell_size + cell_size);
		}
	}
	glEnd();
}
void drawer::draw_grid()
{
	glBegin(GL_LINES);
	glColor3f(0.5, 0.5, 0.5);
	for (int i = cell_size; i < window_width; i += cell_size)
	{
		glVertex2i(i, 0);
		glVertex2i(i, window_height);
	}
	for (int i = cell_size; i < window_height; i += cell_size)
	{
		glVertex2i(0, i);
		glVertex2i(window_width, i);
	}
	glEnd();
}
void drawer::r(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, w, 0, h);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
void drawer::d()
{
	glClear(GL_COLOR_BUFFER_BIT);
	draw_cells();
	draw_grid();
	glutSwapBuffers();
}
void drawer::left(int _x, int _y)
{
	int x = _x / cell_size, y = _y / cell_size;
	area_link->change_cell_state(x, window_height / cell_size - y - 1, 1);
	x_last_cell = x;
	y_last_cell = y;
}
void drawer::right(int _x, int _y)
{
	int x = _x / cell_size, y = _y / cell_size;
	area_link->change_cell_state(x, window_height / cell_size - y - 1, 0);
	x_last_cell = x;
	y_last_cell = y;
}
void drawer::m(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN)
		{
			left_button_state = 1;
			left(x, y);
		}
		else if (state == GLUT_UP)
		{
			left_button_state = 0;
		}
	}
	else if (button == GLUT_RIGHT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			right_button_state = 1;
			right(x, y);
		}
		else if (state == GLUT_UP)
		{
			right_button_state = 0;
		}
	}
}
void drawer::mm(int x, int y)
{
	//printf("%d, %d\n", x, y);
	if (x > -1 && y > -1 && x < window_width && y < window_height &&
		(x > x_last_cell * cell_size + cell_size - 1 ||
			x < x_last_cell * cell_size) ||
			(y > y_last_cell * cell_size + cell_size - 1 ||
				y < y_last_cell * cell_size))
	{
		if (left_button_state == 1) left(x, y);
		else if (right_button_state == 1) right(x, y);
	}
}
void drawer::k(unsigned char key, int x, int y)
{
	//printf("%d\n", key);
	if (key == 32)
		area_link->apply_next_generation();
	/*
	else if (key == 114 || key == 234)
		game_area.clear();
		*/
}
void drawer::start(int &argc, char * argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(window_width, window_height);
	glutCreateWindow("Life");
	glutReshapeFunc(r);
	glutDisplayFunc(d);
	glutIdleFunc(d);
	glutMouseFunc(this->m);
	glutMotionFunc(this->mm);
	glutKeyboardFunc(this->k);
	glutMainLoop();
}
int printer::input_reaction(int _in)
{
	int out = 1;
	if (_in == 32) //space
		area_link->apply_next_generation();
	else if (_in == 97)
		out = 2; //left
	else if (_in == 115)
		out = 3; //down
	else if (_in == 100)
		out = 4; //right
	else if (_in == 119)
		out = 5; //up
	else if (_in == 120)
		out = 6; //x
	else if (_in == 101)
		out = 0; //e
	return out;
}
void printer::draw_cells(int _cursor)
{
	for (int i = 0; i < area_link->get_height(); i++)
	{
		for (int j = 0; j < area_link->get_width(); j++)
			if (i * area_link->get_width() + j == _cursor)
				std::cout << "_ ";
			else
				std::cout << (area_link->get_cell_state(i, j) ? "x" : "-") << " ";
		std::cout << "\n";
	}
}
void printer::start()
{
	int state = 0, in = 0, cursor = 0;
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
	draw_cells(cursor);
	while (1)
	{
		in = getch();
		state = input_reaction(in);
		system("cls");
		if (state == 1)
		{
			draw_cells(cursor);
		}
		else if (state == 2)
		{
			if ((cursor % area_link->get_width()) > 0)
				cursor--;
			draw_cells(cursor);
		}
		else if (state == 3)
		{
			if ((cursor / area_link->get_width()) < area_link->get_height())
				cursor += area_link->get_width();
			draw_cells(cursor);
		}
		else if (state == 4)
		{
			if ((cursor % area_link->get_width()) < area_link->get_height())
				cursor++;
			draw_cells(cursor);
		}
		else if (state == 5)
		{
			if ((cursor / area_link->get_width()) > 0)
				cursor -= area_link->get_width();
			draw_cells(cursor);
		}
		else if (state == 6)
		{
			if ((*sheet).IsMember(cursor))
				(*sheet).DelElem(cursor);
			else
				(*sheet).InsElem(cursor);
			draw_cells(cursor, sheet);
		}
		else
			break;
	}

}