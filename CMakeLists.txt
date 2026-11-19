cmake_minimum_required(VERSION 3.15)
project(PersonalFinanceManager)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(PersonalFinanceManager
        src/main.cpp
        src/Transaction.cpp
        src/Budget.cpp
        src/FinanceManager.cpp)

# So the program always finds the database folder, wherever it runs from
target_compile_definitions(PersonalFinanceManager PRIVATE
        DATA_DIR=\"${CMAKE_SOURCE_DIR}/database/\")