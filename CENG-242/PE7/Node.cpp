#include "Node.h"

// // // THE FRIEND METHODS ARE ALREADY IMPLEMENTED BELOW. // // //
// // // // // // // DO NOT CHANGE THEM! // // // // // // //

Node::Node(int id) {
	// this->children = new vector<Node*>;
	this->id = id;
}

Node::~Node() {
	// TODO
	
	for(unsigned int i = 0; i < this->children.size(); i++) {
        delete children[i];
		children[i] = NULL;
    }
	
}

Node::Node(const Node& node) {
	// TODO
	this->id = node.id;

	
	//childs = new vector<Node*>;
	
	// Node *child;

	
	for (unsigned int i = 0; i < node.children.size(); i++)
	{
		Node *child = new Node(node.children[i]->getId());
		child = node.children[i];
		this->children.push_back(child);
	}
	
	/*
	for (unsigned int i=0; i<node.children.size(); i++) {
		this->children.push_back(node.children[i]);
	}
	*/
	
}

int Node::getId() const {
	return this->id;
}

char Node::getData() const {
	// TODO
	throw InvalidRequest();
	
}

vector<Node*>& Node::getChildren() {
	// TODO
	
	vector<Node*> *childs = &this->children;
	return *childs;
	
}

void Node::operator+=(Node& childNode) {
	// TODO
	this->children.push_back(&childNode);

}

Node* Node::operator&(const Node& node) const {
	// TODO
	try //ilki node
	{
		this->getData();
	}
	catch(InvalidRequest e)
	{
		try
		{
			node.getData();
		}
		catch(InvalidRequest e)
		{
			Node* new_node = new Node(this->id);
			return new_node;
		}
		Node* new_node = new DataNode(node.id,node.getData());
		return new_node;
	}


	try //ikincisi node
	{
		node.getData();
	}
	catch(InvalidRequest e)
	{
		Node* new_node = new DataNode(this->id,this->getData());
		return new_node;
	}
	Node* new_node = new DataNode(this->id,this->getData());
		return new_node;

}

// This is already implemented for you, do NOT change it!
ostream& operator<<(ostream& os, const Node& node) {
	try {
		node.getData();
		os << *(DataNode*)&node;
	}
	catch (InvalidRequest e) {
		os << "[" << node.id;
		for (int i = 0; i < node.children.size(); i++)
			os << ", " << *node.children[i];
		os << "]";
		return os;
	}
}

/*************** DataNode *****************/

DataNode::DataNode(int id, char data):Node(id) {
	// TODO
	//this->id = id;
	this->data = data;
}

DataNode::~DataNode() {
	// TODO
	
	for(unsigned int i = 0; i < this->children.size(); i++) {
        delete children[i];
		children[i] = NULL;
    }
}

DataNode::DataNode(const DataNode& dataNode):Node(dataNode) {
	// TODO
	/*
	const int *pointer = &dataNode.id;
	this->id = *pointer;

	const vector<Node*> *childs = &dataNode.children;
	this->children = *childs;
*/
	const char *copy_data = &dataNode.data;
	this->data = *copy_data;
	
}

DataNode::DataNode(const Node& node, char data):Node(node) {
	// TODO
	// const int *pointer = &node.id;
	// this->id = *pointer;

	// const vector<Node*> *childs = &node.children;
	// this->children = *childs;
	
	const char *copy_data = &data;
	this->data = *copy_data;
	
}

char DataNode::getData() const {
	// TODO
	return this->data;
}

// This is already implemented for you, do NOT change it!
ostream& operator<<(ostream& os, const DataNode& dataNode) {
	os << "[" << "(" << dataNode.id << ", \'" << dataNode.data << "\')";
	for (int i = 0; i < dataNode.children.size(); i++)
		os << ", " << *dataNode.children[i];
	os << "]";
	return os;
}
