#include <time.h>

//#include "Tests\ReflectAndRip.h"
//#include "Tests\Tessellation.h"
//#include "Tests\ConnectFour\ConFourTut.h"
//#include "Tests\ParticlesNPrettyStuff.h"
//#include "Checkers_Dirty\Checkers.h"
#include "Checkers\GameOfCheckers.h"
//#include "PhysXTuts\PhysX_testing.h"

int main(int argc, char* argv[])
{
	srand((unsigned int)time(NULL));
	
	//PhysX_testing* physX = new PhysX_testing(1200, 628, "PhysX testing");
	//physX->Run();
	//delete physX;

	GameOfCheckers* board = new GameOfCheckers(1200, 628, "Checkers");
	board->Run();
	delete board;

	//Checkers* checkers = new Checkers(1200, 628, "Checkers");
	//checkers->Run();
	//delete checkers;

	//ParticlesNPrettyStuff* particles = new ParticlesNPrettyStuff(1200, 628, "Pretticles");
	//particles->Run();
	//delete particles;

	//ReflectAndRip* RnR = new ReflectAndRip(1200, 628, "Reflections And Ripples");
	//RnR->Run();
	//delete RnR;

	//ProGenEnvironment* pge = new ProGenEnvironment(1200, 628, "Procedurally Generated Environment");
	//pge->Run();
	//delete pge;

	//Tessellation* tess = new Tessellation(1200, 628, "Tessellation");
	//tess->Run();
	//delete tess;

	//ConFourTut* con = new ConFourTut(1200, 628, "Connect Four!");
	//con->Run();
	//delete con;
	
	return 0;
}