#include <time.h>

#include "MoreTesting\ReflectAndRip.h"
#include "Tutorials\Tessellation.h"
#include "Tutorials\ConnectFour\ConFourTut.h"

int main(int argc, char* argv[])
{
	srand((unsigned int)time(NULL));
	
	ReflectAndRip* RnR = new ReflectAndRip(1200, 628, "Reflections And Ripples");
	RnR->Run();
	delete RnR;

	//Tessellation* tess = new Tessellation(1200, 628, "Tessellation");
	//tess->Run();
	//delete tess;

	ConFourTut* con = new ConFourTut(1200, 628, "Connect Four!");
	con->Run();
	delete con;
	
	return 0;
}