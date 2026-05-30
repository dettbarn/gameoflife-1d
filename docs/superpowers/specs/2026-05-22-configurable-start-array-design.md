# Design: Configurable Start Array Input

**Date:** 2026-05-22  
**Scope:** Add CLI interface to configure the initial state array instead of hardcoding it

## Overview

The Game of Life 1D program will be enhanced with a **flexible input system** that allows users to configure the starting array at runtime via:
- Command-line argument: `./gol1d.sh 010101001`
- Interactive prompt: Program asks if no argument is provided
- Fallback behavior: If input fails, user is re-prompted with specific error feedback

The array size remains fixed at 60 elements.

## Requirements

### Functional Requirements

1. **Dual Input Modes**
   - Accept start array as command-line argument (1st positional argument)
   - If no argument provided, prompt user interactively
   - If interactive input fails, show error and re-prompt (loop until valid)

2. **Input Validation**
   - Only digits `0` and `1` allowed
   - Minimum length: 1 character
   - Maximum length: 60 characters
   - Empty input: reject with error message

3. **Array Normalization**
   - **Too short:** Pad with zeros at the end to reach 60 elements
   - **Too long:** Take first 60 characters
   - **Both cases:** Inform user before simulation starts
   - **Exact length (60):** Process silently without notification

4. **Error Messages**
   - Specific, actionable feedback for each validation failure
   - Guide user toward correct input format
   - Clear indication of what was wrong and why

5. **Documentation**
   - Update README with valid input format
   - Document padding/truncation behavior
   - Provide usage examples

### Non-Functional Requirements

- No changes to simulation logic or output format
- Backward-compatible with existing shell wrapper (`gol1d.sh`)
- Input validation is performant (no complex parsing needed)
- Error handling is user-friendly and clear

## Architecture

### New Component: `parseStartArray(const char* input)`

**Location:** `src/gol1d.cxx`

**Signature:**
```cpp
std::vector<bool> parseStartArray(const char* input);
```

**Behavior:**
- Input: C-string from command line or stdin
- Output: `std::vector<bool>` with exactly 60 elements
- Throws or returns error indication on validation failure
- All normalization (padding/truncation) handled here

**Validation Logic:**
1. Check for empty input → error
2. Check for non-digit characters → error with char position
3. Check length:
   - If < 60: pad with zeros (return flag/message)
   - If > 60: truncate to first 60 (return flag/message)
   - If == 60: proceed silently

**Error Messages:**
- "Error: Input must contain only digits 0 and 1"
- "Error: Invalid character at position X: 'Y'"
- "Error: Input cannot be empty"

### Modified `main()` Function

**Flow:**
1. Check for command-line argument (argc > 1)
2. If present: validate with `parseStartArray(argv[1])`
3. If not present or validation failed:
   - Print prompt: "Enter start configuration (0s and 1s): "
   - Read from stdin
   - Validate with `parseStartArray()`
   - Loop on validation failure, re-prompt with error message
4. After successful validation:
   - If input was normalized: print info message (e.g., "Input padded to 60 elements")
   - Proceed with simulation using the normalized array

**User Experience Example:**
```
$ ./gol1d.sh
Enter start configuration (0s and 1s): 
abc
Error: Input must contain only digits 0 and 1
Enter start configuration (0s and 1s): 
101
Input padded to 60 elements with zeros.
board   x x                                                  
...
```

## Data Flow

```
User Input (CLI arg or stdin)
    ↓
parseStartArray()
    ├─ Validation
    │  ├─ Error? → Return error state/message
    │  └─ Valid? → Normalize
    ├─ Normalization
    │  ├─ Too short? → Pad with zeros, flag as modified
    │  ├─ Too long? → Truncate to 60, flag as modified
    │  └─ Exact 60? → No change, no flag
    └─ Return vector<bool> (60 elements) + modification flag
    ↓
main() receives normalized array
    ├─ If flagged as modified: inform user
    └─ Proceed with simulation
```

## Error Handling Strategy

- **Parse errors:** Specific message + re-prompt loop (interactive mode)
- **Command-line argument errors:** Show message + fall back to interactive prompt
- **No recovery mechanism:** User always has chance to correct input (unlike silent fallback)
- **Graceful degradation:** Invalid argument → switch to interactive mode

## Testing Considerations

The validation function should be testable with:
- Valid inputs: `"1"`, `"101010"`, `"0" * 60`
- Invalid characters: `"10a01"`, `"1 0 1"`, `"10.1"`
- Length edge cases: `""` (empty), `"1"` (1 char), `"0" * 60` (exact), `"1" * 61` (1 over)
- Normalization verification: output is always exactly 60 elements

## README Updates

Add section: "Usage - Configuring Initial State"

```markdown
### Configuring the Initial State

By default, the program requires you to specify the starting configuration:

**Via command-line argument:**
```
./gol1d.sh 010101001
```

**Via interactive prompt (if no argument provided):**
```
$ ./gol1d.sh
Enter start configuration (0s and 1s): 010101001
```

**Valid input format:**
- Only digits `0` and `1`
- Minimum 1 character, maximum 60 characters
- If input is shorter than 60, it will be padded with zeros on the right
- If input is longer than 60, only the first 60 characters will be used

**Examples:**
- `1` → Padded to: `1` followed by 59 zeros
- `010101010101010101010101010101010101010101010101010101010101` → Exactly 60 chars (no change)
- `0101010101010101010101010101010101010101010101010101010101010101` → Truncated to first 60 chars
```

## Timeline / Scope

This feature is self-contained and does not affect existing simulation logic. Implementation should be straightforward with clear separation of concerns.

## Sign-Off

- [x] Dual input modes (CLI + interactive)
- [x] Validation with specific error messages
- [x] Normalization (padding/truncation) with user notification
- [x] Fixed array size (60 elements)
- [x] README documentation
- [x] Error handling strategy defined
