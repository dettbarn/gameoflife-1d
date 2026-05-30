# Configurable Start Array Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Add a CLI interface that accepts user input to configure the initial 1D Game of Life board state instead of hardcoding it.

**Architecture:** Introduce a validation function `parseStartArray()` that handles input validation, normalization (padding/truncation), and error reporting. Modify `main()` to accept command-line arguments and fall back to interactive stdin, then update README with usage documentation.

**Tech Stack:** C++ standard library (iostream, cstring, vector), no external dependencies beyond existing Boost usage

---

## File Structure

### Modified Files
- `src/gol1d.cxx` — Add `parseStartArray()` function, modify `main()` for CLI input
- `README.md` — Add section documenting start configuration usage

### No new files needed — all changes are contained within existing structure

---

## Task 1: Implement `parseStartArray()` Function

**Files:**
- Modify: `src/gol1d.cxx:1-70` (add new function before main)

**Responsibility:** Validate and normalize user input to a consistent 60-element binary array with clear error messaging.

- [ ] **Step 1: Add function skeleton and documentation**

Insert this after the `symbol()` function (around line 37) and before `println()`:

```cpp
// Parses and validates start configuration string
// Returns a vector<bool> of exactly 60 elements
// Outputs validation messages to cout
// Handles padding (short input) and truncation (long input)
std::vector<bool> parseStartArray(const char* input) {
	const int ARRAY_SIZE = 60;
	
	// TODO: validation and normalization logic
	
	return std::vector<bool>(ARRAY_SIZE, false); // placeholder
}
```

- [ ] **Step 2: Implement empty input check**

Replace the TODO comment with:

```cpp
	// Check for empty input
	if (input == nullptr || std::strlen(input) == 0) {
		std::cerr << "Error: Input cannot be empty" << std::endl;
		return std::vector<bool>(); // return empty vector to signal error
	}
	
	// Additional validation to follow...
```

- [ ] **Step 3: Implement digit-only validation**

Add after the empty check:

```cpp
	// Check for valid characters (only 0 and 1)
	std::string inputStr(input);
	for (unsigned int i = 0; i < inputStr.length(); i++) {
		if (inputStr[i] != '0' && inputStr[i] != '1') {
			std::cerr << "Error: Input must contain only digits 0 and 1" << std::endl;
			std::cerr << "Invalid character at position " << i << ": '" << inputStr[i] << "'" << std::endl;
			return std::vector<bool>(); // return empty vector to signal error
		}
	}
	
	// Additional normalization to follow...
```

- [ ] **Step 4: Implement array normalization (padding and truncation)**

Add after validation:

```cpp
	// Normalize to exactly ARRAY_SIZE elements
	std::vector<bool> result(ARRAY_SIZE, false);
	
	if (inputStr.length() < (unsigned int)ARRAY_SIZE) {
		// Pad with zeros at the end
		for (unsigned int i = 0; i < inputStr.length(); i++) {
			result[i] = (inputStr[i] == '1');
		}
		if (inputStr.length() < (unsigned int)ARRAY_SIZE) {
			std::cout << "Input padded to " << ARRAY_SIZE << " elements with zeros." << std::endl;
		}
	} else if (inputStr.length() > (unsigned int)ARRAY_SIZE) {
		// Take first ARRAY_SIZE characters
		for (int i = 0; i < ARRAY_SIZE; i++) {
			result[i] = (inputStr[i] == '1');
		}
		std::cout << "Input truncated to " << ARRAY_SIZE << " elements." << std::endl;
	} else {
		// Exact length — convert without notification
		for (int i = 0; i < ARRAY_SIZE; i++) {
			result[i] = (inputStr[i] == '1');
		}
	}
	
	return result;
```

- [ ] **Step 5: Compile and test basic function**

Run: `make`

