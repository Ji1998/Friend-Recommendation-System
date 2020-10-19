//
// This code is to test file read and write, and recomendation
//10/17/2020
//ifstream:  to read from files
//ofstream:  to write to files
#include<fstream>
#include<iostream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <set>
#include<queue>

using namespace ::std;

int recommendation(int idQuery, string countryQuery,unordered_map<string, unordered_map<int, unordered_set<int>>>countryDict)
{
    unordered_map<int, unordered_set<int>> countryMap;                               //map of countryQuery(2nd layer)
    unordered_set<int> idQueryFriends;                                              //friends of idQuery
    unordered_map<int, int> counter;                                                //count # of repeats of 1st level connection
    if(!countryDict.count(countryQuery)) return -2;                                 //didn't find country, error code = -2
    countryMap = countryDict[countryQuery];
    if(!countryMap.count(idQuery)) return -1;                                       //didn't find user, error code = -1;
    idQueryFriends = countryMap[idQuery];
    for(auto a : idQueryFriends)
    {
        for(auto b : countryMap[a])
        {
            ++counter[b];
        }
    }
    auto comp  = [](pair<int, int> &a, pair<int, int>&b)
    {
        if(a.second == b.second) return a.first>b.first;
        else return a.second < b.second;
    };
    priority_queue<pair<int,int>, vector<pair<int, int>>, decltype(comp)> friendsComparator(comp);

    for(auto a : counter)
    {
        if(a.first == idQuery) continue;
        friendsComparator.push({a.first, a.second});
    }
    auto it = friendsComparator.top();
    if(friendsComparator.empty()) return -1;
    return it.first;
}

int main () {
    unordered_map<string, unordered_map<int, unordered_set<int>>> countryDict;      //country->(input if -> friends)
    unordered_set<string> countryInBackendA;                                        // return the countries contained in backendserver
    string countryName;                                                             //will be updated when found a new country
    int newlineUserId;                                                           //will be updated when found a newline of user
    string line;
    ifstream myfile("example.txt");
    if (myfile.is_open()) {
        while (getline(myfile, line)) {
            istringstream is(line);
            string stringWithoutSpace;
            string firstString;
            is >> firstString;
            if (isalpha(firstString[0])) {
                countryInBackendA.insert(firstString);
                countryName = firstString;
            }
            if (isdigit(firstString[0])) {
                newlineUserId = stoi(firstString);
                string followString = "";
                countryDict[countryName][newlineUserId].insert(
                        1234);                   /*insert a value then clear the value to keep the value with no friend*/
                countryDict[countryName][newlineUserId].clear();                           /*                                                                   */
                while (is >> followString) {
                    countryDict[countryName][newlineUserId].insert(stoi(followString));
                }
            }
        }
        myfile.close();
    }
    else cout << "Unable to open file";
    cout << recommendation(9999, "jpYsAHXfNwO", countryDict) << endl;
    return 0;
}

/** this part is to test the content in the map **/
//    for(auto a : countryDict)
//    {
//        cout << a.first << endl;
//        unordered_map<int, unordered_set<int>>temp = a.second;
//        for(auto b : temp)
//        {
//            cout << b.first <<" ";
//            for(auto c : b.second)
//            {
//                cout << c << " ";
//            }
//            cout  << endl;
//        }
//    }
//    for(auto a : countryInBackendA) cout << a << endl;




