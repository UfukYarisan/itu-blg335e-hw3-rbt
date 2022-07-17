/*@Author
Student Name: Ufuk Yarisan
Student ID : 070170206
Date: 11.01.2022
*/


// I HAD AN ISSUE TO FIX
// POSSIBLE ERROR ARE = SEARCH FUNCTION, FIXUP


#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <time.h>

using namespace std;

// To define the color and the biggest sales number and the winner of the decades.
const int BLACK = 0;
const int RED = 1;
double biggest = 0;
string winner = "";


struct Node{
	string nameOfVideoGame;
	string nameOfPublisher;
	double naSales;
	double euSales;
	double otherSales;
	int yearOfRelease;
	bool color = RED;
	Node* left = nullptr;
	Node* right = nullptr;
	Node* parent = nullptr;
};

struct RBTree{
	Node* root = nullptr;
};

void printTree(Node* &node, int depth);

void findMostSelledEU(Node* &node);

void findMostSelledNA(Node* &node);

void findMostSelledOther(Node* &node);

int readVideoGames(vector<Node*> &nodePointerVector, const string &I);

Node* searchSame(Node* &root, Node* &searchNode);

Node* normalInsert(Node* &root, Node* &newNode);

void leftRotate(Node* &root, Node* &node);

void rightRotate(Node* &root, Node* &node);

void fixUp(Node* &root, Node* &newNode);

void RBTInsert(RBTree &tree, Node* &newNodePointer);


int main(int argc, char **argv){
	if (argc != 2){
		exit(1);
	}
	
	string I = argv[1];

	vector<Node*> nodePointerVector;
	
	RBTree tree;

	int numberOfVideos = readVideoGames(nodePointerVector, I);
	//initial root is given
	int i = 0;
	for(vector<Node*>::iterator it = nodePointerVector.begin()+1; it != nodePointerVector.end(); ++it){
		// HERE IS TO CHECK SAMPLE OUTPUT I KNOW IT IS FALSE I COULDNT CATCH UP THE DEADLINE :(
		
		if(nodePointerVector[i]->yearOfRelease != nodePointerVector[i+1]->yearOfRelease && nodePointerVector[i]->yearOfRelease % 10 == 0 && nodePointerVector[i+1]->yearOfRelease % 10 != 0){
			cout << "End of the " << nodePointerVector[i]->yearOfRelease << " Year" << endl;

			findMostSelledNA(tree.root);
			cout << "Best seller in North America: " << winner << " - " << biggest << "million" << endl;
			biggest = 0;

			findMostSelledEU(tree.root);
			cout << "Best seller in Europe: " << winner << " - " << biggest << "million" << endl;
			biggest = 0;

			findMostSelledOther(tree.root);
			cout << "Best seller rest of the World: " << winner << " - " << biggest << "million" << endl;
			biggest = 0;

		}
		
		RBTInsert(tree, nodePointerVector[i++]);
	}

	cout << "End of the " << nodePointerVector[i]->yearOfRelease << " Year" << endl;
	findMostSelledNA(tree.root);
	cout << "Best seller in North America: " << winner << " - " << biggest << "million" << endl;
	biggest = 0; // BECAUSE OF GLOBAL VARIABLE

	findMostSelledEU(tree.root);
	cout << "Best seller in Europe: " << winner << " - " << biggest << "million" << endl;
	biggest = 0; // BECAUSE OF GLOBAL VARIABLE

	findMostSelledOther(tree.root);
	cout << "Best seller rest of the World: " << winner << " - " << biggest << "million" << endl;
	biggest = 0; // BECAUSE OF GLOBAL VARIABLE

	printTree(tree.root, 0);

	exit(1);
};

