#pragma once
#include <glm\glm.hpp>

class PhysActor
{
public:

	enum class ShapeType
	{
		PLANE,
		SPHERE,
		CUBE,
		NUMBER_OF_SHAPES
	};
	
	virtual void Update(const glm::vec3 a_gravity, const float a_dt) = 0;
	virtual void Debug() = 0;
	virtual void MakeGizmo() = 0;

	inline const ShapeType& GetID() 
		{ return m_shapeID; }

	virtual const glm::vec3 GetPosition()
		{ return glm::vec3(0); };

	virtual void ResetPosition(){};

protected:
	inline const void SetID(ShapeType a_shapeID) 
		{ m_shapeID = a_shapeID; }

	ShapeType m_shapeID;
};