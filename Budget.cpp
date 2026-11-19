#include "Budget.h"
#include <algorithm>
#include <cctype>
#include <fstream>
using namespace std;

// "Food" and "food" are treated as the same category
string Budget::normalize(const string& text) {
    string result = text;
    transform(result.begin(), result.end(), result.begin(),
              [](unsigned char c) { return static_cast<char>(tolower(c)); });
    return result;
}

void Budget::setLimit(const string& category, double limit) {
    limits[normalize(category)] = limit;
}

bool Budget::hasLimit(const string& category) const {
    return limits.count(normalize(category)) > 0;
}

double Budget::getLimit(const string& category) const {
    auto it = limits.find(normalize(category));
    return it != limits.end() ? it->second : 0.0;
}

bool Budget::removeLimit(const string& category) {
    return limits.erase(normalize(category)) > 0;
}

const map<string, double>& Budget::getAll() const {
    return limits;
}

void Budget::saveToFile(const string& path) const {
    ofstream out(path);
    if (!out) return;
    for (const auto& [cat, lim] : limits)
        out << cat << ',' << lim << '\n';
}

void Budget::loadFromFile(const string& path) {
    ifstream in(path);
    limits.clear();
    if (!in) return;                       // file doesn't exist yet: that's fine

    string line;
    while (getline(in, line)) {
        size_t pos = line.rfind(',');
        if (pos == string::npos) continue;
        try {
            limits[line.substr(0, pos)] = stod(line.substr(pos + 1));
        } catch (...) {
            // skip a broken line
        }
    }
}