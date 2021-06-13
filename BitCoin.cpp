#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include "Transaction.h"
using namespace std;
const int maxWeight = 4000000;
const int entries = 5214;

// comparator function to comare transactions for sorting
bool compareTransaction(Transaction t1, Transaction t2){
    if(t1.getFee() < t2.getFee()){
        return false;
    }
    else if(t1.getFee() > t2.getFee()){
        return true;
    }
    else{
        if(t1.getWeight() < t2.getWeight()){
            return true;
        }
        else{
            return false;
        }
    }
}

// custom file reader for returning data in required format
vector<Transaction> transactionFileParser(string fileName, int numberOfEntries){
    vector<Transaction> data;
    fstream fin;
    fin.open(fileName,ios::in);
    string line,word;
    getline(fin,line); // for ignoring the row having column names
    int count = numberOfEntries;
    while(count--){
        getline(fin,line);
        stringstream s(line);
        vector<string> row;
        while(getline(s,word,',')){
            row.push_back(word);
        }
        string tx_id = row[0];
        int fee = stoi(row[1]);
        int weight = stoi(row[2]);
        vector<string> parentTransactions;
        if(row.size() == 4){
            stringstream r(row[3]);
            string ptx_id;
            while(getline(r,ptx_id,';')){
                parentTransactions.push_back(ptx_id);
            }
        }
        Transaction t(tx_id,fee,weight,parentTransactions);
        data.push_back(t);
    }
    return data;
}

int main(){
    vector<Transaction> data = transactionFileParser("mempool.csv", entries);
    // for(int i=0;i<data.size();i++){
    //     cout<<i+1<<" : ";
    //     data[i].printDetails();
    // }
    // cout<<data.size()<<endl;
}