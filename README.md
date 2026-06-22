# 🚀 Sorting Algorithm's Visualiser in C
A lightweight, terminal-driven sorting algorithm visualiser written in C using the Raylib graphics library.<br>It accepts the array size and algorithm choice as command-line arguments and renders a real-time animated bar chart showing comparisons and swaps frame-by-frame.

---

## ✨ Features

- Visualises three classic sorting algorithms in real-time animated bar form.
- Color-coded bar states to distinguish inactive bars, comparisons, swaps, and fully sorted elements.
- Dynamic array size — pass any count between 2 and 500 at launch.
- Adjustable animation speed at runtime using arrow keys.
- Switch algorithms mid-session without restarting the program.
- Pluggable algorithm table adding a new algorithm requires only a step function, a reset function, and a single line in the algos[] table.

---

## 🛠 Tech Stack
* **Language:** C and its Libraries
* **Tools:** GCC, Neovim, git

## 📚 Dependencies
| Library | Purpose |
|---|---|
| `raylib` | Window creation and bar rendering |
| `stdbool` | To implement Boolean Variables and Literals |
| `stdio.h` | Standard I/O for argument parsing and error output |
| `stdlib.h` | Memory allocation (malloc, free) and atoi |
| `time.h` | Seeding the Fisher-Yates shuffle with time(NULL) |

---

## 📁 Project Structure

```
sort-visualiser/
├── main.c        # Source code
```

---

## 🚀 Getting Started
To get a copy of this project up and running on their local machine:

### Prerequisites
Make sure you have the following installed:

- **GCC** (or any C compiler)
- **Raylib** development libraries

### Installation

- Clone the repository:
```bash
  git clone https://github.com/vishnuparihar2008/sort-visualiser.git
```

- Navigate into the project directory:
```bash
  cd ./sort-visualiser
```

- Compile the project:
```bash
gcc -Wall -Wextra -g main.c -o bin/main -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
```

This compiles `main.c` and links the Raylib library automatically.

- Run the project:
```bash
./bin/main 100          # To visualise bubble-sort on an array of 100 elements

./bin/main 99 2        # To visualise selection-sort on an array of 100 elements

./bin/main 99 3        # To visualise insertion-sort on an array of 100 elements
```

This runs the visualiser<br>
A window will open showing the shuffled bars animating toward sorted order. Close it by clicking the X button or pressing Alt+F4.

---

### 📄 Command-Line Arguments
Launch syntax:
```text
./bin/main <count> [algo]
count : integer between 2 and 500
algo  : 1 = Bubble Sort   (default)
        2 = Selection Sort
        3 = Insertion Sort
```

Runtime keyboard controls:
| Key | Action |
| --- | ------ |
| 1/2/3 | Switch Algorithms |
| R | Reshuffle and Restart the Current Algorithm |
| Arrow Up/Down | Increase/Decrease Steps per Frame |

---

## ⚙️ How It Works
1) Argument Parsing: Reads count and an optional algo index from argv. Validates the range and allocates the numbers array on the heap.
2) Shuffle: Fills numbers[1..count] and applies a Fisher-Yates shuffle seeded with the current time.
3) Algorithm Dispatch: Each algorithm exposes a step() and reset() function packed into an Algorithm struct.<br>An algos[] table holds all three entries, keyed to the number row.
4) Render Loop: Each frame, one or more sort steps are executed depending on the current speed setting.<br>Bars are drawn as filled rectangles color-coded by their state:<br>White (inactive) → Orange (comparing) → Red (swapping) → Lime (sorted).
5) Event Handling: Raylib key events switch algorithms, reshuffle the array, or adjust how many sort steps run per frame.

---

## 💡 Usage Examples
Bubble Sort<br>
![ Bubble Sort ](https://github.com/vishnuparihar2008/sort-visualise/blob/main/util/bs.gif)<br><br>
Selection Sort<br>
![ Selection Sort ](https://github.com/vishnuparihar2008/sort-visualise/blob/main/util/ss.gif)<br><br>
Insertion Sort<br>
![ Insertion Sort ](https://github.com/vishnuparihar2008/sort-visualise/blob/main/util/is.gif)<br><br>

---

## 🗺 Roadmap / Future Enhancements
- [ ] Scale it up to visualise more Sorting Algorithm.
- [ ] Display a live comparison and swap counter on screen.
- [ ] Implement audio by producing a tone based on bar height on each swap.
- [ ] Add a full-scrreen option.

---

## 👩‍💻 Author

**Vishnu Parihar** — [vishnuparihar2008](https://github.com/vishnuparihar2008)

---

## 📄 License

This project is open source. Feel free to use, modify, and distribute.
