#include <algorithm>
#include <chrono>
#include <cstdio>
#include <vector>

#include "circular_deque.h"

using clk = std::chrono::steady_clock;

static double ns_per_op(long long total_ns, long long ops) {
    return static_cast<double>(total_ns) / static_cast<double>(ops);
}

static long long median(std::vector<long long>& v) {
    std::sort(v.begin(), v.end());
    return v[v.size() / 2];
}

int main() {
    std::vector<size_t> sizes;
    for (size_t n = 1000; n <= 10000000; n = static_cast<size_t>(n * 1.5)) {
        sizes.push_back(n);
    }

    const int WARMUP = 1;
    const int REPEAT = 9;

    printf("operation,N,ns_per_op\n");

    for (size_t N : sizes) {
        std::vector<long long> samples;

        samples.clear();
        for (int r = 0; r < WARMUP + REPEAT; ++r) {
            CircularDeque dq(N, false);
            auto t0 = clk::now();
            for (size_t i = 0; i < N; ++i) dq.push_back(static_cast<int>(i));
            auto t1 = clk::now();
            long long dt =
                std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count();
            if (r >= WARMUP) samples.push_back(dt);
        }
        printf("push_back,%zu,%.4f\n", N, ns_per_op(median(samples), N));

        samples.clear();
        for (int r = 0; r < WARMUP + REPEAT; ++r) {
            CircularDeque dq(N, false);
            auto t0 = clk::now();
            for (size_t i = 0; i < N; ++i) dq.push_front(static_cast<int>(i));
            auto t1 = clk::now();
            long long dt =
                std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count();
            if (r >= WARMUP) samples.push_back(dt);
        }
        printf("push_front,%zu,%.4f\n", N, ns_per_op(median(samples), N));

        samples.clear();
        for (int r = 0; r < WARMUP + REPEAT; ++r) {
            CircularDeque dq(N, false);
            for (size_t i = 0; i < N; ++i) dq.push_back(static_cast<int>(i));
            auto t0 = clk::now();
            for (size_t i = 0; i < N; ++i) dq.pop_back();
            auto t1 = clk::now();
            long long dt =
                std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count();
            if (r >= WARMUP) samples.push_back(dt);
        }
        printf("pop_back,%zu,%.4f\n", N, ns_per_op(median(samples), N));

        samples.clear();
        for (int r = 0; r < WARMUP + REPEAT; ++r) {
            CircularDeque dq(N, false);
            for (size_t i = 0; i < N; ++i) dq.push_back(static_cast<int>(i));
            auto t0 = clk::now();
            for (size_t i = 0; i < N; ++i) dq.pop_front();
            auto t1 = clk::now();
            long long dt =
                std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count();
            if (r >= WARMUP) samples.push_back(dt);
        }
        printf("pop_front,%zu,%.4f\n", N, ns_per_op(median(samples), N));

        samples.clear();
        {
            volatile int sink = 0;
            for (int r = 0; r < WARMUP + REPEAT; ++r) {
                CircularDeque dq(N, false);
                for (size_t i = 0; i < N; ++i) dq.push_back(static_cast<int>(i));
                auto t0 = clk::now();
                for (size_t i = 0; i < N; ++i) sink = dq.front();
                auto t1 = clk::now();
                long long dt =
                    std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count();
                if (r >= WARMUP) samples.push_back(dt);
            }
            (void)sink;
        }
        printf("front,%zu,%.4f\n", N, ns_per_op(median(samples), N));

        samples.clear();
        {
            volatile int sink = 0;
            for (int r = 0; r < WARMUP + REPEAT; ++r) {
                CircularDeque dq(N, false);
                for (size_t i = 0; i < N; ++i) dq.push_back(static_cast<int>(i));
                auto t0 = clk::now();
                for (size_t i = 0; i < N; ++i) sink = dq.back();
                auto t1 = clk::now();
                long long dt =
                    std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count();
                if (r >= WARMUP) samples.push_back(dt);
            }
            (void)sink;
        }
        printf("back,%zu,%.4f\n", N, ns_per_op(median(samples), N));

        samples.clear();
        for (int r = 0; r < WARMUP + REPEAT; ++r) {
            CircularDeque dq(N, false);
            for (size_t i = 0; i < N; ++i) dq.push_back(static_cast<int>(i));
            auto t0 = clk::now();
            dq.resize(N * 2);
            auto t1 = clk::now();
            long long dt =
                std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count();
            if (r >= WARMUP) samples.push_back(dt);
        }

        printf("resize,%zu,%.4f\n", N, ns_per_op(median(samples), 1));
    }

    return 0;
}