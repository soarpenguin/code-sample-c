#include <vector>
#include <iostream>
#include <bitset>
using namespace std;

vector<int> twoSum(vector<int> &numbers, int target) {
    // Start typing your C/C++ solution below
    // DO NOT write int main() function
    vector<int> result;

    if (numbers.empty()) {
        return result;
    }
    
    bitset <100> tmp;
    int index = 0;

    for(vector<int>::iterator it = numbers.begin(); it != numbers.end(); it++) {
        if (target >= *it) {
            tmp.set(*it, 1);
        }
    }

    for(vector<int>::iterator it = numbers.begin();
            it != numbers.end(); it++) {
        if (target - *it >= 0) {
            if (tmp.test(target - *it)) {
                result.push_back(index);
                break;
            }
            tmp.set(target - *it, 1);
        }
        index++;
    }

    if (!result.empty()) {
        index = result.back();
        result.pop_back();

        int i = 0;
        for(vector<int>::iterator it = numbers.begin(); 
                it != numbers.end(); it++) {
            if (numbers[index] + *it == target) {
                i++; index++;

                if (i < index) {
                    result.push_back(i);
                    result.push_back(index);
                } else {
                    result.push_back(index);
                    result.push_back(i); 
                }
                break;
            }
            i++;
        }
    }

    return result;
}


int
main(int argc, char **argv) {
	int myints[] = {2, 7, 11, 15};
  	std::vector<int> fifth (myints, myints + sizeof(myints) / sizeof(int) );
	vector<int> result;
	
	result = twoSum(fifth, 9);

	if (! result.empty()) {
		for(vector<int>::iterator it = result.begin(); it != result.end(); it++) {
			cout << *it << "\n";
        }
    }
    return 0;
}

