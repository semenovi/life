#include "life.h"
Area game_area(WINDOW_WIDTH / CELL_SIZE, WINDOW_HEIGHT / CELL_SIZE);
int left_button_state = 0, right_button_state = 0, x_last_cell = -1, y_last_cell = -1;
void r(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, w, 0, h);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
void d()
{
	glClear(GL_COLOR_BUFFER_BIT);
	game_area.draw_cells();
	game_area.draw_grid();
	glutSwapBuffers();
}
void left(int _x, int _y)
{
	int x = _x / CELL_SIZE, y = _y / CELL_SIZE;
	game_area.change_cell_state(x, WINDOW_HEIGHT / CELL_SIZE - y - 1, 1);
	x_last_cell = x;
	y_last_cell = y;
}
void right(int _x, int _y)
{
	int x = _x / CELL_SIZE, y = _y / CELL_SIZE;
	game_area.change_cell_state(x, WINDOW_HEIGHT / CELL_SIZE - y - 1, 0);
	x_last_cell = x;
	y_last_cell = y;
}
void m(int button, int state, int x, int y)
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
void mm(int x, int y)
{
	//printf("%d, %d\n", x, y);
	if (x > -1 && y > -1 && x < WINDOW_WIDTH && y < WINDOW_HEIGHT &&
		(x > x_last_cell * CELL_SIZE + CELL_SIZE - 1 ||
			x < x_last_cell * CELL_SIZE) ||
			(y > y_last_cell * CELL_SIZE + CELL_SIZE - 1 ||
				y < y_last_cell * CELL_SIZE))
	{
		if (left_button_state == 1) left(x, y);
		else if (right_button_state == 1) right(x, y);
	}
}
void k(unsigned char key, int x, int y)
{
	//printf("%d\n", key);
	if (key == 32)
	{
		game_area.apply_next_generation();
	}
	else if (key == 114 || key == 234)
	{
		game_area.clear();
	}
}
void game(int &argc, char * argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("Life");
	glutReshapeFunc(r);
	glutDisplayFunc(d);
	glutIdleFunc(d);
	glutMouseFunc(m);
	glutMotionFunc(mm);
	glutKeyboardFunc(k);
	glutMainLoop();
}
int main(int argc, char * argv[])
{
	game(argc, argv);
	return 0;
}