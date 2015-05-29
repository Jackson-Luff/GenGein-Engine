#pragma once
#include <vector>
#include "glm\glm.hpp"

class CacheCohrence
{
public:
	CacheCohrence();
	void Run();
	~CacheCohrence();
};

class BoundingSphere {
public:
	BoundingSphere() : centre(0), radius(1) {}
	~BoundingSphere() {}

	void fit(const BoundingSphere& sphere) {
		float d = glm::distance(centre, sphere.centre) +
			sphere.radius;
		if (d > radius)
			radius = d;
	}

	glm::vec4 centre;
	float radius;
};

class Node
{
public:
	Node() : parent(nullptr), localTrans(1) {}
	virtual ~Node() { for (auto& child : children) delete child; }

	void addChild(Node* child){
		children.push_back(child);
	}

	virtual void Update() {
		updateWorldTrans();

		for (auto& child : children)
			child->Update();

		updateWorldBounds();
	}

	void updateWorldTrans() {
		if (parent == nullptr)
			worldTrans = worldTrans;
		else
			worldTrans = parent->worldTrans *
			localTrans;
	}

	void updateWorldBounds() {
		worldBounds.centre = worldTrans * localBounds.centre;
		worldBounds.radius = localBounds.radius;

		for (auto& child : children)
			worldBounds.fit(child->worldBounds);
	}

protected:

	Node*			parent;

	BoundingSphere	localBounds;
	BoundingSphere	worldBounds;

	glm::mat4		localTrans;
	glm::mat4		worldTrans;

	std::vector<Node*> children;
};