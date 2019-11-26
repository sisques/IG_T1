cmake_minimum_required(VERSION 3.15)
project(bibliotecas)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(bibliotecas
        esfera.h
        main.cpp
        plano.h
        punto_direccion.h
        tranformations.h
        triangulo.h)
