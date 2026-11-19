#include "Transaction.h"
#include <iostream>
#include <iomanip>
using namespace std;

Transaction::Transaction(int id,
                         const string& title,
                         double amount,
                         const string& category,
                         const string& date,
                         const string& type)
        : id(id), title(title), amount(amount),
          category(category), date(date), type(type) {}

int Transaction::getId() const { return id; }
const string& Transaction::getTitle() const { return title; }
double Transaction::getAmount() const { return amount; }
const string& Transaction::getCategory() const { return category; }
const string& Transaction::getDate() const { return date; }
const string& Transaction::getType() const { return type; }

void Transaction::display() const {
    cout << left
         << setw(5)  << id
         << setw(13) << date
         << setw(10) << type
         << setw(15) << category
         << setw(20) << title
         << right << fixed << setprecision(2) << amount << " EGP\n";
}