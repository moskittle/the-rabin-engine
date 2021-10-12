#pragma once

#include "pch.h"


class Node
{
public:
	Node() = default;
	~Node() = default;

	Node(GridPos _position, float _givenCost, std::shared_ptr<Node> _parent) : position(_position), givenCost(_givenCost), parent(_parent) {}

	//const GridPos& get_position() { return position; }
	//float get_givenCost() { return givenCost; }
	//float get_heuristicCost() { return heuristicCost; }
	//float get_finalCost() { return givenCost + heuristicCost; }
	float getFinalCost(float weight) { return givenCost + heuristicCost * weight; }

	bool operator< (const Node& other)const
	{
		return givenCost + heuristicCost > other.givenCost + other.heuristicCost;
	}

	//private:
	GridPos position;
	float givenCost = -1.0f;
	float heuristicCost = -1.0f;
	std::shared_ptr<Node> parent = nullptr;
	//bool isOnList = false;
};
