//#include <vld.h>
#include <stdlib.h>
#include <time.h>

#include "Assessments\Physics\PhysicsAssignment.h"
#include "Assessments\Particles\Particles.h"

int main(int argc, char* argv[])
{
	srand((unsigned int)time(NULL));

	PhysicsAssignment* physics = new PhysicsAssignment(1280, 720, "Physics!");
	physics->Run();
	delete physics;

	//Particles* particles = new Particles(1280, 720, "Particles!");
	//particles->Run();
	//delete particles;

	return 0;
}