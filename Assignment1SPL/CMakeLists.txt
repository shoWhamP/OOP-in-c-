cmake_minimum_required(VERSION 3.20)
project(assignment1)

set(CMAKE_CXX_STANDARD 14)

add_executable(assignment1 ./src/main.cpp
        ./src/Studio.cpp
        ./src/Trainer.cpp
        ./src/Customer.cpp
        ./src/Action.cpp
        ./src/Workout.cpp)
