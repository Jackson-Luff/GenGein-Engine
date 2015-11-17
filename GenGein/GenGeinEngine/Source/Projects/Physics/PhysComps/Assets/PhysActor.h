#pragma once
#include <glm\glm.hpp>
#include <glm\ext.hpp>

class PhysActor
{
public:

	enum class BodyType
	{
		STATIC,
		DYNAMIC,
		KINEMATIC
	};

	enum class ShapeType
	{
		PLANE  = 0,
		SPHERE = 1,
		AABB   = 2,
		JOINT  = 3,
		NUMBER_OF_SHAPES
	};

	virtual void Update(const glm::vec3 a_gravity, const double a_dt) = 0;
	
	virtual void ResetPosition(){};
	virtual const glm::vec3 GetPosition()
		{ return glm::vec3(0); };

	inline const ShapeType& GetShapeID() 
		{ return m_shapeID; }

	inline const BodyType& GetBodyID() 
		{ return m_bodyID; }

protected:
	ShapeType m_shapeID;
	BodyType m_bodyID;
};