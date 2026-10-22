#include <algorithm>
#include <chrono>
#include <cstdio>
#include <random>
#include <string>
#include <vector>

#include "timsort.h"

using namespace std;

static const int WARMUP = 1;
static const int REPEAT = 9;
static mt19937 rng(2026);

static vector<int> gen_best(int n) {
    vector<int> a(n);
    for (int i = 0; i < n; i++) a[i] = (rng() & 1 ? 1 : -1) * (n - i);
    return a;
}

static vector<int> gen_average(int n) {
    vector<int> a(n);
    for (int i = 0; i < n; i++) a[i] = (int)(rng() % 2000000001u) - 1000000000;
    return a;
}

static unsigned bit_reverse(unsigned x, int bits) {
    unsigned r = 0;
    for (int i = 0; i < bits; i++) {
        r = (r << 1) | (x & 1);
        x >>= 1;
    }
    return r;
}

static vector<int> gen_worst(int n) {
    int min_run = calc_minrun(n);
    int m = (n + min_run - 1) / min_run;
    int bits = 0;
    while ((1 << bits) < m) bits++;
    vector<int> order(m);
    for (int q = 0; q < m; q++) order[q] = q;
    sort(order.begin(), order.end(), [&](int x, int y) { return bit_reverse(x, bits) < bit_reverse(y, bits); });
    vector<vector<int>> blocks(m);
    int rank = 1;
    for (int round = 0; rank <= n; round++) {
        for (int q : order) {
            int size = min(min_run, n - q * min_run);
            if (round < size) blocks[q].push_back(rank++);
        }
    }
    vector<int> a;
    for (auto& b : blocks) {
        for (size_t i = 0; i + 1 < b.size(); i += 2) swap(b[i], b[i + 1]);
        a.insert(a.end(), b.begin(), b.end());
    }
    return a;
}

static vector<int> gen_partial(int n) {
    vector<int> a = gen_best(n);
    int swaps = max(1, n / 100);
    for (int s = 0; s < swaps; s++) swap(a[rng() % n], a[rng() % n]);
    return a;
}

static vector<int> generate(const string& kind, int n) {
    if (kind == "best") return gen_best(n);
    if (kind == "average") return gen_average(n);
    if (kind == "worst") return gen_worst(n);
    return gen_partial(n);
}

struct Result {
    double time_ns;
    long long comparisons;
    long long gallops;
    int merges;
};

static Result measure(const vector<int>& src) {
    int n = src.size();
    int batch = max(1, 200000 / n);
    vector<double> times;
    Result res{0, 0, 0, 0};
    for (int rep = 0; rep < WARMUP + REPEAT; rep++) {
        vector<vector<int>> copies(batch, src);
        comparisons = 0;
        gallopsTotal = 0;
        auto t0 = chrono::steady_clock::now();
        for (int b = 0; b < batch; b++) tim_sort(copies[b], n);
        auto t1 = chrono::steady_clock::now();
        if (rep >= WARMUP) times.push_back(chrono::duration<double, nano>(t1 - t0).count() / batch);
        res.comparisons = comparisons / batch;
        res.gallops = gallopsTotal / batch;
        res.merges = mergeCounter;
    }
    sort(times.begin(), times.end());
    res.time_ns = times[times.size() / 2];
    return res;
}

int main() {
    PRINT = false;
    MINRUN_LIMIT = 64;
    GALLOP = 7;

    printf("experiment,case,n,min_run,gallop,time_ns,comparisons,gallops,merges\n");

    vector<int> sizes = {10, 20, 50, 100, 200, 500, 1000, 2000, 5000, 10000,
                         20000, 50000, 100000, 200000, 500000, 1000000};
    vector<string> kinds = {"best", "average", "worst", "partial"};
    for (const string& kind : kinds) {
        for (int n : sizes) {
            FIXED_MINRUN = 0;
            vector<int> src = generate(kind, n);
            Result r = measure(src);
            printf("size,%s,%d,%d,%d,%.1f,%lld,%lld,%d\n", kind.c_str(), n, calc_minrun(n), GALLOP,
                   r.time_ns, r.comparisons, r.gallops, r.merges);
            fflush(stdout);
        }
    }

    vector<int> min_runs = {1, 2, 4, 8, 16, 32, 48, 64, 128, 256, 512, 1024, 2048};
    for (int n : {1000, 100000}) {
        vector<int> src = gen_average(n);
        for (int mr : min_runs) {
            if (mr > n) continue;
            FIXED_MINRUN = mr;
            Result r = measure(src);
            printf("minrun,average,%d,%d,%d,%.1f,%lld,%lld,%d\n", n, mr, GALLOP,
                   r.time_ns, r.comparisons, r.gallops, r.merges);
            fflush(stdout);
        }
    }
    FIXED_MINRUN = 0;

    for (const string& kind : {string("average"), string("partial")}) {
        vector<int> src = generate(kind, 100000);
        for (int g : {7, 1000000000}) {
            GALLOP = g;
            Result r = measure(src);
            printf("gallop,%s,%d,%d,%d,%.1f,%lld,%lld,%d\n", kind.c_str(), 100000, calc_minrun(100000), g,
                   r.time_ns, r.comparisons, r.gallops, r.merges);
            fflush(stdout);
        }
    }
    GALLOP = 7;
    return 0;
}
