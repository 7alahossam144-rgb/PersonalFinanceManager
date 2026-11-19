#include "FinanceManager.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <limits>
#include <ctime>
#include <map>
#include <vector>
using namespace std;

#ifndef DATA_DIR
#define DATA_DIR "database/"
#endif

// '|' is our separator in the data file, so we replace it if the user types it
static string clean(string text) {
    replace(text.begin(), text.end(), '|', '-');
    return text;
}

// ================= Auto-categorization =================

string FinanceManager::suggestCategory(const string& title) {
    static const vector<pair<string, vector<string>>> rules = {
            {"food",          {"pizza", "burger", "coffee", "lunch", "dinner", "breakfast",
                                                                                 "restaurant", "grocery", "supermarket"}},
            {"transport",     {"uber", "careem", "metro", "bus", "taxi", "fuel", "petrol", "microbus"}},
            {"bills",         {"electricity", "water", "internet", "phone", "wifi", "rent"}},
            {"entertainment", {"netflix", "spotify", "cinema", "game", "movie"}},
            {"education",     {"book", "course", "tuition", "print", "notes"}},
            {"health",        {"pharmacy", "doctor", "medicine", "clinic"}},
    };

    string t = Budget::normalize(title);
    for (const auto& [category, words] : rules)
        for (const auto& word : words)
            if (t.find(word) != string::npos)
                return category;

    return "";    // no match
}

// ================= Transactions =================

void FinanceManager::addTransaction(const string& title,
                                    double amount,
                                    const string& category,
                                    const string& date,
                                    const string& type) {
    transactions.emplace_back(nextId++, clean(title), amount,
                              clean(category), clean(date), type);
    saveTransactions();
    cout << "\n" << type << " added successfully.\n";
}

void FinanceManager::viewTransactions() const {
    if (transactions.empty()) {
        cout << "\nNo transactions yet.\n";
        return;
    }

    cout << "\n" << left
         << setw(5)  << "ID"
         << setw(13) << "Date"
         << setw(10) << "Type"
         << setw(15) << "Category"
         << setw(20) << "Title"
         << "Amount\n";
    cout << string(70, '-') << '\n';

    for (const auto& t : transactions)
        t.display();
}

void FinanceManager::deleteTransaction(int id) {
    auto it = remove_if(transactions.begin(), transactions.end(),
                        [id](const Transaction& t) { return t.getId() == id; });

    if (it == transactions.end()) {
        cout << "No transaction with that ID.\n";
        return;
    }
    transactions.erase(it, transactions.end());
    saveTransactions();
    cout << "Transaction deleted.\n";
}

void FinanceManager::saveTransactions() const {
    ofstream out(string(DATA_DIR) + "transactions.txt");
    if (!out) {
        cout << "Warning: could not save transactions.\n";
        return;
    }
    out << fixed << setprecision(2);
    for (const auto& t : transactions) {
        out << t.getId() << '|' << t.getDate() << '|' << t.getType() << '|'
            << t.getCategory() << '|' << t.getAmount() << '|' << t.getTitle() << '\n';
    }
}

void FinanceManager::loadTransactions() {
    transactions.clear();
    nextId = 1;

    ifstream in(string(DATA_DIR) + "transactions.txt");
    if (!in) return;                       // first run: no file yet, that's fine

    string line;
    while (getline(in, line)) {
        stringstream ss(line);
        string idStr, date, type, category, amountStr, title;

        if (!getline(ss, idStr, '|') || !getline(ss, date, '|') ||
            !getline(ss, type, '|')  || !getline(ss, category, '|') ||
            !getline(ss, amountStr, '|') || !getline(ss, title))
            continue;                      // skip a broken line

        try {
            int id = stoi(idStr);
            double amount = stod(amountStr);
            transactions.emplace_back(id, title, amount, category, date, type);
            nextId = max(nextId, id + 1);  // keep IDs unique
        } catch (...) {
            // skip a line with bad numbers
        }
    }
}

// ================= Summary =================

double FinanceManager::getTotalIncome() const {
    double total = 0;
    for (const auto& t : transactions)
        if (t.getType() == "Income") total += t.getAmount();
    return total;
}

double FinanceManager::getTotalExpenses() const {
    double total = 0;
    for (const auto& t : transactions)
        if (t.getType() == "Expense") total += t.getAmount();
    return total;
}

double FinanceManager::getBalance() const {
    return getTotalIncome() - getTotalExpenses();
}

string FinanceManager::currentMonth() const {
    time_t now = time(nullptr);
    tm* t = localtime(&now);
    char buf[8];
    strftime(buf, sizeof(buf), "%Y-%m", t);      // e.g. 2026-11
    return string(buf);
}

