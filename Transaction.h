#pragma once
#include <string>

class Transaction {
private:
    int id;
    std::string title;
    double amount;
    std::string category;
    std::string date;      // YYYY-MM-DD
    std::string type;      // "Income" or "Expense"

public:
    Transaction(int id,
                const std::string& title,
                double amount,
                const std::string& category,
                const std::string& date,
                const std::string& type);

    int getId() const;
    const std::string& getTitle() const;
    double getAmount() const;
    const std::string& getCategory() const;
    const std::string& getDate() const;
    const std::string& getType() const;

    void display() const;
};