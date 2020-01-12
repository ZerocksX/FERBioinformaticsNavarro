#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <cmath>
#include <sys/time.h>
using namespace std;


//extern struct timeval   StartTime, EndTime;
//int ind; 
//double penalty=-1;

int match = 2;
int mismatch = -1;
int indel = -1;

vector<int> previous_row;
vector<int> next_row;


double similarityScore(char a, char b);
double findMax(double array[], int length);

/*double similarityScore(char a, char b)
{
    double result;
    if(a==b)
    {
        result=2;
    }
    else
    {
        result=penalty;
    }
    return result;
}*/

int similarityScore(char a, char b)
{
    if(a == b)
    {
        return match + previous_row[id-1];
    }
    else
    {
    	option1 = previous_row[id-1] + mismatch
    	option2 = previous_row[id] + indel
    	option3 = previous_row[id-1] + indel
        return min(option1, option2, option3)
    }
}

int Navarro(string A, graph B)
{

	string current_char;
	string current_node;

	lengthSeqA = A.size()
	lengthGraphB = B.size()

}




double findMax(double array[], int length)
{
    double max = array[0];
    ind = 0;

    for(int i=1; i<length; i++)
    {
        if(array[i] > max)
        {
            max = array[i];
            ind=i;
        }
    }
    return max;
}

int main()
{
    string seqA;
    string seqB;
    cout << "Sequence A" << endl;
    cin >> seqA;
    cout << "Sequence B" << endl;
    cin >> seqB;
    cout << "You typed in " << endl << seqA << endl << seqB << endl;
    
    //gettimeofday(&StartTime, NULL);

    int lengthSeqA = seqA.length();
    int lengthSeqB = seqB.length();
    
    // initialize matrix
    double matrix[lengthSeqA+1][lengthSeqB+1];
    for(int i=0;i<=lengthSeqA;i++)
    {
        for(int j=0;j<=lengthSeqB;j++)
        {
            matrix[i][j]=0;
        }
    }

    double traceback[4];
    int I_i[lengthSeqA+1][lengthSeqB+1];
    int I_j[lengthSeqA+1][lengthSeqB+1];

    //start calculating matrix
    for (int i=1;i<=lengthSeqA;i++)
    {
        for(int j=0;j<=lengthSeqB;j++)
        {
            cout << i << " " << j << endl;
            traceback[0] = matrix[i-1][j-1]+similarityScore(seqA[i-1],seqB[j-1]);
            traceback[1] = matrix[i-1][j]+penalty;
            traceback[2] = matrix[i][j-1]+penalty;
            traceback[3] = 0;
            matrix[i][j] = findMax(traceback,4);
            switch(ind)
            {
                case 0:
                    I_i[i][j] = i-1;
                    I_j[i][j] = j-1;
                    break;
                case 1:
                    I_i[i][j] = i-1;
                    I_j[i][j] = j;
                    break;
                case 2:
                    I_i[i][j] = i;
                    I_j[i][j] = j-1;
                    break;
                case 3:
                    I_i[i][j] = i;
                    I_j[i][j] = j;
                    break;
            }
        }
    }

    // print the scoring matrix to console
    for(int i=1;i<lengthSeqA;i++)
    {
        for(int j=1;j<lengthSeqB;j++)
        {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }

    // find the max score in the matrix
    double matrix_max = 0;
    int i_max=0, j_max=0;
    for(int i=1;i<lengthSeqA;i++)
    {
        for(int j=1;j<lengthSeqB;j++)
        {
            if(matrix[i][j]>matrix_max)
            {
                matrix_max = matrix[i][j];
                i_max=i;
                j_max=j;
            }
        }
    }

    cout << "Max score in the matrix is " << matrix_max << endl;

    // traceback
    int current_i=i_max,current_j=j_max;
    int next_i=I_i[current_i][current_j];
    int next_j=I_j[current_i][current_j];
    int tick=0;
    char consensus_a[lengthSeqA+lengthSeqB+2],consensus_b[lengthSeqA+lengthSeqB+2];

    while(((current_i!=next_i) || (current_j!=next_j)) && (next_j!=0) && (next_i!=0))
    {

        if(next_i==current_i)  consensus_a[tick] = '-';                  // deletion in A
        else                   consensus_a[tick] = seqA[current_i-1];   // match/mismatch in A

        if(next_j==current_j)  consensus_b[tick] = '-';                  // deletion in B
        else                   consensus_b[tick] = seqB[current_j-1];   // match/mismatch in B

        current_i = next_i;
        current_j = next_j;
        next_i = I_i[current_i][current_j];
        next_j = I_j[current_i][current_j];
        tick++;
    }


    //gettimeofday(&EndTime, NULL);

    //print the consensus sequences
    cout<<endl<<" "<<endl;
    cout<<"Alignment:"<<endl<<endl;
    for(int i=0;i<lengthSeqA;i++){cout<<seqA[i];}; cout<<"  and"<<endl;
    for(int i=0;i<lengthSeqB;i++){cout<<seqB[i];}; cout<<endl<<endl;  
    for(int i=tick-1;i>=0;i--) cout<<consensus_a[i]; 
    cout<<endl;
    for(int j=tick-1;j>=0;j--) cout<<consensus_b[j];
    cout<<endl;

    return 0;
}
/*
    if (EndTime.tv_usec < StartTime.tv_usec) {
        int nsec = (StartTime.tv_usec - EndTime.tv_usec) / 1000000 + 1;
        StartTime.tv_usec -= 1000000 * nsec;
        StartTime.tv_sec += nsec;
    }
    if (EndTime.tv_usec - StartTime.tv_usec > 1000000) {
        int nsec = (EndTime.tv_usec - StartTime.tv_usec) / 1000000;
        StartTime.tv_usec += 1000000 * nsec;
        StartTime.tv_sec -= nsec;
    }
    printf("\n\nParallel calculation time: %ld.%.6ld seconds\n", EndTime.tv_sec  - StartTime.tv_sec, EndTime.tv_usec - StartTime.tv_usec);

    return 0;
}

*/