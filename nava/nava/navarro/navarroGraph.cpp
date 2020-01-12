#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <cmath>
#include <sys/time.h>
#include <vector>  
#include <chrono> 
#include <unordered_map>
#include <iterator> 
#include <map>
#include <algorithm>
#include <memory>
#include <set>


#include "fastqloader.h"
#include "GfaGraph.h"
#include "Node.h"

using namespace std::chrono;
using namespace std;

int match = -2;
int mismatch = 1;
int indel = 1;

//pretvorit u unordered map <node, int>
//vector<int> previous_row;
//vector<int> next_row;
unordered_map <NodePosition, int> previous_row;
unordered_map <NodePosition, int> next_row;
unordered_map <int, string> backtrack_map;
unordered_map <NodePosition, int> result_row;

int similarityScore(string current_char, string current_node, unordered_map <NodePosition, int> previous_row, unordered_map <NodePosition, int> next_row, int j);
std::pair<int, int> findMin(unordered_map <NodePosition, int> row);
int Navarro(string A, unordered_set<NodePosition> B);
int reconstruction(string current_char, string current_node);
void bactrack(GfaGraph graph, int positionGraph, int total_distance, string sequence, int positionSeq, int reconstructed_length);

/*
int similarityScore(char a, char b, int position){
	if(a == b)
	{
		return match + previous_row[position-1];
	}
	else
	{
		int option1 = previous_row[position-1] + mismatch;
		int option2 = previous_row[position] + indel;
		int option3 = previous_row[position-1] + indel;
		return min(option1, option2, option3);
	}
}*/

int similarityScore(string current_char, string current_node, unordered_map <NodePosition, int> previous_row, unordered_map <NodePosition, int> next_row, int j){
	if(current_char == current_node){
		return previous_row[j-1];
	}
	else{
		return min(previous_row[j], previous_row[j-1], next_row[j-1]) + 1;
	}
}

std::pair<int, int> findMin(unordered_map <NodePosition, int> row){

	map<string, int>::iterator it;
	int min = row[0];
	int position;             

	for(it=row.begin(); it!=row.end();it++){
		if(it.second <= min){
			position = it.first;
			min = it.second;
		}
	}

	/*int length = (sizeof(array)/sizeof(*array));
	int min = array[0];
	int position = 0;

	for(int i=1; i<length; i++)
	{
		if(array[i] < min)
		{
			min = array[i];
			position = i;
		}
	}*/
 	return std::make_pair(position, min);
}

int reconstruction(string current_char, string current_node){

	if(current_char == current_node){
		return 0;
	}
	else{
		return 1;
	}

}

void bactrack(GfaGraph graph, int positionGraph, int total_distance, string sequence, int positionSeq, int reconstructed_length){

	if(total_distance == 0 && reconstructed_length == sequence){
		return;
	}

	total_distance = total_distance - reconstruction(sequence[positionSeq], graph.vertices[positionGraph]);

	backtrack_map.insert(positionSeq, sequence[positionSeq]);

	bactrack(graph, positionGraph-1, total_distance, sequence, positionSeq-1, reconstructed_length+1);

}

// V, E, patt
// preko Cv, C'v

int Navarro(FastQ A, std::unordered_set<NodePosition> B, GfaGraph *gfaGraph){
	string current_char;
	// string current_node;

	int lengthSeqA = A.sequence.size();
	//int lengthGraphB = B.size();

	pair<int, int> result;

	result_row.clear();

	/*for(int i=0; i<lengthGraphB; i++){
		//previous_row[i]=0;
		previous_row.insert(i,0);
	}*/

	for(auto node: B){
		previous_row.insert(node,0);
	}

	for(int i=1; i<lengthSeqA; i++){
		current_char = A.sequence[i-1];
		// next_row.insert(i,i); //ovo me jebe masu

		/*for(int j=1; j<lengthGraphB; j++){
			// nodes ili allNodes - primjenit na pavin kod
			current_node = B.vertices.find(j);
			//saljemo 
			int minimum = similarityScore(current_char, current_node, previous_row, next_row, j);
			//int minimum = similarityScore(current_char,current_node, j);
			next_row.insert(j,minimum);
		}*/

		for(auto no: B){
			// current_node = no.getCurrentChar();

			next_row[no] = g(no, i, current_char, gfaGraph, previous_row );

			// next_row.insert(no, minimum);
		}
		for(auto nod: B){
			previous_row[nod] = next_row[nod];
		}

		int min = 1 << 30u;

		for(auto nodescore: previous_row){
			std::cout << nodeScore.first << " ->" << nodeScore.second << std::endl;
        	min = std::min(nodeScore.second, min);

		}

		//previous_row = next_row;
		// previous_row.clear();
		// previous_row.insert(next_row.begin(), next_row.end());

		/*int* array = &next_row[0];
		int minRow = findMin(array);
		bactrack[i] = minRow;*/

		// next_row.clear();
	}

	// result_row.insert(previous_row.begin(), previous_row.end());
	// result = findMin(result_row);

	// bactrack(B, result.first, result.second, A, A.size()-1, 0);

	return min;
}

