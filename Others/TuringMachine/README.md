# Turing Machine Simulator

A deterministic, multi-tape Turing Machine simulator written in C++20, developed as an assignment for the **Theory of Computation** course.

The machine is not hard-coded. It is described in a plain-text configuration file (number of tapes, tape type, blank symbol, special states and transition function). The simulator loads that file, runs the machine on any input string, prints every computation step and reports whether the input was **accepted**, **rejected** or the run was **cancelled**.

---

## Features

- **Multi-tape:** any number of tapes, *k ≥ 1*. The input is written on tape 1 and the other tapes start blank.
- **One-way or two-way infinite tapes:** with `bidirectional: false`, moving left of the first cell cancels the run. With `bidirectional: true`, the tapes grow to the left as needed.
- **Deterministic transition function** given as a table. Duplicate or malformed entries are reported when the file is loaded.
- **Step-by-step trace:** shows the contents of every tape, the head positions and the transition applied at each step.
- **Implicit rejection:** if no transition is defined for the current state and the symbols being read, the machine halts and rejects.
- **Step limit:** a run is cancelled after 100 000 steps, so a non-halting machine can't freeze the program.

---

## Formal model

The simulator implements a deterministic *k*-tape Turing Machine

$$M = (Q,\ \Sigma,\ \Gamma,\ \delta,\ q_0,\ q_{accept},\ q_{reject})$$

with transition function

$$\delta : Q \times \Gamma^k \rightarrow Q \times \Gamma^k \times \{L, R, S\}^k$$

Each element of the tuple maps to the configuration file like this:

| Formal element | In the config file |
|---|---|
| *k* (number of tapes) | `tapes` |
| *Q* (states) | every state name used in the transitions |
| *Γ* (tape alphabet) | every symbol used in the transitions, plus the blank |
| ⊔ (blank symbol) | `blank_symbol` |
| *q₀* | `initial_state` |
| *q_accept*, *q_reject* | `accept_state`, `reject_state` |
| *δ* | the rows under `[transitions]` |

Moves are `L` (left), `R` (right) and `S` (stay). The `S` move lets a single head stay put while the other heads move.

---

## Project structure

```
TuringMachine/
├── main.cpp               # Entry point: loads ./config and shows the interactive menu
├── config                 # Machine description (example: palindromes over {0,1})
├── Makefile               # Build, run and clean targets
├── include/
│   ├── config.h           # Config struct: machine parameters + transition table
│   ├── transitions.h      # Move, TransitionKey, TransitionAction, Transitions
│   ├── turingMachine.h    # TuringMachine class: tapes, heads and execution
│   └── utils.h            # String helpers
├── src/
│   ├── config.cpp
│   ├── transitions.cpp
│   ├── turingMachine.cpp
│   └── utils.cpp
└── build/                 # Object and dependency files (generated, git-ignored)
```

### Modules

**`main.cpp`**
Loads the machine from `./config`, builds a `TuringMachine` and runs a menu loop with three options: print the configuration, run an input, or exit.

**`config` (`config.h` / `config.cpp`)**
Reads the configuration file. The header section is parsed as `key: value` pairs (`tapes`, `bidirectional`, `blank_symbol`, `initial_state`, `accept_state`, `reject_state`). When the parser reaches the `[transitions]` marker, it hands the rest of the file to `Transitions::load`. Unknown keys and lines without `:` produce warnings. A missing file, a non-numeric tape count or an invalid transition stops the program with an error. `Config::print()` shows the loaded machine.

**`transitions` (`transitions.h` / `transitions.cpp`)**
Represents the transition function *δ*.
- `Move`: the head movement, `Left`, `Right` or `Stay`.
- `TransitionKey`: the left-hand side of *δ*, the current state plus the *k* symbols under the heads.
- `TransitionAction`: the right-hand side, the next state plus the *k* symbols to write and the *k* moves.
- `Transitions`: stores the table in a `std::map<TransitionKey, TransitionAction>`. Each lookup takes *O(log n)*. The class parses and validates the rows (arrow `->`, symbol count, valid moves, no duplicates), looks up transitions and prints them.

