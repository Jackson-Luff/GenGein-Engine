#include <time.h>
#include "..\Tutorials\Tutorial12.h" 

int main(int argc, char* argv[])
{
	srand((unsigned int)time(NULL));
	
	Tutorial12 tut = Tutorial12(1200, 628, "Computer Graphics");
	tut.Run();
	return 0;
}