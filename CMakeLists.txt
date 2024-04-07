cmake_minimum_required(VERSION 3.27)
project(lab_19_16 C)

set(CMAKE_C_STANDARD 11)

add_executable(lab_19_16 main.c
        matrix.c
        libs/data_structures/matrix/matrix.h
)

dd_executable(lab_19_16 main.c
        libs/data_structures/matrix/matrix.h)
