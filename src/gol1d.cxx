#include <iostream>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <boost/dynamic_bitset.hpp>


// Checks if a value is contained in a vector
bool isin(int val, std::vector<int> confvals)
{
	for(unsigned int i = 0; i < confvals.size(); i++) {
		if(confvals[i] == val)
			return true;
	}
	return false;
}

// Iterates the Game of Life, using the given rules for birth and survival
int iterate(std::vector<bool>& arr, std::vector<int> conf_survival, std::vector<int> conf_birth, std::vector<int> conf_survival_edge, std::vector<int> conf_birth_edge) {
	// arr[1] is identical to the neighborhood-sum of arr[0] for non-cyclical
        std::vector<bool> prevarr = arr;
  
	arr[0] = prevarr[0] ? isin((int)prevarr[1],conf_survival_edge) : isin((int)arr[1],conf_birth_edge);
	for(unsigned int i = 1; i < prevarr.size() - 1; i++) {
		arr[i] = prevarr[i] ? isin((int)prevarr[i-1] + (int)prevarr[i+1], conf_survival) : isin((int)prevarr[i-1] + (int)prevarr[i+1], conf_birth);
	}
	int n = prevarr.size();

	arr[n-1] = prevarr[n-1] ? isin((int)prevarr[n-2], conf_survival_edge) : isin((int)prevarr[n-2], conf_birth_edge);

	return 0;
}

// Returns a certain symbol to mark if a spot is populated or not
char symbol(bool b) {
	return b ? 'x' : ' ';
}

// Prints the given 1D vector as integers, prepended by the string vname
template <typename T>
int println(std::vector<T> v, std::string vname) {
	std::cout << vname << "   ";
	for(unsigned int i = 0; i < v.size(); i++) {
		std::cout << (int)v[i];
	}
	std::cout << std::endl;
	return 0;
}

// Prints the given 1D bool vector as symbols, prepended by the string vname
int printlnsymbol(std::vector<bool> v, std::string vname) {
	std::cout << vname << "   ";
	for(unsigned int i = 0; i < v.size(); i++) {
	  std::cout << symbol(v[i]);
	}
	std::cout << std::endl;
	return 0;
}

// Waits for some time
void wait() {
	double d = 1;
	for(unsigned long i = 0; i < 1000000; i++) {
		// These operations are arbitrary; they need to be sufficiently complex so that the compiler does not optimize them away
		d*=(i+1)/(i+2);
	}
}

int main() {
	// Initialize the 1D "board"
	const int n = 60;
	bool array[n] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0};
	std::vector<bool> arr(&array[0], &array[0] + n);
	// maybe free array at some point (with proper GUI)
	
	// Define the rules for survival and birth
	std::vector<int> cs,cb,cse,cbe;
//	cs.push_back(0);
	cs.push_back(1);
//	cs.push_back(2);
//	cb.push_back(0);
	cb.push_back(1);
	//	cb.push_back(2);
	cse=cs;
	cbe=cb;

	// Print the rules for survival and birth
	println(cs, "survival");
	println(cb, "birth");
	println(cse, "survival edge");
	println(cbe, "birth edge");

	// Run the Game of Life for a long time
	const unsigned int niter = 10000000;
	for(unsigned int i = 0; i < niter; i++) {
		printlnsymbol(arr, "board");
		iterate(arr, cs, cb, cse, cbe);
		wait();
	}

	return 0;
}
