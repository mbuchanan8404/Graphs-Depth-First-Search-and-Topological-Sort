///*
//*  Matthew Buchanan
//*  Project 3
//*  CS-340
//*  Graphs, Depth First Search, and Topological Sort
//*/

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <cassert>
#include <sstream>
#include <queue>
using namespace std;

/* Vertex Class */
class Vertex
{
public:
	int id;
	int color; // 0 = white, 1 = gray, 2 = black
	int d;
	int f;
	vector<int> adjacencies;

	Vertex()
	{
	};

	~Vertex()
	{
	};
};

/* Graph class */
class Graph
{
public:
	vector<Vertex> vertices;
	vector<Vertex> cycle;//holds back edges
	queue<Vertex> topSort;// holds results of topological sort
	int time;
	bool isDAG; // flag variable to mark if graph has a cycle

	Graph()
	{
	};

	~Graph()
	{
	};

	/* Depth first search function */
	void depthFirstSearch()
	{
		for (int i = 0; i < vertices.size(); i++)// set all vertices to white
			vertices[i].color = 0;
		isDAG = 1;
		time = 0;
		for (int i = 0; i < vertices.size(); i++)
		{
			if (!(vertices[i].color))
				dfsVisit(vertices[i]);
		}
	}

	/* Depth first search helper function */
	void dfsVisit(Vertex &u)
	{
		time++;
		u.d = time;
		u.color = 1;
		for (int i = 0; i < u.adjacencies.size(); i++)//for each adjacency of u
		{
			if (vertices[u.adjacencies[i] - 1].color == 0)// if adajency of u is white recurse on u's adjacent vertex
				dfsVisit(vertices[u.adjacencies[i] - 1]);
		}
		u.color = 2;// color u black
		time++;
		u.f = time;// finalize timestamp
	}

	/* Topological sort function */
	void topologicalSort()
	{
		for (int i = 0; i < vertices.size(); i++)// set all vertices to white
			vertices[i].color = 0;
		isDAG = 1;
		time = 0;
		for (int i = 0; i < vertices.size(); i++)
		{
			if (!(vertices[i].color))
				topVisit(vertices[i]);
		}
	}

	/* Topological sort helper function */
	void topVisit(Vertex &u)//note all back edges will be stored rather than returning false once one is detected
	{
		time++;
		u.d = time;
		u.color = 1;
		for (int i = 0; i < u.adjacencies.size(); i++)//for each adjacency of u
		{
			if (vertices[u.adjacencies[i] - 1].color == 1)// if adjacency is gray, back edge detected 
			{
				isDAG = false;// set flag for not a DAG
				cycle.push_back(u);// add the two vertices of the back edge to cycle
				cycle.push_back(vertices[u.adjacencies[i] - 1]);
			}
			if (vertices[u.adjacencies[i] - 1].color == 0)// // if adajency of u is white recurse on u's adjacent vertex
				topVisit(vertices[u.adjacencies[i] - 1]);
		}
		u.color = 2;// color u black
		time++;
		u.f = time;// finalize timestamp
		topSort.push(u);
	}
};

/* Function prototypes */
void readFile(string f, Graph * g);
void displayGraph(Graph g);
void displayResults(Graph g);
string getFileName();


/**************** Begin main ****************/
int main()
{
	bool run = true;
	while (run)
	{
		Graph * newGraph = new Graph;
		readFile(getFileName(), newGraph);
		displayGraph(*newGraph);
		newGraph->topologicalSort();
		displayResults(*newGraph);
		delete newGraph;
		cout << "Enter 1 to continue, 0 to quit:" << endl;
		run = false;
		cin >> run;
		cout << endl;
	}
}
/***************** End Main ******************/


/* Function to retrieve input file name from user */
string getFileName()//error catch this-----------------------------------------!!!!!!!!!!!!!!!!!!!!remove
{
	string fileName;
	cout << "Please input the full name of the file you wish to build a graph from, for example: graphin.txt" << endl;
	cin >> fileName;
	cout << endl;
	return fileName;
}

/* Function to read an input .txt file and build the graph */
void readFile(string f, Graph * g)
{
	ifstream in;
	in.open(f);
	assert(in.is_open());
	int i = 0;
	int adjacency;
	string line;
	char temp2;
	while (!in.eof())
	{
		Vertex newVertex;
		g->vertices.push_back(newVertex);
		in >> g->vertices[i].id >> temp2;//assign node id and discard colon into temp2
		getline(in, line);
		istringstream is(line);
		while (is >> adjacency)//use string stream to parse ints from line of text
		{
			g->vertices[i].adjacencies.push_back(adjacency);
		}
		i++;
	}
	in.close();
	in.clear();
	
	return;
}

/* Function to display the graph in same format as input file */
void displayGraph(Graph g)
{
	cout << "Input graph:" << endl;
	for (int i = 0; i < g.vertices.size(); i++)
	{
		cout << g.vertices[i].id << ": ";
		for (int j = 0; j < g.vertices[i].adjacencies.size(); j++)
		{
			cout << g.vertices[i].adjacencies[j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}

/* Function to display final results of DFS / Topological Sort */
void displayResults(Graph g)
{
	if (!g.isDAG)// Inform user if a cycle was found
	{
		cout << "Graph is not a DAG " << endl << "Back-edges:";
		int counter = g.cycle.size();
		for (int l = 0; l < counter; l++)//display all back edges as ordered pairs of vertices with arrow indicating path direction
		{
			if (l % 2 == 0)
				cout << endl;
			if (l % 2 || l % 2 == 2)
				cout << "-> ";
			cout << g.cycle[l].id << " ";
		}
	}
	else 
	{
		cout << "Graph is a DAG." << endl << "Topological sort results:" << endl;
		vector<Vertex> outputTemp;
		while (!g.topSort.empty())// transfer topological sort vertices into temp array for correct output
		{
			outputTemp.push_back(g.topSort.front());
			g.topSort.pop();
		}
		for (int i = outputTemp.size(); i > 0; i--)
			cout << "Vertice ID: " << outputTemp[i - 1].id << " Finish Time " << outputTemp[i - 1].f << " " << endl;
	}
	cout << endl << endl;
}