// Author: Jacob Christiansen
// Class: CSCI 2270
// Course Instructor: Rhonda Hoenigman
// Recitation: 105
//
// Assignment 9

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <queue>

#include "Graph.hpp"

using namespace std;


Graph::Graph(){
}

Graph::~Graph(){
}

vertex* Graph::findVertex(string name){
	int i = 0;
	while(i < vertices.size()){
		if(vertices[i].name==name){
			return &vertices[i];
		}
		i++;
	}
	return NULL;
}

void Graph::addVertex(string name){
	vertex temp;
	temp.name = name;
	temp.districtID = -1;
	temp.visited = false;
	temp.ID = vertices.size();
	vertices.push_back(temp);
}

void Graph::addEdge(string v1, string v2, int weight){
	adjVertex V2;
	V2.v = Graph::findVertex(v2);
	V2.weight = weight;
	Graph::findVertex(v1)->adj.push_back(V2);
}

void Graph::displayEdges(){
	for(int i = 0; i < vertices.size(); i++){
		cout<<vertices[i].districtID<<":";
		cout<<vertices[i].name<<"-->";
		for(int j = 0; j < vertices[i].adj.size(); j++){
			if(j != vertices[i].adj.size() - 1){
				cout<<vertices[i].adj[j].v->name<<"***";
			}
			else{
				cout<<vertices[i].adj[j].v->name<<endl;
			}
		}
		cout<<endl;
	}
}

void Graph::assignDistricts(){
	int distr = 1;
	for(int i=0; i<vertices.size(); i++){
		while(vertices[i].districtID==-1){
			DFSLabel(vertices[i].name, distr);
			distr++;

		}
	}
}

void Graph::DFSLabel(string startingCity, int distID){
	vertex *city = findVertex(startingCity);
	city->visited = true;
	stack <vertex*> cStack;
	cStack.push(city);

	while(cStack.empty()==false){
		vertex *newCity = cStack.top();
		newCity->districtID = distID;
		cStack.pop();

		for(int i=0; i<newCity->adj.size(); i++){
			if(newCity->adj[i].v->visited==false && newCity->adj[i].v->districtID==-1){
				newCity->adj[i].v->visited = true;
				newCity->adj[i].v->districtID = distID;
				cStack.push(newCity->adj[i].v);
			}
		}
	}
}


void Graph::shortestPath(string startingCity, string endingCity){
	vertex* start = findVertex(startingCity);
	vertex* end = findVertex(endingCity);
	if(start==NULL || end==NULL){
		cout<<"One or more cities doesn't exist"<<endl;
		return;
	}
	if(start->districtID==-1 || end->districtID==-1){
		cout<<"Please identify the districts before checking distances"<<endl;
		return;
	}
	if(start->districtID != end->districtID){
		cout<<"No safe path between cities"<<endl;
		return;
	}
	else if(start->districtID==end->districtID){
		for(int i=0; i<vertices.size(); i++){
			vertices[i].visited = false;
			vertices[i].parent = NULL;
			vertices[i].unweightedDistance = 99999999;
		}
		start->visited = true;
		start->unweightedDistance = 0;
		queue<int> cQ;
		cQ.push(start->ID);

		while(!cQ.empty()){
			int temp = cQ.front();
			cQ.pop();

			for(int j=0; j<vertices[temp].adj.size(); j++){
				if(vertices[temp].adj[j].v->unweightedDistance > vertices[temp].unweightedDistance){
					vertices[temp].adj[j].v->unweightedDistance = vertices[temp].unweightedDistance + 1;
					vertices[temp].adj[j].v->parent = &vertices[temp];
				}
				if(vertices[temp].adj[j].v->visited==false){
					cQ.push(vertices[temp].adj[j].v->ID);
					vertices[temp].adj[j].v->visited = true;
				}
			}
		}

		int j = end->ID;
		int numEdges = 0;
		vector<int> thePath;

		while(vertices[j].name != start->name){
			numEdges++;
			thePath.push_back(j);
			j = vertices[j].parent->ID;
		}
		cout<<numEdges<<", "<< start->name;
		for(int j=thePath.size()-1; j>=0; j--){
			cout<<", "<<vertices[thePath[j]].name;
		}
		cout<<endl;
	}
}



