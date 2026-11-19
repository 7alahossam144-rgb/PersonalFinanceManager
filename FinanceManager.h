#pragma once
#include <string>
#include <vector>
#include "Transaction.h"
#include "Budget.h"

class FinanceManager {
private:
    std::vector<Transaction> transactions;
    Budget budget;
    int nextId = 1;

    double spentInCategory(const std::string& category, const std::string& month) const;
    std::string currentMonth() const;

public:
    // Auto-categorization
    static std::string suggestCategory(const std::string& title);

    // Transactions
    void addTransaction(const std::string& title,
                        double amount,
                        const std::string& category,
                        const std::string& date,
                        const std::string& type);
    void viewTransactions() const;
    void deleteTransaction(int id);
    void loadTransactions();
    void saveTransactions() const;

    // Summary
    double getTotalIncome() const;
    double getTotalExpenses() const;
    double getBalance() const;
    void monthlyReport(const std::string& month = "") const;   // "" = current month

    // Budget
    void setBudgetMenu();
    void showBudgetStatus(const std::string& month) const;
    void checkBudgetWarning(const std::string& category, const std::string& month) const;
    void loadBudget();
    void saveBudget() const;
};