int g(NodePosition node, int i, char c, GfaGraph *gfaGraph, std::unordered_map<NodePosition, int> &cv){
	if (c == node.getCurrentChar()) {
        int min = i - 1;
        for (auto &parent : node.previous(gfaGraph)) {
            min = std::min(cv[parent], min);
        }
        return min;
    } else {
        auto parents = node.previous(gfaGraph);
        if (parents.empty()) {
            return i;
        }
        int min = 1u << 30u;
        for (auto &parent : node.previous(gfaGraph)) {
            min = std::min(cv[parent], min);
        }
        return 1 + min;
    }

}

int main(int argc, char** argv){

	//1. FastQ result: seuquence, seq_id, quality
	//2. GraGraph result: nodes, edgeOverlap, edges

	//argv = name of the program + arguments
	// program, filename for sequences, filename for graph, scores

	string fileSeq = argv[1];
	string fileGraph = argv[2];

	int match = atoi(argv[3]);
	int mismatch = atoi(argv[4]);
	int indel = atoi(argv[5]);

	std::vector<FastQ> sequences = loadFastqFromFile(fileSeq);
	//GfaGraph graph = GfaGraph::LoadFromFile(fileGraph);
	//GfaGraph graph = *GfaGraph::loadFromFile(fileGraph);

	//int lengthGraph = graph.vertices.size();  //.nodes.size();

	std::string inputDir = R"(/home/toni/BioINF/nava/input/)";
    std::cout << inputDir << std::endl;
    graph = std::unique_ptr<GfaGraph>(GfaGraph::loadFromFile(
//            inputDir + "ref10000_onechar.gfa"
//            inputDir + "ref10000_tangle.gfa"
//            inputDir + "ref10000_snp.gfa"
//            inputDir + "ref10000_twopath.gfa"
            inputDir + "example.gfa"
    ));
    int n = graph->vertices.size();

    //samo obicni nodeovi
    // for (int i = 1; i <= n; ++i) {
    //     Node node = graph->vertices[i];
    //     std::cout << node.id << "(" << node.sequence << "): ";
    //     for (auto &it : node.outEdges) {
    //         Node child = graph->vertices[it.to];
    //         std::cout << child.id << "(" << child.sequence << "), ";
    //     }
    //     std::cout << std::endl;
    // }

    // posebni nodeovi koji idu char po char
    std::unordered_set<NodePosition> front;
    for (auto &it: graph->vertices) {
        Node node = it.second;
        for (auto &e: node.inEdges) {
            front.insert(NodePosition(node, e, true));
        }
    }
    std::unordered_set<NodePosition> allPos;
    allPos.insert(front.begin(), front.end());
    bool addedNew;
    do {
        addedNew = false;
        std::unordered_set<NodePosition> frontNew;
        for (auto np : front) {
            for (auto &newNP : np.previous(graph.get())) {
                bool isIn = allPos.find(newNP) != allPos.end();
                if(!isIn){
                    addedNew = true;
                    frontNew.insert(newNP);
                }
            }
        }
        // std::cout << "size: " << frontNew.size() << std::endl;
        // for(auto p : frontNew){
        //     std::cout << p << std::endl;
        // }
        front.clear();
        front.insert(frontNew.begin(), frontNew.end());
        allPos.insert(front.begin(), front.end());
    } while (addedNew);
    //allPos su svi node-ovi onda se na njima radi previous i sto vec treba
    //preporucam za onaj Cv radit novu unordered_map<NodePosition, int> pa imat Cv i Cv' mape za racunanje 'retka' i novog 'retka'
    // std::cout << "All node(char): " << std::endl;
    // for (auto nodePos: allPos) {
    //     std::cout << nodePos << std::endl << std::endl;
    //     for(auto parent : nodePos.previous(graph.get())){
    //         std::cout << parent << " ";
    //     }std::cout << std::endl << "end" << std::endl;
    // }



	//allPos su svi node-ovi onda se na njima radi previous i sto vec treba
    //preporucam za onaj Cv radit novu unordered_map<NodePosition, int> 
    //pa imat Cv i Cv' mape za racunanje 'retka' i novog 'retka'


	auto start = high_resolution_clock::now();

	for(int index=0; index<sequences.size(); index++){

		//string sequence = sequences[index].sequence;
		int result = Navarro(sequences[index], allPos, graph.get());

		/*for(int i=0; i<lengthGraph; i++){
			cout << previous_row[i] << " ";
			cout << endl;
		}

		for(int i=0; i<lengthGraph; i++){
			cout << next_row[i] << " ";
			cout << endl;
		}*/
		
		cout << "Min distance is: " << result << endl;
	}

	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start); 
	cout << duration.count() << endl; 

	/*ofstream myfile;
	myfile.open ("output.txt");
	if(myfile.is_open()) { 
		myfile <<  "Duration of algoritm: " << duration << endl;
		myfile.close();
	}
	else cout << "Unable to open the file.";*/

	return 0;
}