**`turingMachine` (`turingMachine.h` / `turingMachine.cpp`)**
The simulation engine. Each tape is a `std::deque<char>`, so cells can be added at either end in constant time, which is how the tapes behave as infinite. The constructor checks that the machine has at least one tape, at least one transition, and different accept and reject states. `run(input)` does the following:
1. Writes the input on tape 1 and fills the other tapes with blanks. All heads start at position 0.
2. On every step, reads the symbol under each head, looks up the transition, writes the new symbols, moves the heads and changes the state.
3. Adds a blank cell to every tape when a head moves past either end. Moving left of position 0 is only allowed when the tapes are bidirectional.
4. Halts on the accept or reject state, when no transition is defined (reject), or when the step limit is reached (cancel).

`printStep` prints each step of the trace (see [Reading the trace](#reading-the-trace)).

**`utils` (`utils.h` / `utils.cpp`)**
`trim()`, which strips leading and trailing spaces, tabs and `\r` from a line. Because of it, config files with Windows line endings also work.

---

## Building and running

Requirements: a C++20 compiler (`g++` by default) and `make`.

```bash
make          # compile into ./tm
make run      # compile (if needed) and run
make clean    # remove build/ and the executable
```

The program always reads **`./config`** from the current directory, so run it from the project folder.

### Usage

```
===== MENU =====
1. PRINT CONFIG
2. RUN
3. EXIT
Option:
```

- **1. PRINT CONFIG** prints the loaded parameters and the full transition table.
- **2. RUN** asks for an input string and simulates the machine on it, printing the trace and the result.
- **3. EXIT** quits the program.

The input is read as a whole line, and every character becomes one tape cell. An empty input starts the machine on a blank tape.

---

## Writing a configuration file

```ini
# Lines starting with '#' are comments; blank lines are ignored.
tapes: 2
bidirectional: true
blank_symbol: _
initial_state: q0
accept_state: qA
reject_state: qR

[transitions]
# state  read(t1..tk)  ->  next  write(t1..tk)  move(t1..tk)
q0  0 _  ->  q0  0 0  R L
```

How to write each transition row:
- Separate the tokens with whitespace. Each row has the current state, *k* read symbols, `->`, the next state, *k* write symbols and *k* moves.
- Every symbol is a **single non-whitespace character**. State names can be any word without spaces.
- Each `(state, read symbols)` pair may appear only once, because the machine is deterministic.
- `[transitions]` must come after all the header keys. Everything below it is treated as transitions.

If a key is left out, it takes its default value: `tapes: 1`, `bidirectional: false`, `blank_symbol: _`, `initial_state: q0`, `accept_state: qA`, `reject_state: qR`.

### Included example: palindromes over {0, 1}

The `config` file in this folder decides the language *L = { w ∈ {0,1}\* | w = wᴿ }* using 2 bidirectional tapes:

1. **q0, copy reversed:** tape 1's head moves right through the input while tape 2's head moves left, so tape 2 ends up holding *wᴿ*.
2. **q1, rewind:** tape 1 returns to the start of the input. Tape 2 already points at the first symbol of *wᴿ*.
3. **q2, compare:** both heads move right together. A mismatch goes to `qR`. If both heads reach a blank at the same time, the machine goes to `qA`.

```
RUN 0110  ->  ACCEPTED (0110)
RUN 01    ->  REJECTED (01)
```

---

## Reading the trace

```
Step 1:
   >   _  0'[1]  <
   >  [_] 0' _   <
   { q0 (1,_)  ->  q0 (1,1) (R,L) }
---
```

- Each `> ... <` line is one tape, from tape 1 at the top to tape *k* at the bottom.
- `[x]` marks the cell under that tape's head.
- `'` marks the cell where the input started (position 0). It is useful on bidirectional tapes, where cells get added to the left.
- The `{ ... }` line is the transition about to be applied: `state (read) -> next (write) (moves)`.

At the end of the run, the simulator prints one of:

| Result | Meaning |
|---|---|
| `ACCEPTED (w)` | the machine reached `accept_state` |
| `REJECTED (w)` | the machine reached `reject_state`, or no transition was defined for the current configuration |
| `CANCELLED (w): Invalid head move to the left` | a head moved left of position 0 on a one-way tape |
| `CANCELLED (w): Step limit (100000) reached` | the machine probably doesn't halt on this input |
