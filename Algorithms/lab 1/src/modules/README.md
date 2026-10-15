## Структура проекта

    src/main.cpp                   — точка входа: интерактивная работа с деком
    src/tests.cpp                  — юнит-тесты (GoogleTest)
    src/modules/circular_deque.h   — класс CircularDeque
    src/modules/benchmark.cpp      — бенчмарк, печатает CSV в stdout
    src/modules/plot_benchmark.py  — построение графиков по результатам бенчмарка

## Сборка

Для сборки проекта требуется cmake.
Чтобы собрать проект в папке build, нужно выполнить следующие команды:

    cmake -S . -B build
    cmake --build build

## Запуск
Для запуска программы нужно перейти в папку build и выполнить:

    ./main

Программа запрашивает вместимость дека N и режим push_force (0/1),
затем принимает команды (push_front X, push_back X, pop_front, pop_back,
front, back, size, empty, full, resize N, help, exit).

Для запуска unit-тестов нужно перейти в папку build и выполнить:

    ./run_tests

Для запуска бенчмарка нужно перейти в папку build и выполнить:

    ./benchmark
    ./benchmark > ../benchmark.csv

Для построения графиков (benchmark.csv должен лежать в корне проекта,
графики сохраняются в папку plots в корне проекта):

    python3 src/modules/plot_benchmark.py
