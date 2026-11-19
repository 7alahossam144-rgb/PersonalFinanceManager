#pragma once
#include <string>
#include <map>

class Budget {
private:
    std::map<std::string, double> limits;   // category (lowercase) -> monthly limit

public:
    static std::string normalize(const std::string& text);   // lowercase

    void setLimit(const std::string& category, double limit);
    bool hasLimit(const std::string& category) const;
    double getLimit(const std::string& category) const;
    bool removeLimit(const std::string& category);
    const std::map<std::string, double>& getAll() const;

    void saveToFile(const std::string& path) const;
    void loadFromFile(const std::string& path);
};