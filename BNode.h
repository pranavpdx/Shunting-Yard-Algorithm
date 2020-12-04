#pragma once
#include <iostream>
class BNode
{
public: 
	// node class for the nodes in the binary expression tree
	// variables
	char data;
	BNode* right;
	BNode* left;
	// constructor defines variables
	BNode(char v);
};

