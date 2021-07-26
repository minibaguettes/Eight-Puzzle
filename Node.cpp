#include <iostream>
#include <queue>
#include <string>
#include <cmath>
using namespace std;

class Node {
public:
	vector<vector<int>> state;	// state the node corresponds to
	Node* parent;	// node's parent node
	string action;	// parent node's action to get to current node
	double pathCost;	// total cost (g(n) + h(n))
	double h;  // h(n) 
	double g;  // g(n)

	Node() {
		parent = nullptr;	
		action = "";	
		pathCost = 0;
		h = 0;
		g = 0;
	};
	Node(Node* n) {
		this->state = n->state;
		this->parent = n->parent;
		this->action = n->action;
		this->pathCost = n->pathCost;
		this->h = n->h;
		this->g = n-> g;
	};
	void printNode() {
		for (int i = 0; i < this->state.size(); i++) {
			for (int j = 0; j < this->state.size(); j++) {
				cout << this->state.at(i).at(j) << " ";
			}
			cout << endl;
		}
	}
};

class Problem {
public:
	Node* initialState = new Node();
	Node* goalState = new Node();
	string operators[4] = { "left", "right", "up", "down" };

	Problem(Node* init) {
		initialState = init;
		int size = initialState->state.size();
		int count = 0;
		vector<int> row;
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				if (i == size - 1 && j == size - 1) {
					row.push_back(0);
				}
				else {
					count++;
					row.push_back(count);
				}
			}
			goalState->state.push_back(row);
			while (!row.empty()) {
				row.pop_back();
			}
		}
	}
};

// print out node path
void printPath(Node* n) {
	vector<Node*> path;
	Node* curr = n;
	path.push_back(curr);
	while (curr->parent != nullptr) {
		path.push_back(curr->parent);
		curr = curr->parent;
	}
	//path.push_back(curr);
	for (int i = path.size() - 1; i > -1; i--) {
		cout << "g = " << path.at(i)->g << ", h = " << path.at(i)->h << endl;
		cout << path.at(i)->action << endl;
		path.at(i)->printNode();
		cout << "______" << endl;
	}
}

// for min pq
class ComparePath {
public:
	bool operator()(const Node* n1, const Node* n2) {
		return (n1->pathCost > n2->pathCost);
	}
};

// compare two node states
int CompareNode(Node* current, Node* goal) {	// compare current and goal state; 1 if the same, 0 if different
	for (int i = 0; i < current->state.size(); i++) {
		for (int j = 0; j < current->state.size(); j++) {
			if (current->state.at(j) != goal->state.at(j)) {
				return 0;
			}
		}
	}
	return 1;
}

Node* MoveBlank(Node* n, int i, int j, string move){
	if (move == "up"){
		swap(n->state[i][j], n->state[i - 1][j]);
	}
	else if (move == "down"){
		swap(n->state[i][j], n->state[i + 1][j]);
	}
	else if (move == "left"){
		swap(n->state[i][j], n->state[i][j - 1]);
	}
	else if (move == "right"){
		swap(n->state[i][j], n->state[i][j + 1]);
	}
	Node* child = new Node(n);
	return child;
}

bool inFrontier(Node* child, priority_queue<Node*, vector<Node*>, ComparePath> frontier) {
	int notFr;
	bool inFr;
	vector<Node*> vect;

	while (!frontier.empty()) {
		vect.push_back(frontier.top());
		frontier.pop();
	}
	for (int i = 0; i < vect.size(); i++) {
		frontier.push(vect.at(i));
		notFr = CompareNode(child, vect.at(i));
		if (notFr == 0){  //not in frontier
			inFr = false;
		}
		else{  //in frontier
			return true;
		}
	}
	return inFr;
}

bool isExplored(Node* child, vector<Node*> explore){
	int notEx;
  	bool explored;

	for(int k = 0; k < explore.size(); k++){
		notEx = CompareNode(child, explore.at(k));	
		if (notEx == 0){  //not explored
			explored = false;
		}
		else{  //explored
			return true;
		}
	}
  	return explored;
}

