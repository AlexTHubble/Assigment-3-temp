#include "DijsktraPathfinder.h"
#include "Path.h"
#include "Connection.h"
#include "GridGraph.h"
#include "Game.h"
#include "PriorityQueue(NEW).h"
#include <PerformanceTracker.h>
#include <list>
#include <vector>
#include <algorithm>
#include "Compare.h"
#include "GameApp.h"
#include "Grid.h"
#include <Vector2D.h>
//#include <queue>

using namespace std;

Dijsktra::Dijsktra(Graph* pGraph, bool Astar)
	:GridPathfinder(dynamic_cast<GridGraph*>(pGraph))
{
#ifdef VISUALIZE_PATH
	mpPath = NULL;
#endif
	mApplyAStar = Astar;
}

Dijsktra::~Dijsktra()
{
#ifdef VISUALIZE_PATH
	delete mpPath;
#endif
}

//Finds a path from a node to a node using Dijkstra's algorithm
//Apply's A*'s algorithm if set to true in mApplyAStar
Path* Dijsktra::findPath(Node* pFrom, Node* pTo)
{
	cout << "Beginging path finding Dijsktra..." << endl;

	gpPerformanceTracker->clearTracker("path");
	gpPerformanceTracker->startTracking("path");

	//Sets up the open list
	MyQueue<Node*, std::vector<Node*>, Compare> openNodes;
	openNodes.push(pFrom);

#ifdef VISUALIZE_PATH
	delete mpPath;
	mVisitedNodes.clear();
	mVisitedNodes.push_back(pFrom);
#endif

	
	Path* closedNodes = new Path(); //Used for what nodes has been closed
	Path* shortestPath = new Path(); //Used for displaying the shortest path to the target

	Node* currentNode; //Current node in the open list
	bool toNodeAdded = false; //If the node has been found

	//Prosses through each open node
	while (openNodes.size() > 0)
	{

		//get current node from front of list
		currentNode = openNodes.top();

		//remove node from list
		openNodes.pop();

		//Adds the node to the closed node list
		closedNodes->addNode(currentNode);

		//If the current node is not the target
		if (currentNode == pTo)
		{
			break;
		}

		//get the Connections for the current node
		vector<Connection*> connections = mpGraph->getConnections(currentNode->getId());

		//add all toNodes in the connections to the "toVisit" list, if they are not already in the list
		for (unsigned int i = 0; i < connections.size(); i++)
		{

			//Sets up connection & node
			Connection* nodeConnection = connections[i];
			Node* endNode = connections[i]->getToNode();



			//If the node hasn't been found, it's not in the closed list, and if it's been found in the open node priority queue
			if (!toNodeAdded && 
				!closedNodes->containsNode(endNode) &&
				openNodes.find(endNode) == openNodes.end())
			{
				float endNodeCost = currentNode->getCost() + nodeConnection->getCost();

				if (mApplyAStar) //If it is set to apply A* then add the weight calculation to the cost
				{
					//The heuistic being used is the direction of the current node to the targeted node

					Grid* grid = dynamic_cast<GameApp*>(gpGame)->getGrid(); //Gets the grid from gpGame
					Vector2D toVector = grid->getULCornerOfSquare(pTo->getId()); //Gets the vector for pTo
					Vector2D endVector = grid->getULCornerOfSquare(endNode->getId()); //Gets the vector for end node

					Vector2D direction = toVector - endVector;
					float heuristic = direction.getLength();

					endNodeCost += heuristic;


				}

				endNode->setCost(endNodeCost); //Sets the cost to endNode

				//Add to the priority queue and set the previous node to it's correct node
				openNodes.push(endNode);
				endNode->setPreviousNode(currentNode);

				//Checks to see if the node is the one we are looking for
				if (endNode == pTo)
				{
					toNodeAdded = true;
				}

				//Adds the node to mVistedNodes for the UI elements
#ifdef VISUALIZE_PATH
				mVisitedNodes.push_back(endNode);
#endif
			}
		} //END OF OPEN NODES FOR LOOP
	}

	//Shut's down preformance tracker
	gpPerformanceTracker->stopTracking("path");
	mTimeElapsed = gpPerformanceTracker->getElapsedTime("path");

	//If the node has not been found break out of the algorithm
	if (currentNode != pTo)
	{
		cout << "Not found" << endl;

		return NULL;
	}

	//Finds the shortest path through the previous node pointers
	Node* temp = pTo;
	bool endOfLoop = false;
	while (temp != pFrom)
	{
		shortestPath->addNode(temp);
		temp = temp->getPreviousNode();
	}

	//Visualizes the shortest path
#ifdef VISUALIZE_PATH
	mpPath = shortestPath;
#endif

	//Clean up
	delete closedNodes;
	cout << "Ended path finding Dijsktra..." << endl;

	return shortestPath;

}