void Graph::shortestWeightedPath(string startingCity, string endingCity){
	vertex* start = findVertex(startingCity);
	vertex* end = findVertex(endingCity);
	if(start==NULL || end==NULL){
		cout<<"One or more cities doesn't exist"<<endl;
		return;
	}
	if(start->districtID==-1 || end->districtID==-1){
		cout<<"Please identify the districts before checking distances"<<endl;
		return;
	}
	if(start->districtID != end->districtID){
		cout<<"No safe path between cities"<<endl;
		return;
	}
	else if(start->districtID==end->districtID){
		for(int i=0; i<vertices.size(); i++){
			vertices[i].visited = false;
			vertices[i].weightedDistance = 99999999;
			vertices[i].parent = NULL;
		}

		start->visited = true;
		start->weightedDistance = 0;
		queue<int> cQ;
		cQ.push(start->ID);

		while(cQ.empty()==false){
			int temp = cQ.front();
			cQ.pop();
			for(int j=0; j<vertices[temp].adj.size(); j++){
				if(vertices[temp].adj[j].v->weightedDistance > vertices[temp].adj[j].weight + vertices[temp].weightedDistance){
					vertices[temp].adj[j].v->weightedDistance = vertices[temp].adj[j].weight + vertices[temp].weightedDistance;
					vertices[temp].adj[j].v->parent = &vertices[temp];
				}
				if(vertices[temp].adj[j].v->visited==false){
					cQ.push(vertices[temp].adj[j].v->ID);
					vertices[temp].adj[j].v->visited = true;
				}

			}
		}
		int j = end->ID;
		vector<int> thePath;
		while(vertices[j].name != start->name){
			thePath.push_back(j);
			j=vertices[j].parent->ID;
		}

		cout<<end->weightedDistance<<", "<<start->name;

		for(int j = thePath.size()-1; j >= 0; j--){
			cout<<", "<<vertices[thePath[j]].name;
		}
		cout<<endl;
	}

}

int main(int argc, char *argv[]){
	Graph cityGraph;
	ifstream zombieCities;
	string cityData;
	vector<string> cities;
	zombieCities.open(argv[1]);

	if(zombieCities.is_open()){
		int numCities = 0;
		int dist;
		int counter = 0;
		string city, temp;
		getline(zombieCities, cityData);
		stringstream ss(cityData);
		int x = 0;

		while(getline(ss, city, ',')){
			if(x==1){
				cities.push_back(city);
				numCities++;
				cityGraph.addVertex(city);
			}
			x = 1;
		}

		while(getline(zombieCities, cityData)){
			stringstream ss(cityData);
			int x = 0;

			while(getline(ss, city, ',')){
				if(x != 0){
					stringstream ss2(city);
					ss2>>dist;
					if(dist>0){
						cityGraph.addEdge(cities[counter], temp, dist);
					}
					counter++;
				}else{
					counter = 0;
					temp = city;
					x = 1;
				}
			}
		}
	}

	int inputVal = 0;
	while(inputVal<5){
		cout<<"======Main Menu======"<<endl;
		cout<<"1. Print vertices"<<endl;
		cout<<"2. Find districts"<<endl;
		cout<<"3. Find shortest path"<<endl;
		cout<<"4. Find shortest weighted path"<<endl;
		cout<<"5. Quit"<<endl;
		cin>>inputVal;
		if(inputVal==1){
			cityGraph.displayEdges();
		}else if(inputVal==2){
			cityGraph.assignDistricts();
		}else if(inputVal==3){
			string start;
			string end;
			cout<<"Enter a starting city:"<<endl;
			cin.ignore();
			getline(cin, start);
			cout<<"Enter an ending city:"<<endl;
			getline(cin, end);
			cityGraph.shortestPath(start, end);
		}else if(inputVal==4){
			string start;
			string end;
			cout<<"Enter a starting city:"<<endl;
			cin.ignore();
			getline(cin, start);
			cout<<"Enter an ending city:"<<endl;
			getline(cin, end);
			cityGraph.shortestWeightedPath(start, end);
		}else if(inputVal==5){
			cout<<"Goodbye!"<<endl;
			return 0;
		}
	}
}