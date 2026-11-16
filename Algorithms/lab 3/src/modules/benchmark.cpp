#include <algorithm>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <random>
#include <string>
#include <vector>

#define main rope_program_main
#include "main.cpp"
#undef main

using namespace std;
using Clock = chrono::steady_clock;

static volatile long long benchmarkSink = 0;

#if defined(_MSC_VER)
#define NO_INLINE __declspec(noinline)
#else
#define NO_INLINE __attribute__((noinline))
#endif

NO_INLINE static void preventElision(const void* pointer) {
#if defined(__clang__) || defined(__GNUC__)
    __asm__ __volatile__("" : : "g"(pointer) : "memory");
#else
    benchmarkSink += pointer != nullptr;
#endif
}

NO_INLINE static char ropeAt(Node* root, int index) {
    Node* current = root;
    while (!current->isLeaf) {
        if (index < current->leftSize) {
            current = current->left;
        } else {
            index -= current->leftSize;
            current = current->right;
        }
    }
    return current->text[index];
}

NO_INLINE static char stringAt(const string& value, int index) {
    return value[static_cast<size_t>(index)];
}

static double median(vector<double> values) {
    sort(values.begin(), values.end());
    return values[values.size() / 2];
}

static void materialize(Node* root) {
    string output;
    appendText(root, output);
    benchmarkSink += static_cast<long long>(output.size());
}

static void materialize(const string& text) {
    string output = text;
    benchmarkSink += static_cast<long long>(output.size());
}

static void printRow(const char* operation, int n, int iterations,
                     double ropeNs, double stringNs) {
    printf("%s,%d,%d,%.1f,%.1f,%.4f\n", operation, n, iterations,
           ropeNs, stringNs, stringNs == 0 ? 0.0 : ropeNs / stringNs);
}

static void benchmarkBuild(const string& source) {
    const int n = static_cast<int>(source.size());
    const int iterations = max(3, 100000 / n);
    vector<double> ropeTimes;
    vector<double> stringTimes;

    for (int rep = 0; rep < 8; ++rep) {
        auto start = Clock::now();
        for (int i = 0; i < iterations; ++i) {
            Node* root = buildFromString(source);
            preventElision(root);
            benchmarkSink += Node::nodeSize(root);
            deleteTree(root);
        }
        auto ropeEnd = Clock::now();

        for (int i = 0; i < iterations; ++i) {
            string copy = source;
            preventElision(copy.data());
            benchmarkSink += static_cast<long long>(copy[copy.size() / 2]);
        }
        auto stringEnd = Clock::now();

        if (rep > 0) {
            ropeTimes.push_back(
                chrono::duration<double, nano>(ropeEnd - start).count() / iterations);
            stringTimes.push_back(
                chrono::duration<double, nano>(stringEnd - ropeEnd).count() / iterations);
        }
    }

    printRow("build_and_destroy", n, iterations,
             median(ropeTimes), median(stringTimes));
}

static void benchmarkIndex(const string& source) {
    const int n = static_cast<int>(source.size());
    const int iterations = 50000;
    Node* root = buildFromString(source);
    mt19937 rng(2026 + static_cast<unsigned>(n));
    vector<int> indices(iterations);
    for (int& index : indices) {
        index = static_cast<int>(rng() % static_cast<unsigned>(n));
    }

    vector<double> ropeTimes;
    vector<double> stringTimes;
    for (int rep = 0; rep < 8; ++rep) {
        long long checksum = 0;
        auto start = Clock::now();
        for (int index : indices) checksum += ropeAt(root, index);
        auto ropeEnd = Clock::now();
        for (int index : indices) checksum += stringAt(source, index);
        auto stringEnd = Clock::now();
        benchmarkSink += checksum;

        if (rep > 0) {
            ropeTimes.push_back(
                chrono::duration<double, nano>(ropeEnd - start).count() / iterations);
            stringTimes.push_back(
                chrono::duration<double, nano>(stringEnd - ropeEnd).count() / iterations);
        }
    }

    printRow("random_index", n, iterations,
             median(ropeTimes), median(stringTimes));
    deleteTree(root);
}

static void benchmarkEdit(const string& source, bool includeMaterialization) {
    const int n = static_cast<int>(source.size());
    const int iterations = 100;
    const int index = n / 2;
    const string inserted = "ABCDEFGH";
    vector<double> ropeTimes;
    vector<double> stringTimes;

    for (int rep = 0; rep < 8; ++rep) {
        Node* root = buildFromString(source);
        auto start = Clock::now();
        for (int i = 0; i < iterations; ++i) {
            SplitResult prefixAndSuffix = split(root, index);
            root = join(join(prefixAndSuffix.prefix, buildFromString(inserted)),
                        prefixAndSuffix.suffix);
            if (includeMaterialization) materialize(root);

            SplitResult first = split(root, index);
            SplitResult removedAndRight =
                split(first.suffix, static_cast<int>(inserted.size()));
            deleteTree(removedAndRight.prefix);
            root = join(first.prefix, removedAndRight.suffix);
            if (includeMaterialization) materialize(root);
        }
        auto ropeEnd = Clock::now();
        string ropeResult;
        appendText(root, ropeResult);
        if (ropeResult != source) {
            fprintf(stderr, "Rope edit benchmark changed the input string\n");
            exit(2);
        }
        deleteTree(root);

        string value = source;
        auto stringStart = Clock::now();
        for (int i = 0; i < iterations; ++i) {
            value.insert(static_cast<size_t>(index), inserted);
            if (includeMaterialization) materialize(value);
            value.erase(static_cast<size_t>(index), inserted.size());
            if (includeMaterialization) materialize(value);
        }
        auto stringEnd = Clock::now();
        if (value != source) {
            fprintf(stderr, "std::string edit benchmark changed the input string\n");
            exit(2);
        }

        if (rep > 0) {
            ropeTimes.push_back(
                chrono::duration<double, nano>(ropeEnd - start).count() / iterations);
            stringTimes.push_back(
                chrono::duration<double, nano>(stringEnd - stringStart).count() / iterations);
        }
    }

    printRow(includeMaterialization ? "edit_pair_with_output" : "edit_pair",
             n, iterations, median(ropeTimes), median(stringTimes));
}

int main() {
    printf("operation,n,iterations,rope_ns_per_op,string_ns_per_op,rope_over_string\n");
    for (int n : {256, 1024, 4096, 16384, 65536}) {
        string source(static_cast<size_t>(n), 'a');
        for (int i = 0; i < n; ++i) {
            source[static_cast<size_t>(i)] = static_cast<char>('a' + (i % 26));
        }

        benchmarkBuild(source);
        benchmarkIndex(source);
        benchmarkEdit(source, false);
        benchmarkEdit(source, true);
    }
    fprintf(stderr, "benchmark checksum: %lld\n", benchmarkSink);
    return 0;
}
