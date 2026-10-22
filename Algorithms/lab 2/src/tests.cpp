#include <gtest/gtest.h>
#include <random>
#include <sstream>
#include <string>
#include <vector>

#define main timsort_main
#include "main.cpp"
#undef main

// Запускает сортировку и возвращает всё, что она вывела в stdout
static string run_sort(vector<int> arr) {
    mergeCounter = 0;
    testing::internal::CaptureStdout();
    tim_sort(arr, arr.size());
    return testing::internal::GetCapturedStdout();
}

// Значения min_run для разных n
TEST(TimSort, CalcMinRun) {
    EXPECT_EQ(calc_minrun(20), 10);
    EXPECT_EQ(calc_minrun(16), 8);
    EXPECT_EQ(calc_minrun(15), 15);
    EXPECT_EQ(calc_minrun(33), 9);
    EXPECT_EQ(calc_minrun(1000), 16);
}

// Строго возрастающий по модулю run находится и разворачивается
TEST(TimSort, FindRunReversesAscending) {
    vector<int> a = {1, -2, 3, -4, 5, 6, -7, -8, 9, -10, 11, -10, -9};
    EXPECT_EQ(find_run(a, 0, a.size()), 11);
    vector<int> expected = {11, -10, 9, -8, -7, 6, 5, -4, 3, -2, 1, -10, -9};
    EXPECT_EQ(a, expected);
}

// Невозрастающий run (с равными модулями) не разворачивается
TEST(TimSort, FindRunKeepsNonIncreasing) {
    vector<int> a = {5, -5, 5, 4, 7};
    EXPECT_EQ(find_run(a, 0, a.size()), 4);
    vector<int> expected = {5, -5, 5, 4, 7};
    EXPECT_EQ(a, expected);
}

// Сортировка вставками не меняет порядок элементов с равным модулем
TEST(TimSort, InsertionSortIsStable) {
    vector<int> a = {1, -3, 2, 3, -1, -2};
    insertion_sort(a, 0, a.size());
    vector<int> expected = {-3, 3, 2, -2, 1, -1};
    EXPECT_EQ(a, expected);
}

// Галоп возвращает длину блока, который можно перенести целиком
TEST(TimSort, GallopFindsBlockLength) {
    vector<int> a = {9, 8, 7, -7, 5, 3};
    EXPECT_EQ(gallop(a, 0, a.size(), 7), 4);
    EXPECT_EQ(gallop(a, 0, a.size(), 10), 0);
    EXPECT_EQ(gallop(a, 0, a.size(), 1), 6);
}

// Примеры из условия задания: вывод должен совпадать полностью
TEST(TimSort, ExampleOneFromTask) {
    string out = run_sort({1, -2, 3, -4, 5, 6, -7, -8, 9, -10, 11, -10, -9, 8, 7, -7, -6, 6, 5, 4});
    EXPECT_EQ(out,
        "Part 0: 11 -10 9 -8 -7 6 5 -4 3 -2 1\n"
        "Part 1: -10 -9 8 7 -7 -6 6 5 4\n"
        "Gallops 0: 0\n"
        "Merge 0: 11 -10 -10 9 -9 -8 8 -7 7 -7 6 -6 6 5 5 -4 4 3 -2 1\n"
        "Answer: 11 -10 -10 9 -9 -8 8 -7 7 -7 6 -6 6 5 5 -4 4 3 -2 1\n");
}

TEST(TimSort, ExampleTwoFromTask) {
    string out = run_sort({-1, 2, 3, 4, 5, -6, 7, 8, -8, -8, 7, -7, 7, 6, -5, 4});
    EXPECT_EQ(out,
        "Part 0: 8 7 -6 5 4 3 2 -1\n"
        "Part 1: -8 -8 7 -7 7 6 -5 4\n"
        "Gallops 0: 1\n"
        "Merge 0: 8 -8 -8 7 7 -7 7 6 -6 5 -5 4 4 3 2 -1\n"
        "Answer: 8 -8 -8 7 7 -7 7 6 -6 5 -5 4 4 3 2 -1\n");
}