int readVideoGames(vector<Node*> &nodePointerVector, const string &I){
	fstream fp;
	fp.open(I, ios::in | ios::out);
	if(!fp.is_open()){
		cerr << "File couldn't be opened." << endl;
		exit(1);
	}
	string line, column;
	getline(fp, line); //in order to skip headers
	int howManyVideoGames = 0; //FOR DEBUGGING PURPOSES
	getline(fp, line);
	

	while(getline(fp, line)){

		Node* newNodePtr = nullptr;
		newNodePtr = new Node;

		stringstream s(line);
		getline(s, column, ',');
		newNodePtr->nameOfVideoGame = column;
		getline(s, column, ',');
		//skip platform
		getline(s, column, ',');
		newNodePtr->yearOfRelease = stoi(column);
		getline(s, column, ',');
		newNodePtr->nameOfPublisher = column;
		getline(s, column, ',');
		newNodePtr->naSales = stod(column);
		getline(s, column, ',');
		newNodePtr->euSales = stod(column);
		getline(s, column, ',');
		newNodePtr->otherSales = stod(column);
		nodePointerVector.push_back(newNodePtr);
		howManyVideoGames++;

	}
	fp.close();
	return howManyVideoGames;
}

//DOES REGULAR INSERT
Node* normalInsert(Node* &root, Node* &newNodePointer){
	//CHECKS IF THERE IS A NODE WITH A SAME PUBLISHER
	Node* sameNode = searchSame(root, newNodePointer);

	//CHECKS TREE IS EMPTY OR NOT
	if(root == nullptr){
		return newNodePointer;
	}
	//IF THERE IS A NODE WITH SAME PUBLISHER NAME
	else if(sameNode != nullptr){
		sameNode->naSales += newNodePointer->naSales;
		sameNode->euSales += newNodePointer->euSales;
		sameNode->otherSales += newNodePointer->otherSales;
		return root;
	}
	//PUB NAME IS PRIOR ALPHABETICALLY
	else if(newNodePointer->nameOfPublisher < root->nameOfPublisher){
		root->left = normalInsert(root->left, newNodePointer);
		root->left->parent = root;
	}
	//PUB NAME IS LATTER ALPHABETICALLY
	else if(newNodePointer->nameOfPublisher > root->nameOfPublisher){
		root->right = normalInsert(root->right, newNodePointer);
		root->right->parent = root;
	}
	return root;
}

//ROTATES 
void leftRotate(Node* &root, Node* &node){
	
    Node *rightOfNode = node->right;
 
    Node *leftOfRightOfNode = rightOfNode->left;
 
    if (leftOfRightOfNode != nullptr){
		leftOfRightOfNode->parent = node;
		node->right = leftOfRightOfNode;
	}else{
		node->right = nullptr;
	}
        
 
    rightOfNode->parent = node->parent;
 
    if (node->parent == nullptr)
        root = rightOfNode;
 
    else if (node == node->parent->left)
        node->parent->left = rightOfNode;
 
    else
        node->parent->right = rightOfNode;
 
    rightOfNode->left = node;
    node->parent = rightOfNode;
}

void rightRotate(Node* &root, Node* &node){
	Node* leftOfNode = node->left;
	Node* rightOfLeftNode = leftOfNode->right;

	if(rightOfLeftNode != nullptr){
		rightOfLeftNode->parent = node;
		node->left = rightOfLeftNode;
	}else{
		node->left = nullptr;
	}
		

	leftOfNode->parent = node->parent;

	if(node->parent == nullptr)
		root = leftOfNode;
	
	else if (node == node->parent->left)
		node->parent->left = leftOfNode;

	else
		node->parent->right = leftOfNode;

	leftOfNode->right = node;
	node->parent = leftOfNode;

}

