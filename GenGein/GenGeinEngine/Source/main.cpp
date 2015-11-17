
//#include <vld.h>

#include <stdlib.h>
#include <time.h>

#include "Projects\Physics\PhysicsAssignment.h"
#include "Projects\Particles\Particles.h"
#include "Projects\TIANW\TIANW_App.h"

int main(int argc, char* argv[])
{
	srand((unsigned int)time(NULL));
	
	//Particles* particles = new Particles(p1280, 720, "Particles!");
	//particles->Run();
	//delete particles;

	TIANW_App* tomorrow = new TIANW_App(1280, 720, "Tomorrow!");
	tomorrow->Run();
	delete tomorrow;

	return 0;
}