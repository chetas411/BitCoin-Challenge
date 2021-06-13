#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
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

void writeTransactionsToFile(vector<string> data, string fileName){
    fstream fout;
    fout.open(fileName, ios::app | ios::out);
    for(auto s : data){
        fout<<s<<endl;
    }
    fout.close();
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

vector<string> getBlockData(vector<Transaction> data, int &totalFees){
    vector<string> finalBlock;
    unordered_set<string> included;
    int currentWeight = 0;
    unordered_map<string,int> mapId;
    for(int i=0;i<data.size();i++){
        mapId[data[i].getID()] = i;
    }
    for(int i=0;i<data.size();i++){
        if(currentWeight > maxWeight){
            break;
        }
        if(included.find(data[i].getID())==included.end()){
            if(data[i].getParents().size()==0){
                currentWeight += data[i].getWeight();
                if(currentWeight <= maxWeight){
                    finalBlock.push_back(data[i].getID());
                    included.insert(data[i].getID());
                    totalFees += data[i].getFee();
                }
            }
            else{
                stack<string> pendingTransactions;
                pendingTransactions.push(data[i].getID());
                while(!pendingTransactions.empty()){
                    if(currentWeight > maxWeight){
                        break;
                    }
                    string curr = pendingTransactions.top();
                    int k = mapId[curr];
                    if(data[k].getParents().size()==0){
                        pendingTransactions.pop();
                        currentWeight += data[k].getWeight();
                        if(currentWeight <= maxWeight){
                            finalBlock.push_back(data[k].getID());
                            included.insert(data[k].getID());
                            totalFees += data[k].getFee();
                        }
                    }
                    else{
                        bool flag = true;
                        for(int j=0;j<data[k].getParents().size();++j){
                            if(included.find(data[k].getParents()[j])==included.end()){
                                pendingTransactions.push(data[k].getParents()[j]);
                                flag = false;
                            }
                        }
                        if(flag){
                            pendingTransactions.pop();
                            currentWeight += data[k].getWeight();
                            if(currentWeight <= maxWeight){
                                finalBlock.push_back(data[k].getID());
                                included.insert(data[k].getID());
                                totalFees += data[k].getFee();
                            }
                        }
                    }
                }
            }
        }
    }
    // cout<<currentWeight<<" "<<totalFees<<endl;
    return finalBlock;
}



int main(){
    vector<Transaction> dataSortedForFee = transactionFileParser("mempool.csv", entries);
    vector<Transaction> dataSortedForWeight = dataSortedForFee;
    sortTransactions(dataSortedForFee,dataSortedForWeight);
    int totalFeeOne = 0, totalFeeTwo = 0;
    vector<string> blockforFee = getBlockData(dataSortedForFee,totalFeeOne);
    vector<string> blockforWeight = getBlockData(dataSortedForWeight, totalFeeTwo);
    if(totalFeeOne > totalFeeTwo){
        writeTransactionsToFile(blockforFee,"block.txt");
    }
    else{
        writeTransactionsToFile(blockforWeight,"block.txt");
    }
}