Expected: Compilation succeeds (function exists but isn't called yet)

- [ ] **Step 6: Commit**

```bash
cd /home/michi/code/gameoflife-1d
git add src/gol1d.cxx
git commit -m "feat: add parseStartArray validation function"
```

---

## Task 2: Modify `main()` to Accept Command-Line Arguments

**Files:**
- Modify: `src/gol1d.cxx:69-102` (main function)

**Responsibility:** Handle dual input modes (CLI argument with fallback to interactive prompt), validate, and initialize the array before simulation.

- [ ] **Step 1: Replace hardcoded array with input parsing logic**

Replace lines 70-73 (the hardcoded array initialization) with:

```cpp
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
```

- [ ] **Step 2: Verify main function signature accepts argc/argv**

The function signature should be:

```cpp
int main(int argc, char* argv[]) {
```

Change it if it currently only has `int main()`.

- [ ] **Step 3: Remove old std::vector initialization line**

The old line:
```cpp
std::vector<bool> arr(&array[0], &array[0] + n);
```

should be removed (it's now replaced in Step 1).

- [ ] **Step 4: Compile and test**

Run: `make`

Expected: Compilation succeeds

- [ ] **Step 5: Test with command-line argument**

Run: `./bin/gol1d 010101010101010101010101010101010101010101010101010101010101`

Expected: Program starts simulation without prompting (exact 60 chars)

- [ ] **Step 6: Test with interactive input**

Run: `./bin/gol1d` and type `101` when prompted

Expected: Program shows "Input padded to 60 elements with zeros." and starts simulation

- [ ] **Step 7: Test error handling**

Run: `./bin/gol1d` and type `10a01` when prompted

Expected: Shows "Error: Input must contain only digits 0 and 1" and re-prompts

- [ ] **Step 8: Commit**

```bash
git add src/gol1d.cxx
git commit -m "feat: add CLI argument parsing and interactive input with validation"
```

---

## Task 3: Update README with Usage Documentation

**Files:**
- Modify: `README.md:25-29` (Usage section)

**Responsibility:** Document how users can configure the initial state, provide examples, explain padding/truncation behavior.

- [ ] **Step 1: Expand Usage section**

Replace the current "Usage" section (lines 25-29) with:

```markdown
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
```

- [ ] **Step 2: Verify README is well-formatted**

Run: `cat README.md` and visually check formatting

Expected: No syntax errors, clear structure with examples

- [ ] **Step 3: Commit**

```bash
git add README.md
git commit -m "docs: add usage instructions for configurable start array"
```

---

## Task 4: Final Integration Testing

**Files:**
- Test: `src/gol1d.cxx` (manual testing of the compiled binary)

**Responsibility:** Verify all functionality works correctly before deployment.

- [ ] **Step 1: Clean rebuild**

Run: `make clean && make`

Expected: Compilation succeeds, binary is in `./bin/gol1d`

- [ ] **Step 2: Test exact 60-character input**

Run: `./bin/gol1d 010101010101010101010101010101010101010101010101010101010101`

Expected: 
- No padding/truncation message
- Simulation runs immediately
- First line shows: `board   x x x x x x x x x x x x x x x x x x x x x x x x x x x x x `

- [ ] **Step 3: Test short input with padding notification**

Run: `./bin/gol1d 111`

Expected:
- Output includes: "Input padded to 60 elements with zeros."
- Simulation shows `board   xxx` on first line (followed by many spaces)

- [ ] **Step 4: Test long input with truncation notification**

Run: `./bin/gol1d 11111111111111111111111111111111111111111111111111111111111111111111111111111111`

Expected:
- Output includes: "Input truncated to 60 elements."
- Simulation shows only first 60 chars of the input

- [ ] **Step 5: Test invalid characters (error + re-prompt)**

Run: `./bin/gol1d` and type `10x01`

Expected:
- Output: "Error: Input must contain only digits 0 and 1"
- Output: "Invalid character at position 2: 'x'"
- Re-prompts: "Enter start configuration (0s and 1s): "
- Enter valid input like `10101` — simulation runs

- [ ] **Step 6: Test empty input (error + re-prompt)**

Run: `./bin/gol1d` and press Enter without typing

Expected:
- Output: "Error: Input cannot be empty"
- Re-prompts: "Enter start configuration (0s and 1s): "
- Enter valid input — simulation runs

- [ ] **Step 7: Test via shell wrapper**

Run: `./gol1d.sh 101010`

Expected: Simulation starts with padded array (no need for interactive prompt)

- [ ] **Step 8: Commit final state**

```bash
git add -A
git commit -m "test: verify all CLI input scenarios work correctly"
```

---

## Self-Review

**Spec Coverage:**
- ✅ Dual input modes (CLI arg + interactive) — Task 2
- ✅ Input validation with specific error messages — Task 1, Step 2-3
- ✅ Padding/truncation with user notification — Task 1, Step 4
- ✅ Fixed array size (60 elements) — Task 1-2
- ✅ README documentation — Task 3
- ✅ Error handling and re-prompt loop — Task 2

**Placeholder Scan:** No placeholders, all code is complete and specific.

**Type Consistency:** 
- `parseStartArray()` returns `std::vector<bool>` consistently
- Array size constant `ARRAY_SIZE = 60` matches spec
- Function signature and calling sites match

**Scope Check:** All tasks are focused on the feature, no unrelated refactoring.

---

## Execution Options

Plan is complete and saved. How would you like to proceed?

**Option 1: Subagent-Driven (recommended)** — I dispatch fresh subagents per task, review progress, fast iteration

**Option 2: Inline Execution** — I execute all tasks in this session with checkpoints for review

Which approach works better for you?

