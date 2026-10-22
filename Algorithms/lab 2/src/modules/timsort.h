#pragma once
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

inline int MINRUN_LIMIT = 16;
inline int GALLOP = 3;
inline int FIXED_MINRUN = 0;
inline bool PRINT = true;
inline long long comparisons = 0;
inline long long gallopsTotal = 0;
inline int mergeCounter = 0;

struct Run {
    int len;
    int start;
};

inline void printRange(const vector<int>& a, int lo, int hi) {
    for (int i = lo; i < hi; i++) {
        if (i > lo) cout << " ";
        cout << a[i];
    }
    cout << "\n";
}

inline bool before(int a, int b) {
    comparisons++;
    return abs(a) > abs(b);
}

inline int calc_minrun(int n) {
    if (FIXED_MINRUN > 0) return FIXED_MINRUN;
    int r = 0;
    while (n >= MINRUN_LIMIT) {
        r |= n & 1;
        n >>= 1;
    }
    return n + r;
}

inline void reverseRange(vector<int>& arr, int lo, int hi) {
    hi--;
    while (lo < hi) {
        int t = arr[lo];
        arr[lo] = arr[hi];
        arr[hi] = t;
        hi--;
        lo++;
    }
}

inline int find_run(vector<int>& arr, int lo, int n) {
    int hi = lo + 1;
    if (hi == n) return hi;
    if (before(arr[hi], arr[lo])) {
        while (hi < n && before(arr[hi], arr[hi - 1])) hi++;
        reverseRange(arr, lo, hi);
    } else {
        while (hi < n && !before(arr[hi], arr[hi - 1])) hi++;
    }
    return hi;
}

inline void insertion_sort(vector<int>& arr, int lo, int hi) {
    for (int i = lo; i < hi; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= lo && before(key, arr[j])) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

inline bool goesFirst(int x, int key) {
    return !before(key, x);
}

inline int gallop(const vector<int>& arr, int from, int end, int key) {
    int len = end - from;
    if (len <= 0 || !goesFirst(arr[from], key)) return 0;
    int last_ok = 0;
    int bound = 1;
    while (bound < len && goesFirst(arr[from + bound], key)) {
        last_ok = bound;
        bound = bound * 2 + 1;
    }
    if (bound > len) bound = len;
    int lo = last_ok + 1;
    int hi = bound;
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (goesFirst(arr[from + mid], key)) lo = mid + 1;
        else hi = mid;
    }
    return lo;
}

inline int merge_runs(vector<int>& arr, int lo, int mid, int hi) {
    vector<int> left(arr.begin() + lo, arr.begin() + mid);
    vector<int> right(arr.begin() + mid, arr.begin() + hi);
    int n_left = left.size();
    int n_right = right.size();
    int i = 0, j = 0, k = lo;
    int count_left = 0, count_right = 0, gallops = 0;
    while (i < n_left && j < n_right) {
        if (!before(right[j], left[i])) {
            arr[k++] = left[i++];
            count_left++;
            count_right = 0;
        } else {
            arr[k++] = right[j++];
            count_right++;
            count_left = 0;
        }
        if (count_left == GALLOP) {
            gallops++;
            if (i < n_left && j < n_right) {
                int cnt = gallop(left, i, n_left, right[j]);
                for (int t = 0; t < cnt; t++) arr[k++] = left[i++];
            }
            count_left = 0;
            count_right = 0;
        } else if (count_right == GALLOP) {
            gallops++;
            if (i < n_left && j < n_right) {
                int cnt = gallop(right, j, n_right, left[i]);
                for (int t = 0; t < cnt; t++) arr[k++] = right[j++];
            }
            count_left = 0;
            count_right = 0;
        }
    }
    while (i < n_left) arr[k++] = left[i++];
    while (j < n_right) arr[k++] = right[j++];
    return gallops;
}

inline void mergeAt(vector<int>& arr, vector<Run>& stack, int k) {
    int lo = stack[k].start;
    int mid = lo + stack[k].len;
    int hi = mid + stack[k + 1].len;
    int gallops = merge_runs(arr, lo, mid, hi);
    gallopsTotal += gallops;
    if (PRINT) {
        cout << "Gallops " << mergeCounter << ": " << gallops << "\n";
        cout << "Merge " << mergeCounter << ": ";
        printRange(arr, lo, hi);
    }
    mergeCounter++;
    stack[k].len += stack[k + 1].len;
    stack.erase(stack.begin() + k + 1);
}

inline void merge_collapse(vector<int>& arr, vector<Run>& stack) {
    while (stack.size() > 1) {
        int k = stack.size() - 2;
        if ((k > 0 && stack[k - 1].len <= stack[k].len + stack[k + 1].len) ||
            (k > 1 && stack[k - 2].len <= stack[k - 1].len + stack[k].len)) {
            if (stack[k - 1].len < stack[k + 1].len) k--;
            mergeAt(arr, stack, k);
        } else if (stack[k].len <= stack[k + 1].len) {
            mergeAt(arr, stack, k);
        } else {
            break;
        }
    }
}

inline void mergeForceCollapse(vector<int>& arr, vector<Run>& stack) {
    while (stack.size() > 1) {
        int k = stack.size() - 2;
        if (k > 0 && stack[k - 1].len < stack[k + 1].len) k--;
        mergeAt(arr, stack, k);
    }
}

inline void tim_sort(vector<int>& arr, int n) {
    mergeCounter = 0;
    int min_run = calc_minrun(n);
    int lo = 0;
    int part = 0;
    vector<Run> runs;
    while (lo < n) {
        int hi = find_run(arr, lo, n);
        if (hi - lo < min_run) hi = min(lo + min_run, n);
        insertion_sort(arr, lo, hi);
        if (PRINT) {
            cout << "Part " << part << ": ";
            printRange(arr, lo, hi);
        }
        part++;
        runs.push_back({hi - lo, lo});
        lo = hi;
    }
    vector<Run> stack;
    for (int r = 0; r < (int)runs.size(); r++) {
        stack.push_back(runs[r]);
        merge_collapse(arr, stack);
    }
    mergeForceCollapse(arr, stack);
    if (PRINT) {
        cout << "Answer: ";
        printRange(arr, 0, n);
    }
}