// Галоп засчитывается, даже если после него одна из частей уже закончилась
TEST(TimSort, GallopCountedWhenRunEnds) {
    string out = run_sort({3, -3, 2, 2, -2, 1, 1, -1, 100, -10, -5, -4, 3, 3, 2, 1});
    EXPECT_NE(out.find("Gallops 0: 3\n"), string::npos);
    EXPECT_NE(out.find("Answer: 100 -10 -5 -4 3 3 3 -3 2 2 -2 2 1 1 -1 1\n"), string::npos);
}

// Галоп на элементах с одинаковым модулем
TEST(TimSort, GallopOnEqualModules) {
    string out = run_sort({10, -10, 10, -10, -10, -10, 10, 10, 11, 10, -10, 10, -10, -10, 10, -10});
    EXPECT_NE(out.find("Gallops 0: 1\n"), string::npos);
    EXPECT_NE(out.find("Answer: 11 10 -10 10 -10 -10 -10 10 10 10 -10 10 -10 -10 10 -10\n"), string::npos);
}

// Четыре отрезка: проверка порядка слияний по инвариантам стека
TEST(TimSort, FourBlocksStackInvariant) {
    string out = run_sort({10, -10, 10, -10, -10, -10, 10, 10, 10, 11, 10, -10, 10, -10, -10, 10, -10,
                           25, -25, -15, 13, -12, -11, 11, -11, 15, -13, 12, 11, -11, 11, 10});
    EXPECT_NE(out.find("Part 3: 15 -13 12 11 -11 11 10\nGallops 0: 1\n"), string::npos);
    EXPECT_NE(out.find("Merge 0: 25 -25 -15 13 -12 -11 11 -11 11 10 -10 10 -10 -10 10 -10\n"), string::npos);
    EXPECT_NE(out.find("Gallops 2: 3\n"), string::npos);
}

// Большой пример с несколькими галопами в одном слиянии
TEST(TimSort, SevenGallopsInLastMerge) {
    string out = run_sort({-9, -8, 7, 5, 5, -4, -4, 3, -3, 2, -1, -10, 9, -7, 6, -4, -4, -3, 2, -2, 1, 0,
                           -8, -8, -8, -8, 8, -7, 5, -3, -1, -1, 0, -9, 9, -8, 6, -6, -3, 3, 3, -1, 1, -1});
    EXPECT_NE(out.find("Gallops 0: 1\n"), string::npos);
    EXPECT_NE(out.find("Gallops 1: 2\n"), string::npos);
    EXPECT_NE(out.find("Gallops 2: 7\n"), string::npos);
}

// Граничные случаи: один элемент, все модули равны, массив уже отсортирован
TEST(TimSort, SingleElement) {
    EXPECT_EQ(run_sort({-43}), "Part 0: -43\nAnswer: -43\n");
}

TEST(TimSort, AllEqualModules) {
    EXPECT_EQ(run_sort({10, -10, 10}), "Part 0: 10 -10 10\nAnswer: 10 -10 10\n");
}

TEST(TimSort, AlreadySortedOneRun) {
    EXPECT_EQ(run_sort({-6, -6, -6, 6, 5, -5, 5, 4, -4, 4}),
              "Part 0: -6 -6 -6 6 5 -5 5 4 -4 4\nAnswer: -6 -6 -6 6 5 -5 5 4 -4 4\n");
}

// Случайные массивы: результат упорядочен по модулю и является перестановкой исходного
TEST(TimSort, RandomArraysSortedByModule) {
    mt19937 rng(42);
    for (int iter = 0; iter < 200; iter++) {
        int n = 1 + rng() % 2000;
        vector<int> a(n);
        for (int i = 0; i < n; i++) a[i] = (int)(rng() % 41) - 20;
        vector<int> src = a;
        mergeCounter = 0;
        testing::internal::CaptureStdout();
        tim_sort(a, n);
        testing::internal::GetCapturedStdout();
        for (int i = 1; i < n; i++) ASSERT_GE(abs(a[i - 1]), abs(a[i]));
        vector<int> x = a, y = src;
        sort(x.begin(), x.end());
        sort(y.begin(), y.end());
        ASSERT_EQ(x, y);
    }
}
