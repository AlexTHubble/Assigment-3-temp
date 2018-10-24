#pragma once

#include <Trackable.h>
#include "Connection.h"

const int BAD_NODE_ID = -1;
#define NODE_ID int

class Node:public Trackable
{
public:
	Node( const NODE_ID& id );
	Node();
	~Node();


	int getCost() { return mCost; };
	void setCost(int newCost) { mCost = newCost; };
	//void setConnection(Connection* newConnection) { mConnection = newConnection; };
	//Connection* getConnection() { return mConnection; };
	void setPreviousNode(Node* newNode) { mPreviousNode = newNode; };
	Node* getPreviousNode() { return mPreviousNode; };

	const NODE_ID& getId() const {return mId;};

private:
	const NODE_ID mId;
	int mCost = 1;
	Node *mPreviousNode;
	//Connection *mConnection;
};