#include <stdlib.h>
#include <time.h>

#include "Assessments\Physical\PhysicsAssignment.h"

int main(int argc, char* argv[])
{
	srand((unsigned int)time(NULL));

	PhysicsAssignment* physics = new PhysicsAssignment(1280, 720, "Physics!");
	physics->Run();
	delete physics;
	return 0;
}