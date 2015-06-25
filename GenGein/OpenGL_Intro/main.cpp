#include <stdlib.h>
#include <time.h>

#include "Extras\LetsGetPhysical\PhysicsTest01.h"

int main(int argc, char* argv[])
{
	srand((unsigned int)time(NULL));

	PhysicsTest01* checkers = new PhysicsTest01(1280, 720, "Checkers!");
	checkers->Run();
	return 0;
}