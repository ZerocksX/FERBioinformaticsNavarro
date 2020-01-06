#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <cmath>
#include <sys/time.h>
#include <vector>  
#include <chrono> 
using namespace std::chrono;
using namespace std;

int match = -2;
int mismatch = 1;
int indel = 1;

vector<int> previous_row;
vector<int> next_row;

int similarityScore(char a, char b);
int findMin(double array[], int length);

int similarityScore(char a, char b, int position)
{
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
}

int findMin(int array[])
{
	int length = (sizeof(array)/sizeof(*array));
    int min = array[0];
    int position = 0;

    for(int i=1; i<length; i++)
    {
        if(array[i] < min)
        {
            min = array[i];
            position=i;
        }
    }
    return min;
}

//int Navarro(string A, graph B)
int Navarro(string A, string B)
{
	string current_char;
	string current_node;

	int lengthSeqA = A.size();
	int lengthGraphB = B.size();

	for(int i=0;i<lengthGraphB;i++){
		previous_row[i]=0;
	}

	for(int i=1;i<lengthSeqA;i++){
		current_char = A[i];
		next_row.push_back(i+1);

		for(int j=1;j<lengthGraphB;j++){
			current_node = B[j];
			int minimum = similarityScore(current_char,current_node, j);
			next_row.push_back(minimum);
		}

		previous_row = next_row;
		next_row.clear();
	}

	//vracati element ili pointer na element?
	int* array = &next_row[0];
	return findMin(array);
}

int main(int argc, char** argv)
{
    string seqA;
    string seqB;

    cout << "Sequence A" << endl;
    cin >> seqA;
    cout << "Sequence B" << endl;
    cin >> seqB;
    cout << "You typed in " << endl << seqA << endl << seqB << endl;
    
    auto start = high_resolution_clock::now();

    int result = Navarro(seqA, seqB);

    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(stop - start); 
    cout << duration.count() << endl; 

    int lengthSeqA = seqA.length();
    int lengthSeqB = seqB.length();

    for(int i=0;i<lengthSeqB;i++)
    {
        cout << previous_row[i] << " ";
        cout << endl;
    }

    for(int i=0;i<lengthSeqB;i++)
    {
        cout << next_row[i] << " ";
        cout << endl;
    }
    
    cout << "Min score is " << result << endl;

    ofstream myfile;
    myfile.open ("output.txt");
    if(myfile.is_open()) { 
		myfile << "This is a line.\n";
		myfile <<  "Duration of algoritm: " << duration << endl;
		myfile.close();
	}
    else cout << "Unable to open file";

    return 0;
}
