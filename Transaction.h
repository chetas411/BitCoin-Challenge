#include <iostream>
#include <string>
#include <vector>
using namespace std;
class Transaction{
    private:
        string tx_id;
        int fee;
        int weight;
        vector<string> parentTransactions;

    public:
        //constructor
        Transaction(string tx_id, int fee, int weight, vector<string> parentTransactions){
            this->tx_id = tx_id;
            this->fee = fee;
            this->weight = weight;
            this->parentTransactions = parentTransactions; 
        }

        //getters
        string getID(){
            return tx_id;
        }

        int getFee(){
            return fee;
        }

        int getWeight(){
            return weight;
        }

        vector<string> getParents(){
            return parentTransactions;
        }

        void printDetails(){
            cout<<tx_id<<" "<<fee<<" "<<weight<<" ";
            for(auto s : parentTransactions){
                cout<<s<<" ";
            }
            cout<<endl;
        }
};