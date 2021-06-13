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

// descending order of fee
bool compareTransactionMaxFee(Transaction t1, Transaction t2){
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

// ascending order of weight
bool compareTransactionMinWeight(Transaction t1, Transaction t2){
    if(t1.getWeight() < t2.getWeight()){
        return true;
    }
    else if(t1.getWeight() > t2.getWeight()){
        return false;
    }
    else{
        if(t1.getFee() < t2.getFee()){
            return false;
        }
        else{
            return true;
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

// for sorting the transactions 
void sortTransactions(vector<Transaction> &dataFee, vector<Transaction> &dataWeight){
    int n = dataFee.size();
    for(int i=0;i<n;i++){
        Transaction current_max_fee = dataFee[i], current_min_weight = dataWeight[i];
        int swapPosition_fee = i, swapPosition_weight = i;
        for(int j=i+1;j<n;j++){
            if(!compareTransactionMaxFee(current_max_fee,dataFee[j])){
                current_max_fee = dataFee[j];
                swapPosition_fee = j;
            }
            if(!compareTransactionMinWeight(current_min_weight,dataWeight[j])){
                current_min_weight = dataWeight[j];
                swapPosition_weight = j;
            }
        }
        dataFee[swapPosition_fee] = dataFee[i];
        dataFee[i] = current_max_fee;
        dataWeight[swapPosition_weight] = dataWeight[i];
        dataWeight[i] = current_min_weight;
    }
}

int main(){
    vector<Transaction> dataSortedForFee = transactionFileParser("mempool.csv", entries);
    vector<Transaction> dataSortedForWeight = dataSortedForFee;
    sortTransactions(dataSortedForFee,dataSortedForWeight);
    for(int i=0;i<dataSortedForFee.size();i++){
        cout<<i+1<<"-> ";
        cout<<"Sorted Fee :"<<dataSortedForFee[i].getFee()<<" "<<dataSortedForFee[i].getWeight()<<"  ";
        cout<<"Sorted Weight :"<<dataSortedForWeight[i].getFee()<<" "<<dataSortedForWeight[i].getWeight()<<"  ";
        cout<<endl;

    }
    // cout<<data.size()<<endl;
}