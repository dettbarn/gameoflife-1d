# gameoflife-1d

One-dimensional Game of Life for the terminal

## Requirements

You only need the g++ compiler and these C++ libraries:

* iostream
* cstdlib
* cstring
* vector
* boost/dynamic_bitset.hpp

## Installation

From the root folder, just call:

    make

Make sure the shell script is executable:

	chmod +x gol1d.sh

## Usage

Run it with a start configuration as a command-line argument:

    ./gol1d.sh 010101001

Or run without arguments and enter the configuration when prompted:

    ./gol1d.sh
    Enter start configuration (0s and 1s): 010101001

### Configuring the Initial State

The initial state of the 1D Game of Life board must be specified as a binary string (only `0` and `1` digits).

**Valid input format:**
- Only digits `0` and `1` are allowed
- Minimum length: 1 character
- Maximum length: 60 characters (board width)

**Normalization behavior:**
- **Input shorter than 60:** Padded with zeros on the right
  - Example: `101` becomes `1010000...000` (57 additional zeros)
- **Input longer than 60:** Truncated to first 60 characters
  - Example: `11111111111111111111111111111111111111111111111111111111111111111` (65 chars) becomes the first 60 chars
- **Input exactly 60:** Used as-is without modification

**Error handling:**
If invalid input is provided, the program will show a specific error message and ask again:
- "Error: Input cannot be empty"
- "Error: Input must contain only digits 0 and 1"
- "Invalid character at position X: 'Y'"

**Examples:**

Start with a single cell:
```
./gol1d.sh 1
```

Start with alternating pattern:
```
./gol1d.sh 0101010101010101010101010101010101010101010101010101010101010101
```

Start with interactive prompt:
```
./gol1d.sh
Enter start configuration (0s and 1s): 001000100010001000100010001000100010001000100010001000100010001
Input truncated to 60 elements.
board   x x x x x x x x x x x x x x x x x x x x 
...
```
