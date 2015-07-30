#include <malloc.h>
#include <PxPhysicsAPI.h>

class MemAlloc : public physx::PxAllocatorCallback
{
public:
	virtual ~MemAlloc() {}
	virtual void* allocate(size_t a_size, const char* a_typeName, const char* a_fileName, int a_line)
	{
		void* pointer = _aligned_malloc(a_size, 16);
		return pointer;
	}

	virtual void deallocate(void* a_ptr)
	{
		_aligned_free(a_ptr);
	}
};