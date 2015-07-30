#pragma once
#include <vector>
#include <glm\glm.hpp>

class AABB
{
public:
	AABB() { Reset(); }
	~AABB();

	void Reset() {
		min.x = min.y = min.z = 1e37f;
		max.x = max.y = max.z = -1e37f;
	}

	void fit(const std::vector<glm::vec3> & points) {
		for (auto& p : points)
		{
			for (int i = 0; i < 3; i++) {
				if (p[i] < min[i]) min[i] = p[i];
				if (p[i] > max[i]) max[i] = p[i];
			}
		}
	}

private:
	glm::vec3 min, max;
};

#pragma once
#include <vector>
#include <glm\glm.hpp>

class BoundingSphere
{
public:
	BoundingSphere() : center(0), radius(0) {}
	~BoundingSphere() {}

	void fit(const std::vector<glm::vec3> & points) {
		
		glm::vec3 min(1e37f), max(1e37f);
		
		for (auto& p : points)
		{
			for (int i = 0; i < 3; i++) {
				if (p[i] < min[i]) min[i] = p[i];
				if (p[i] > max[i]) max[i] = p[i];
			}
		}

		center = (min + max) * 0.5f;
		radius = glm::distance(min, center);
	}

	// Getter n Setter for 'center' (vec3)
	inline const glm::vec3& GetCenter() const {
		return center; 
	}
	inline void SetCenter( const glm::vec3& a_newCenter ) {
		center = a_newCenter;
	}
	// Getter n Setter for 'radius' (float)
	inline const float& GetRadius() const {
		return radius; 
	}
	inline void SetRadius(const float& a_newRadius) {
		radius = a_newRadius;
	}
private:
	glm::vec3 center;
	float radius;
};