// expand current node
vector<Node* > Expand(Node* n, vector<Node*> explored, priority_queue<Node*, vector<Node*>, ComparePath> frontier) {
	int size = n->state.size();
	int row;
	int column;
	vector<Node* > children;
	Node* temp = new Node(n);

	// check where blank is
	for (int i = 0; i < n->state.size(); i++) {
		for (int j = 0; j < n->state.size(); j++) {
			if (n->state[i][j] == 0) {
				row = i;  // blank's row
				column = j;  //blank's column
			}
		}
	}

	if (row == 0){  // first row
		if (column == 0){  // first column of first row
			Node* child = MoveBlank(temp, row, column, "right");
			if (!isExplored(child, explored) && !inFrontier(child, frontier)){
				children.push_back(child);
				child->g++;
				child->parent = n;
				child->action = "right";
			}

			temp = MoveBlank(temp, row, column, "right");

			child = MoveBlank(temp, row, column, "down");
			if (!isExplored(child, explored) && !inFrontier(child, frontier)){
				children.push_back(child);
				child->g++;
				child->parent = n;
				child->action = "down";
			}		
		}
		else if (column == size - 1){  // last column of first row
			Node* child = MoveBlank(temp, row, column, "left");
			if (!isExplored(child, explored) && !inFrontier(child, frontier)){
				children.push_back(child);
				child->g++;
				child->parent = n;
				child->action = "left";
			}

			temp = MoveBlank(temp, row, column, "left");

			child = MoveBlank(temp, row, column, "down");
			if (!isExplored(child, explored) && !inFrontier(child, frontier)){
				children.push_back(child);
				child->g++;
				child->parent = n;
				child->action = "down";
			}		
		}
		else {  // any other column
			Node* child = MoveBlank(temp, row, column, "left");
			if (!isExplored(child, explored) && !inFrontier(child, frontier)){
				children.push_back(child);
				child->g++;
				child->parent = n;
				child->action = "left";
			}

			temp = MoveBlank(temp, row, column, "left");

			child = MoveBlank(temp, row, column, "right");
			if (!isExplored(child, explored) && !inFrontier(child, frontier)){
				children.push_back(child);
				child->g++;
				child->parent = n;
				child->action = "right";
			}		
			
			temp = MoveBlank(temp, row, column, "right");

			child = MoveBlank(temp, row, column, "down");
			if (!isExplored(child, explored) && !inFrontier(child, frontier)){
				children.push_back(child);
				child->g++;
				child->parent = n;
				child->action = "down";
			}
		}
	}		
	else if (row == size - 1){
		if (column == 0){  // first column of last row
			Node* child = MoveBlank(temp, row, column, "up");
			if (!isExplored(child, explored) && !inFrontier(child, frontier)){
				children.push_back(child);
				child->g++;
				child->parent = n;
				child->action = "up";
			}

			temp = MoveBlank(temp, row, column, "up");

			child = MoveBlank(temp, row, column, "right");
			if (!isExplored(child, explored) && !inFrontier(child, frontier)){
				children.push_back(child);
				child->g++;
				child->parent = n;
				child->action = "right";
			}		
		}
		else if (column == size - 1){  // last column of last row
			Node* child = MoveBlank(temp, row, column, "up");
			if (!isExplored(child, explored) && !inFrontier(child, frontier)){
				children.push_back(child);
				child->g++;
				child->parent = n;
				child->action = "up";
			}

			temp = MoveBlank(temp, row, column, "up");

			child = MoveBlank(temp, row, column, "left");
			if (!isExplored(child, explored) && !inFrontier(child, frontier)){
				children.push_back(child);
				child->g++;
				child->parent = n;
				child->action = "left";
			}		
		}
		else {  // any other column
			Node* child = MoveBlank(temp, row, column, "left");
			if (!isExplored(child, explored) && !inFrontier(child, frontier)){
				children.push_back(child);
				child->g++;
				child->parent = n;
				child->action = "left";
			}

			temp = MoveBlank(temp, row, column, "left");

			child = MoveBlank(temp, row, column, "right");
			if (!isExplored(child, explored) && !inFrontier(child, frontier)){
				children.push_back(child);
				child->g++;
				child->parent = n;
				child->action = "right";
			}		

			temp = MoveBlank(temp, row, column, "right");
			
			child = MoveBlank(temp, row, column, "up");
			if (!isExplored(child, explored) && !inFrontier(child, frontier)){
				children.push_back(child);
				child->g++;
				child->parent = n;
				child->action = "up";
			}
		}	
	}
	else {
		if (column == 0){  // first column 
			Node* child = MoveBlank(temp, row, column, "right");
			if (!isExplored(child, explored) && !inFrontier(child, frontier)){
				children.push_back(child);
				child->g++;
				child->parent = n;
				child->action = "right";
			}

			temp = MoveBlank(temp, row, column, "right");

			child = MoveBlank(temp, row, column, "up");
			if (!isExplored(child, explored) && !inFrontier(child, frontier)){
				children.push_back(child);
				child->g++;
				child->parent = n;
				child->action = "up";
			}		

			temp = MoveBlank(temp, row, column, "up");

			child = MoveBlank(temp, row, column, "down");
			if (!isExplored(child, explored) && !inFrontier(child, frontier)){
				children.push_back(child);
				child->g++;
				child->parent = n;
				child->action = "down";
			}
		}
		else if (column == size - 1){  // last column
			Node* child = MoveBlank(temp, row, column, "left");
			if (!isExplored(child, explored) && !inFrontier(child, frontier)){
				children.push_back(child);
				child->g++;
				child->parent = n;
				child->action = "left";
			}

			temp = MoveBlank(temp, row, column, "left");

			child = MoveBlank(temp, row, column, "up");
			if (!isExplored(child, explored) && !inFrontier(child, frontier)){
				children.push_back(child);
				child->g++;
				child->parent = n;
				child->action = "up";
			}		

			temp = MoveBlank(temp, row, column, "up");

			child = MoveBlank(temp, row, column, "down");
			if (!isExplored(child, explored) && !inFrontier(child, frontier)){
				children.push_back(child);
				child->g++;
				child->parent = n;
				child->action = "down";
			}
		}
		else {  // any other column
			Node* child = MoveBlank(temp, row, column, "left");
			if (!isExplored(child, explored) && !inFrontier(child, frontier)){
				children.push_back(child);
				child->g++;
				child->parent = n;
				child->action = "left";
			}

			temp = MoveBlank(temp, row, column, "left");

			child = MoveBlank(temp, row, column, "right");
			if (!isExplored(child, explored) && !inFrontier(child, frontier)){
				children.push_back(child);
				child->g++;
				child->parent = n;
				child->action = "right";
			}		

			temp = MoveBlank(temp, row, column, "right");

			child = MoveBlank(temp, row, column, "up");
			if (!isExplored(child, explored) && !inFrontier(child, frontier)){
				children.push_back(child);
				child->g++;
				child->parent = n;
				child->action = "up";
			}

			temp = MoveBlank(temp, row, column, "up");

			child = MoveBlank(temp, row, column, "down");
			if (!isExplored(child, explored) && !inFrontier(child, frontier)){
				children.push_back(child);
				child->g++;
				child->parent = n;
				child->action = "down";
			}
		}	
	}	
	return children;
}

