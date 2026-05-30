#include <iostream>
#include <cstdlib>
#include <cstring>
#include <vector>


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

// Parses and validates a start array string, normalizing it to exactly 60 elements
std::vector<bool> parseStartArray(const char* input) {
	const size_t ARRAY_SIZE = 60;
	
	// Check for empty input
	if (input == nullptr || std::strlen(input) == 0) {
		std::cerr << "Error: Input cannot be empty" << std::endl;
		return std::vector<bool>(); // return empty vector to signal error
	}
	
	// Check for valid characters (only 0 and 1)
	std::string inputStr(input);
	for (size_t i = 0; i < inputStr.length(); i++) {
		if (inputStr[i] != '0' && inputStr[i] != '1') {
			std::cerr << "Error: Input must contain only digits 0 and 1" << std::endl;
			std::cerr << "Invalid character at position " << i << ": '" << inputStr[i] << "'" << std::endl;
			return std::vector<bool>(); // return empty vector to signal error
		}
	}
	
	// Normalize to exactly ARRAY_SIZE elements
	std::vector<bool> result(ARRAY_SIZE, false);
	
	// Convert input string to binary, handling length differences
	for (size_t i = 0; i < inputStr.length() && i < ARRAY_SIZE; i++) {
		result[i] = (inputStr[i] == '1');
	}
	
	// Notify user of padding/truncation if needed
	if (inputStr.length() < ARRAY_SIZE) {
		std::cout << "Input padded to " << ARRAY_SIZE << " elements with zeros." << std::endl;
	} else if (inputStr.length() > ARRAY_SIZE) {
		std::cout << "Input truncated to " << ARRAY_SIZE << " elements." << std::endl;
	}
	
	return result;
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

int main(int argc, char* argv[]) {
	// Initialize the 1D "board"
	// Get start configuration from argument or interactive input
	const int n = 60;
	std::vector<bool> arr;
	std::string input_str;
	bool input_valid = false;
	
	// Try command-line argument first
	if (argc > 1) {
		arr = parseStartArray(argv[1]);
		if (arr.size() == n) {
			input_valid = true;
		} else {
			std::cout << "Invalid command-line argument. Please enter configuration interactively." << std::endl;
		}
	}
	
	// If no argument or invalid, prompt interactively
	while (!input_valid) {
		std::cout << "Enter start configuration (0s and 1s): ";
		std::getline(std::cin, input_str);
		arr = parseStartArray(input_str.c_str());
		if (arr.size() == n) {
			input_valid = true;
		}
	}
	
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
