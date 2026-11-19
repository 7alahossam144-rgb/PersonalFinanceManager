#include <iostream>
#include <limits>
#include <string>
#include <cctype>
#include "FinanceManager.h"

using namespace std;

void displayMenu() {
    cout << "\n====================================\n";
    cout << "       PERSONAL FINANCE MANAGER     \n";
    cout << "====================================\n";
    cout << "1. Add Income\n";
    cout << "2. Add Expense\n";
    cout << "3. View Transactions\n";
    cout << "4. Financial Summary\n";
    cout << "5. Delete Transaction\n";
    cout << "6. Budget\n";
    cout << "7. Monthly Report\n";
    cout << "8. Exit\n";
    cout << "====================================\n";
    cout << "Choose an option: ";
}

bool isValidDate(const string& d) {
    if (d.size() != 10 || d[4] != '-' || d[7] != '-') return false;
    for (int i : {0, 1, 2, 3, 5, 6, 8, 9})
        if (!isdigit(static_cast<unsigned char>(d[i]))) return false;

    int month = stoi(d.substr(5, 2));
    int day   = stoi(d.substr(8, 2));
    return month >= 1 && month <= 12 && day >= 1 && day <= 31;
}

void addTransactionMenu(FinanceManager& manager, const string& type) {
    string title, category, date;
    double amount;

    cout << "\nEnter transaction title: ";
    getline(cin, title);

    cout << "Enter amount: ";
    if (!(cin >> amount) || amount <= 0) {
        cout << "Invalid amount!\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter category (press Enter to auto-detect): ";
    getline(cin, category);

    if (category.empty()) {
        category = FinanceManager::suggestCategory(title);
        if (category.empty()) category = "other";
        cout << "Category set to: " << category << "\n";
    }

    cout << "Enter date (YYYY-MM-DD): ";
    getline(cin, date);

    if (title.empty()) {
        cout << "Title cannot be empty!\n";
        return;
    }
    if (!isValidDate(date)) {
        cout << "Invalid date! Use YYYY-MM-DD (e.g. 2026-11-06).\n";
        return;
    }

    manager.addTransaction(title, amount, category, date, type);

    if (type == "Expense")
        manager.checkBudgetWarning(category, date.substr(0, 7));
}

int main() {
    FinanceManager manager;
    manager.loadTransactions();
    manager.loadBudget();

    int choice = 0;

    do {
        displayMenu();

        if (!(cin >> choice)) {
            cout << "Invalid input! Enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            choice = 0;
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1:
                addTransactionMenu(manager, "Income");
                break;

            case 2:
                addTransactionMenu(manager, "Expense");
                break;

            case 3:
                manager.viewTransactions();
                break;

            case 4:
                cout << "\n========== FINANCIAL SUMMARY ==========\n";
                cout << "Total Income: "     << manager.getTotalIncome()   << " EGP\n";
                cout << "Total Expenses: "   << manager.getTotalExpenses() << " EGP\n";
                cout << "Current Balance: "  << manager.getBalance()       << " EGP\n";
                break;

            case 5: {
                int id;
                cout << "Enter transaction ID to delete: ";
                if (!(cin >> id)) {
                    cout << "Invalid ID!\n";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    break;
                }
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                manager.deleteTransaction(id);
                break;
            }

            case 6:
                manager.setBudgetMenu();
                manager.saveBudget();
                break;

            case 7: {
                string month;
                cout << "Month (YYYY-MM) or press Enter for current month: ";
                getline(cin, month);
                manager.monthlyReport(month);
                break;
            }

            case 8:
                manager.saveBudget();
                cout << "\nThank you for using Personal Finance Manager!\n";
                break;

            default:
                cout << "Invalid choice! Try again.\n";
        }

    } while (choice != 8);

    return 0;
}