void MisplacedTile(Node* n, Node* goal){ 
	int count = 0;

	for (int i = 0; i < n->state.size(); i++) {
		for (int j = 0; j < n->state.size(); j++) {
			if(n->state[i][j] != 0){  // does not consider the blank 
				if (n->state[i][j] != goal->state[i][j]) {  // node state does not match goal
					count++;  // increase count for each misplaced piece
				}
			}
		}
	}
	n->h = count;
}

double rowDist (double i, double j, Node* goal, Node* n){
	double dist = 0;

	for (int k = 0; k < n->state.size(); k++) {
		for (int m = 0; m < n->state.size(); m++) {
			if (n->state[i][j] == goal->state[k][m]) {
				dist = k - i;  // goal state's row number minus node state's row number
			}
		}
	}
	return dist;
}

int colDist (int i, int j, Node* goal, Node* n){
	double dist = 0;

	for (int k = 0; k < n->state.size(); k++) {
		for (int m = 0; m < n->state.size(); m++) {
			if (n->state[i][j] == goal->state[k][m]) {
				dist = m - j; // goal state's column number minus node state's column number
			}
		}
	}
	return dist;
}

void Euclidean (Node*n, Node* goal){
	double col = 0;
	double row = 0;
	double sum = 0;

	for (int i = 0; i < n->state.size(); i++) {
		for (int j = 0; j < n->state.size(); j++) {
			if (n->state[i][j] != 0){  // does not consider the blank
				if (n->state[i][j] != goal->state[i][j]) {
					col = colDist(i, j, goal, n);
					row = rowDist(i, j, goal, n);

					if (col != 0 && row != 0){  // node state is diagonal from goal state  
						col = pow(col, 2);
						row = pow(row, 2);
						sum = sum + sqrt(col + row);
					}
					else if (col != 0){  // node piece needs to go up or down 
						sum = sum + abs(col);
					}
					else{  //node piece needs to go left or right
						sum = sum + abs(row); 
					}
				}
			}
		}
	}
	n->h = sum;
}