void FinanceManager::monthlyReport(const string& monthInput) const {
    string month = monthInput.empty() ? currentMonth() : monthInput;

    double income = 0, expenses = 0;
    map<string, double> byCategory;

    for (const auto& t : transactions) {
        if (t.getDate().substr(0, 7) != month) continue;

        if (t.getType() == "Income") {
            income += t.getAmount();
        } else if (t.getType() == "Expense") {
            expenses += t.getAmount();
            byCategory[Budget::normalize(t.getCategory())] += t.getAmount();
        }
    }

    cout << "\n========== MONTHLY REPORT (" << month << ") ==========\n";

    if (income == 0 && expenses == 0) {
        cout << "No transactions in this month.\n";
        return;
    }

    cout << fixed << setprecision(2);
    cout << "Income:   " << income   << " EGP\n";
    cout << "Expenses: " << expenses << " EGP\n";
    cout << "Saved:    " << income - expenses << " EGP\n";

    if (expenses > 0) {
        vector<pair<string, double>> sorted(byCategory.begin(), byCategory.end());
        sort(sorted.begin(), sorted.end(),
             [](const auto& a, const auto& b) { return a.second > b.second; });

        cout << "\nTop spending categories:\n";
        int rank = 1;
        for (const auto& [category, amount] : sorted) {
            double percent = amount / expenses * 100;
            cout << rank++ << ". " << left << setw(15) << category << right
                 << setw(10) << amount << " EGP  (" << percent << "%) "
                 << string(static_cast<size_t>(percent / 5), '#') << '\n';
        }
        cout << "\nYou spent the most on: " << sorted.front().first << "\n";
    }
}

// ================= Budget =================

double FinanceManager::spentInCategory(const string& category, const string& month) const {
    double spent = 0;
    for (const auto& t : transactions) {
        if (t.getType() == "Expense" &&
            Budget::normalize(t.getCategory()) == Budget::normalize(category) &&
            t.getDate().substr(0, 7) == month)
            spent += t.getAmount();
    }
    return spent;
}

void FinanceManager::showBudgetStatus(const string& month) const {
    if (budget.getAll().empty()) {
        cout << "\nNo budgets set yet.\n";
        return;
    }

    cout << "\n=== Budget Status (" << month << ") ===\n";
    for (const auto& [category, limit] : budget.getAll()) {
        double spent = spentInCategory(category, month);
        double percent = (limit > 0) ? spent / limit * 100 : 0;

        cout << fixed << setprecision(2)
             << category << ": " << spent << " / " << limit
             << " EGP (" << percent << "%)";
        if (percent >= 100)     cout << "  EXCEEDED!";
        else if (percent >= 80) cout << "  Almost there";
        cout << '\n';
    }
}

void FinanceManager::checkBudgetWarning(const string& category, const string& month) const {
    if (!budget.hasLimit(category)) return;

    double spent = spentInCategory(category, month);
    double limit = budget.getLimit(category);

    if (spent > limit)
        cout << "Warning: you exceeded your " << category << " budget!\n";
    else if (spent >= 0.8 * limit)
        cout << "Heads up: you used 80% of your " << category << " budget.\n";
}

void FinanceManager::loadBudget() {
    budget.loadFromFile(string(DATA_DIR) + "budget.csv");
}

void FinanceManager::saveBudget() const {
    budget.saveToFile(string(DATA_DIR) + "budget.csv");
}

void FinanceManager::setBudgetMenu() {
    int choice = 0;
    do {
        cout << "\n=== BUDGET MENU ===\n"
             << "1. Set category budget\n"
             << "2. View budget status\n"
             << "3. Remove a budget\n"
             << "4. Back\n"
             << "Choose an option: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input!\n";
            choice = 0;
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice == 1) {
            string category;
            double limit;

            cout << "Category: ";
            getline(cin, category);
            if (category.empty()) {
                cout << "Category cannot be empty!\n";
                continue;
            }

            cout << "Monthly limit: ";
            if (!(cin >> limit) || limit <= 0) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid amount!\n";
                continue;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            budget.setLimit(category, limit);
            cout << "Budget saved.\n";
        }
        else if (choice == 2) {
            showBudgetStatus(currentMonth());
        }
        else if (choice == 3) {
            string category;
            cout << "Category to remove: ";
            getline(cin, category);
            if (budget.removeLimit(category)) cout << "Budget removed.\n";
            else cout << "No budget found for that category.\n";
        }
    } while (choice != 4);
}