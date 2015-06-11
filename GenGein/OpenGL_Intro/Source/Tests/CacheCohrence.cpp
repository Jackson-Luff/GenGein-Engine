#include <Windows.h>

#include "CacheCohrence.h"


void addChildren(Node* root, int childrenPerNode, int depth)
{
	if (depth > 0)
	{
		for (int i = 0; i < childrenPerNode; i++)
		{
			Node* child = new Node();
			addChildren(child, childrenPerNode, depth - 1);
			root->addChild(child);
		}
	}
}

CacheCohrence::CacheCohrence()
{}

void CacheCohrence::Run()
{
	Node* root = new Node();
	addChildren(root, 10, 6);

	// query the speed of the CPU clock cycles
	LARGE_INTEGER perfFreq;
	QueryPerformanceFrequency(&perfFreq);

	LARGE_INTEGER startTime, endTime;

	// get start time
	QueryPerformanceCounter(&startTime);

	root->Update();

	// get end time
	QueryPerformanceCounter(&endTime);

	// length of time taken in seconds
	double_t duration = (endTime.QuadPart - startTime.QuadPart) / (double_t)perfFreq.QuadPart;

	printf("Duration: %f seconds", duration);

	delete root;
}

CacheCohrence::~CacheCohrence()
{
}
