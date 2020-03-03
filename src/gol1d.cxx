#include <iostream>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <boost/dynamic_bitset.hpp>


// checks if a value is contained in a vector
bool isin(int val, std::vector<int> confvals)
{
	for(unsigned int i = 0; i < confvals.size(); i++) {
		if(confvals[i] == val)
			return true;
	}
	return false;
}

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

char symbol(bool b) {
	return b ? 'x' : ' ';
}

template <typename T>
int println(std::vector<T> v, std::string vname) {
	std::cout << vname << "   ";
	for(unsigned int i = 0; i < v.size(); i++) {
		std::cout << (int)v[i];
	}
	std::cout << std::endl;
	return 0;
}

int printlnsymbol(std::vector<bool> v, std::string vname) {
	std::cout << vname << "   ";
	for(unsigned int i = 0; i < v.size(); i++) {
	  std::cout << symbol(v[i]);
	}
	std::cout << std::endl;
	return 0;
}

void wait() {
	double d = 1;
	for(unsigned long i = 0; i < 1000000; i++) {
		d*=(i+1)/(i+2);
	}
}

int main() {
	const int n = 60;
	bool array[n] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0};
	std::vector<bool> arr(&array[0], &array[0] + n);
	// maybe free array at some point
	
	std::vector<int> cs,cb,cse,cbe;
//	cs.push_back(0);
	cs.push_back(1);
//	cs.push_back(2);
//	cb.push_back(0);
	cb.push_back(1);
	//	cb.push_back(2);
	cse=cs;
	cbe=cb;

	println(cs, "survival");
	println(cb, "birth");
	println(cse, "survival edge");
	println(cbe, "birth edge");

	const unsigned int niter = 10000000;
	for(unsigned int i = 0; i < niter; i++) {
		printlnsymbol(arr, "board");
		iterate(arr, cs, cb, cse, cbe);
		wait();
	}

	return 0;
}
