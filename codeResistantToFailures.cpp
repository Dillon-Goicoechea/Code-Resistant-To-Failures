

#include <iostream>
#include <cstdlib>
#include <list>
#include <vector>
#include <tuple>
using namespace std;
class TwoPhaseCommit {
public:
    list<char*> dataSet;
    list<char*>::iterator iter = dataSet.begin();
    int prevCommitIndex=0;
    int currCommitIndex=0;
    vector<tuple<char, int, char[5]>> transactionLog;

    //Allocate memory here in the program
    void PhaseOne() {
        try {
            for (tuple<char, int, char[5]> command : transactionLog) {
                if (get<0>(command) == 'a') {
                    char* tempArray = new char[5];
                    tempArray = get<2>(command);
                    Add(get<1>(command), tempArray);
                }
            };
            Commit();
        }
        catch (bad_alloc& ex) {
            cout << "Issues with memory allocation" << endl;
        }

    }

    void PhaseTwo() {
        prevCommitIndex = currCommitIndex; 
        currCommitIndex = static_cast<int>(transactionLog.size());
        for (tuple<char, int, char[5]> command : transactionLog) {
            if (get<0>(command) == 'd') {
                Delete(get<1>(command));
            }
        };

    }

    void Add(int iKey, char* string1) {
        advance(iter, iKey);
        dataSet.insert(iter, string1);
        iter = dataSet.begin();
    }
    void Delete(int iKey) {
        advance(iter, iKey);
        char* tempPtr = *iter;
        dataSet.erase(iter);
        delete tempPtr; 
        iter = dataSet.begin();
    }
    void Commit() {
        cout << "Phase one completed succesfully, commiting to phase two of transaction" << endl;
        PhaseTwo();
    }
    void Rollback() {
        try {
            for (int i = currCommitIndex; i > prevCommitIndex; i--) {
                if (get<0>(transactionLog.at(i)) == 'd') {
                    char* tempArray = new char[5];
                    tempArray = get<2>(transactionLog.at(i)); 
                    Add(get<1>(transactionLog.at(i)), tempArray);
                }
                if (get<0>(transactionLog.at(i)) == 'a') {
                    Delete(get<1>(transactionLog.at(i)));
                } 
            };
        }
        catch (bad_alloc& ex) {
            cout << "Issues with memory allocation" << endl;
        }
    }
};
int main(){
    char test[]="d";
    TwoPhaseCommit mainObj;

}


