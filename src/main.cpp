#include "container.h"
#include "render.h"
int main(int argc, char * argv[])
{
	container a(20, 10);
	to_command_line r(&a);
	r.start();
	return 0;
}