cmake_minimum_required(VERSION 3.9)
project(GAMES-orig)

set(CMAKE_CXX_STANDARD 11)

add_executable(Maxim main.cpp Maxim.cpp Maxim.h
        Gomoku.cpp Gomoku.h
        ThreeMensMorris.cpp ThreeMensMorris.h
        Minesweeper.cpp Minesweeper.h
        #Genetic.cpp Genetic.h DNA.cpp DNA.h
        STB.cpp STB.h
        Sudoku.cpp Sudoku.h G2048.cpp G2048.h Chomp.cpp Chomp.h)