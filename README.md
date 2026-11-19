# 💰 Personal Finance Manager

Track income and expenses, set monthly budgets per category, and see where your money goes.
Available as a **C++ console app** and as a **web version**.

🔗 **Live demo:** https://7alahossam144-rgb.github.io/PersonalFinanceManager/

## Features
- Add income and expenses (title, amount, category, date)
- Automatic category detection from the transaction title
- View and delete transactions
- Financial summary: total income, total expenses, balance
- Monthly budget per category, with warnings at 80% and when exceeded
- Monthly report showing top spending categories
- Data saved between runs
- Input validation for amounts, dates, and menu choices

## Built with
- **C++17**, CMake, object-oriented design (`Transaction`, `Budget`, `FinanceManager`), file I/O
- **HTML, CSS, JavaScript** for the web version (data stored in the browser with localStorage)

## Project structure
```
include/   header files (.h)
src/       C++ source files and main
database/  saved data files
docs/      web version (index.html)
```

## How to run

### Web version
Open the live demo link above, or open `docs/index.html` in your browser.

### C++ console version
```bash
git clone https://github.com/7alahossam144-rgb/PersonalFinanceManager.git
cd PersonalFinanceManager
mkdir build && cd build
cmake ..
cmake --build .
./PersonalFinanceManager
```
On Windows the executable is `PersonalFinanceManager.exe`.

## What I learned
- Splitting a program into classes with header and source files
- Reading and writing files in C++
- Validating user input and handling errors
- Publishing a project with Git, GitHub, and GitHub Pages

## Future improvements
- Edit existing transactions
- Export transactions to CSV
- Search and filter transactions