int GraphSearch(Problem problem, int input) {	
	// get initial state's path cost
	if (input == 1){
		problem.initialState->pathCost = problem.initialState->g;
	}

	if (input == 2){
		MisplacedTile(problem.initialState, problem.goalState);
		problem.initialState->pathCost = problem.initialState->g + problem.initialState->h;
	}

	if (input == 3){
		Euclidean(problem.initialState, problem.goalState);
		problem.initialState->pathCost = problem.initialState->g + problem.initialState->h;

	}

	priority_queue<Node*, vector<Node*>, ComparePath> frontier;		// initialize frontier with initial state of problem 
	frontier.push(problem.initialState);

	vector<Node*> explored;  // initialize explored set to empty 			

	int numExpanded = 0;  // number of nodes expanded
	int maxQ = 0; // maximum number of nodes in queue at any one time

	cout << "Initial: \n";
	problem.initialState->printNode();
	cout << "Expanding node..." << endl << endl;

	while(1) {
		if (frontier.empty()) {		// check if frontier is empty, return failure
			cout << "Failure! Frontier is empty.\n";
			return 0;
		}
    
		Node* n = new Node(frontier.top());	// choose leaf node, remove from frontier

		frontier.pop();
		if (CompareNode(n, problem.goalState) == 1) {	// compare node and goal state
      		cout << endl;
			cout << "Solution found! Depth: " << n->pathCost << endl;
      		cout << "Path to goal state: " << endl << endl;
			// return solution
			//n->printNode();
			//n->parent->printNode();
			printPath(n);
			
			cout << "To solve this problem the search algorithm expanded a total of " << numExpanded << " nodes.\n";
			cout << "The maximum number of nodes in the queue at any one time: " << maxQ << ".\n";
			return 1;
		}

		explored.push_back(n);		// add node to explored set
		
		//cout << "Expanding state\n";
		vector<Node*> result = Expand(n, explored, frontier);	// expand chosen leaf node, check if is explored before adding to explored
		numExpanded++;  //increment number of nodes expanded 

		if (numExpanded > 1) { //trace
			cout << "The best node to expand with g = " << n->g << ", h = " << n->h << " is..." << endl;
			n->printNode();
			cout << "Expanding node..." << endl << endl;
		}

		// calculate h(n) values and total cost
		if (input == 1){
			for (int i = 0; i < result.size(); i++){	
				result.at(i)->pathCost = result.at(i)->g;
			}	
		}

		if (input == 2){
			for (int i = 0; i < result.size(); i++){	
				MisplacedTile(result.at(i), problem.goalState);
				result.at(i)->pathCost = result.at(i)->g + result.at(i)->h;
			}
		}

		if (input == 3){
			for (int i = 0; i < result.size(); i++){
				Euclidean(result.at(i), problem.goalState);
				result.at(i)->pathCost = result.at(i)->g + result.at(i)->h;
			}		
		}

		for(int i = 0; i < result.size(); i++){	// check if in frontier, add to frontier
			frontier.push(result.at(i));
		}

		// update maximum number of nodes in queue
		if (frontier.size() > maxQ){
			maxQ = frontier.size();
		}

	}
	cout << "end";
	return 0;
}

int main() {
 	int puzzleChoice;
	int input;
	int size;
	int val;

	Node* n1 = new Node();	// test explore set
	vector<int> v1 = { 1, 0, 3 };
	vector<int> v2 = { 4, 2, 6 };
	vector<int> v3 = { 7, 5, 8 };
	n1->state.push_back(v1);
	n1->state.push_back(v2);
	n1->state.push_back(v3);

	Node* n2 = new Node();

	Problem p = Problem(n1);

	cout << "Welcome to Karina's [862104904] & Serena's [862068523] 8 puzzle solver!\n";
	cout << "Type 1 to use a default puzzle, or 2 to enter your own puzzle.\n";
	cin >> puzzleChoice;
	if (puzzleChoice == 1) {	// create problem with default puzzle
		p = Problem(n1);	
	}
	else if (puzzleChoice == 2) {	// create problem with inputted puzzle
		cout << "Enter puzzle size N (NxN): \n";
		cin >> size;
		
    	cout << endl << "Please note that zero will be used for the blank piece." << endl; 
		for (int j = 0; j < size; j++) {
			cout << "Enter row " << j + 1 << " (use spaces inbetween each value): \n";
			vector<int> row;
			for (int i = 0; i < size; i++) {
				cin >> val;
				row.push_back(val);
			}
			n2->state.push_back(row);
		}
		p = Problem(n2);
	}

	cout << "Type 1 for UCS, 2 for A* with Misplaced Tile heuristic, or 3 for A* with Euclidean distance heuristic: "; 
	cin >> input;

	GraphSearch(p, input);

	cin >> input;	// VSC automatically closes terminal so this keeps it open for me

	return 0;
}