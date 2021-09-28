#pragma once
#include "Misc/PathfindingDetails.hpp"
#include "AStarNode.h"

class AStarPather
{
public:
    /* 
        The class should be default constructible, so you may need to define a constructor.
        If needed, you can modify the framework where the class is constructed in the
        initialize functions of ProjectTwo and ProjectThree.
    */

    /* ************************************************** */
    // DO NOT MODIFY THESE SIGNATURES
    bool initialize();
    void shutdown();
    PathResult compute_path(PathRequest &request);
    /* ************************************************** */

    /*
        You should create whatever functions, variables, or classes you need.
        It doesn't all need to be in this header and cpp, structure it whatever way
        makes sense to you.
    */

private:
    std::shared_ptr<AStarNode> getMinCostNode(const std::set<std::shared_ptr<AStarNode>>& list);
    std::vector<GridPos> addNeighbors(GridPos pos);
    void mapChangeCallback();

    std::set<std::shared_ptr<AStarNode>> openList;
    std::set<std::shared_ptr<AStarNode>> closeList;

    int mapWidth, mapHeight;
};