#include <time.h>
#include "..\Tutorials\Tutorial04.h" 

int main(int argc, char* argv[])
{
	srand((unsigned int)time(NULL));
	
	Tutorial04 tut = Tutorial04(1366, 768, "Computer Graphics");
	tut.Run();
	return 0;
}