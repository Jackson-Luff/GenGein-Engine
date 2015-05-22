#include <time.h>

#include "Tests\ReflectAndRip.h"
#include "Tests\Tessellation.h"
#include "Tests\ConnectFour\ConFourTut.h"
#include "Tests\ParticlesNPrettyStuff.h"
#include "Checkers\Checkers.h"

#include "Checkers_Clean\CheckersGame.h"

int main(int argc, char* argv[])
{
	srand((unsigned int)time(NULL));
	
	CheckersGame* board = new CheckersGame(1200, 628, "Checkers");
	board->Run();

	delete board;

	//Checkers* checkers = new Checkers(1200, 628, "Checkers");
	//checkers->Run();
	//delete checkers;

	//ParticlesNPrettyStuff* particles = new ParticlesNPrettyStuff(1200, 628, "");
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