void fixUp(Node* &root, Node* &newNodePointer){
	while(newNodePointer != root && newNodePointer != root->left && newNodePointer != root->right && newNodePointer->parent->color == RED){

		Node* parent = newNodePointer->parent;
		Node* grandParent = parent->parent;
		Node* uncle;

		if(parent != nullptr && grandParent != nullptr && parent == grandParent->right){
			uncle = grandParent->left;
		}
		else{
			uncle = grandParent->right;
		}

		//IF THERE IS AN UNCLE AND UNCLE IS RED THEN RECOLOR IT
		if(uncle != nullptr && uncle->color == RED){
				uncle->color = BLACK;
				parent->color = BLACK;
				grandParent->color = RED;
				newNodePointer = grandParent;
				fixUp(root, newNodePointer);
			}
		//IF UNCLE IS BLACK(NULL) ROTATE AND CHANGE COLORS
		else{
				bool color;
			//LEFT LEFT CASE
			if(parent == grandParent->left && newNodePointer == parent->left){
				rightRotate(root, grandParent);
				color = parent->color;
				parent->color = grandParent->color;
				grandParent->color = color;
			}
			//LEFT RIGHT CASE
			else if(parent == grandParent->left && newNodePointer == parent->right){
				leftRotate(root, parent);
				rightRotate(root, grandParent);
				color = newNodePointer->color;
				newNodePointer->color = grandParent->color;
				grandParent->color = color;
			}
			//RIGHT LEFT CASE
			else if(parent == grandParent->right && newNodePointer == parent->left){
				rightRotate(root, parent);
				leftRotate(root, grandParent);
				color = newNodePointer->color;
				newNodePointer->color = grandParent->color;
				grandParent->color = color;
			}
			//RIGHT RIGHT CASE
			else if(parent == grandParent->right && newNodePointer == parent->right){
				leftRotate(root, grandParent);
				color = parent->color;
				parent->color = grandParent->color;
				grandParent->color = color;
			}
			}	
	}
	//ALWAYS PAINT THE ROOT BLACK
	root->color = BLACK;
}

void RBTInsert(RBTree &tree, Node* &newNodePointer){
	//CHECKS IS THERE SAME NODE BEFORE INSERTION
	bool same = false;
	if(searchSame(tree.root, newNodePointer) != nullptr){
		same = true;
	}
	//SAME CASE IS HANDLED IN THE NORMAL INSERTION FUNCTION
	tree.root = normalInsert(tree.root, newNodePointer);
	//IF THERE WAS NO SAME NODE BEFORE INSERTION, FIX THE INSERTED ONE
	if(same == false){
		fixUp(tree.root, newNodePointer);
	}
	
	
}

//RETURNS THE POINTER WHICH POINTS THE SAME NODE
Node* searchSame(Node* &root, Node* &searchNode){
	Node* rootOfMiniTree = root;
	if(root == nullptr){
		return nullptr;
	}
	else if(searchNode->nameOfPublisher < root->nameOfPublisher){
		rootOfMiniTree = root->left;
		return searchSame(rootOfMiniTree, searchNode);
	}else if(searchNode->nameOfPublisher > root->nameOfPublisher){
		rootOfMiniTree = root->right;
		return searchSame(rootOfMiniTree, searchNode);
	}else{
		return rootOfMiniTree;
	}
	return nullptr;
}

//CALCULATES MOST SELLED IN NA THANKS TO GLOBAL VARIABLE
void findMostSelledNA(Node* &node)
{
    if (node == NULL)
        return;

	if(node->naSales > biggest){
		biggest = node->naSales;
		winner = node->nameOfPublisher;
	}
    findMostSelledNA(node->left);
    findMostSelledNA(node->right);
}

//CALCULATES MOST SELLED IN EU THANKS TO GLOBAL VARIABLE
void findMostSelledEU(Node* &node){
    if (node == NULL)
        return;

	if(node->euSales > biggest){
		biggest = node->euSales;
		winner = node->nameOfPublisher;
	}
    findMostSelledEU(node->left);
    findMostSelledEU(node->right);
}

//CALCULATES MOST SELLED IN OTHER PART OF THE WORLD THANKS TO GLOBAL VARIABLE
void findMostSelledOther(Node* &node){
    if (node == NULL)
        return;

	if(node->otherSales > biggest){
		biggest = node->otherSales;
		winner = node->nameOfPublisher;
	}
    findMostSelledOther(node->left);
    findMostSelledOther(node->right);
}

void printTree(Node* &node, int depth){
    if (node == NULL)
        return;
	for(int i = 0; i < depth; i++){
		cout << "-";
	}

	if(node->color == RED){
		cout << "(RED) " << node->nameOfPublisher << endl;
	}else{
		cout << "(BLACK) " << node->nameOfPublisher << endl;
	}
	depth++;

    printTree(node->left, depth);
    printTree(node->right, depth);
}