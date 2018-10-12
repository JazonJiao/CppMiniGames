cmake_minimum_required(VERSION 3.9)
project(GAMES)

set(CMAKE_CXX_STANDARD 11)

add_executable(Maxim main.cpp Maxim.cpp Maxim.h
        Gomoku.cpp Gomoku.h
        ThreeMensMorris.cpp ThreeMensMorris.h
        Minesweeper.cpp Minesweeper.h
        #Genetic.cpp Genetic.h DNA.cpp DNA.h
        #STB.cpp STB.h
        )