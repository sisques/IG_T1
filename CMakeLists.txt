cmake_minimum_required(VERSION 3.15)
project(IG_T1)

#set(CMAKE_CXX_FLAGS_DEBUG  "-O3")
#set(CMAKE_CXX_FLAGS_RELEASE  "-O3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

include_directories(bibliotecas)

add_executable(IG_T1
        bibliotecas/camara.h
        bibliotecas/figuras.h
        bibliotecas/materialProperties.h
        bibliotecas/matrix.h
        bibliotecas/monteCarlo.h
        bibliotecas/perlinNoise.h
        bibliotecas/ply_reader.h
        bibliotecas/ppm_reader.h
        bibliotecas/punto_direccion.h
        bibliotecas/textures.h
        bibliotecas/ToneMapping.h
        IG_T3/main.cpp)
