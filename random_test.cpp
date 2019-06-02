#include <iostream>
#include <random>
#include <algorithm>
#include <vector>

using namespace std;

int main(){
    vector<int> test;

    for(int i = 0; i < 10; i++){
        test.push_back(i);
    }

    srand(time(0));
    random_shuffle(test.begin(), test.end());

    for(auto i : test){
        cout << i << " ";
    }
    cout << endl;
    return 0;
}
