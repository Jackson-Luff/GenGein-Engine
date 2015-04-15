#include <time.h>
#include "Final/ProGenEnvironment.h" 

int main(int argc, char* argv[])
{
	srand((unsigned int)time(NULL));
	
	ProGenEnvironment tut = ProGenEnvironment(1200, 628, "Computer Graphics");
	tut.Run();
	return 0;
}