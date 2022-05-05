cmake_minimum_required(VERSION 3.20)
project(base_code)

set(CMAKE_CXX_STANDARD 11)

include_directories(Include)

add_executable(base_code
        include/Action.h
        include/Customer.h
        include/Studio.h
        include/Trainer.h
        include/Workout.h
        src/Action.cpp
        src/Customer.cpp
        src/main.cpp
        src/Studio.cpp
        src/Trainer.cpp
        src/Workout.cpp
        )
