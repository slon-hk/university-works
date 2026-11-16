#include <gtest/gtest.h>

#include <algorithm>
#include <random>
#include <sstream>
#include <string>

#define main rope_program_main
#include "main.cpp"
#undef main

using namespace std;

static string flatten(Node* root) {
    string result;
    appendText(root, result);
    return result;
}

static int validateTree(Node* node, string& text) {
    if (node == nullptr) {
        return 0;
    }

    if (node->isLeaf) {
        EXPECT_EQ(node->left, nullptr);
        EXPECT_EQ(node->right, nullptr);
        EXPECT_EQ(node->size, static_cast<int>(node->text.size()));
        EXPECT_GT(node->size, 0);
        EXPECT_LE(node->size, 8);
        EXPECT_EQ(node->leftSize, 0);
        EXPECT_EQ(node->height, 1);
        text += node->text;
        return node->size;
    }

    EXPECT_NE(node->left, nullptr);
    EXPECT_NE(node->right, nullptr);
    if (node->left == nullptr || node->right == nullptr) {
        return 0;
    }
    int leftSize = validateTree(node->left, text);
    int rightSize = validateTree(node->right, text);
    EXPECT_EQ(node->leftSize, leftSize);
    EXPECT_EQ(node->size, leftSize + rightSize);
    EXPECT_EQ(node->height, 1 + max(Node::nodeHeight(node->left),
                                   Node::nodeHeight(node->right)));
    return leftSize + rightSize;
}

static string runProgram(const string& input) {
    static const bool streamsConfigured = [] {
        ios::sync_with_stdio(false);
        cin.tie(nullptr);
        return true;
    }();
    (void)streamsConfigured;

    istringstream inputStream(input);
    ostringstream outputStream;
    cin.clear();
    cout.clear();
    streambuf* oldInput = cin.rdbuf(inputStream.rdbuf());
    streambuf* oldOutput = cout.rdbuf(outputStream.rdbuf());

    int status = rope_program_main();
    cout.flush();
    cin.rdbuf(oldInput);
    cout.rdbuf(oldOutput);
    cin.clear();
    cout.clear();

    EXPECT_EQ(status, 0);
    return outputStream.str();
}

static string randomText(mt19937& rng, int maxLength) {
    static const string alphabet = "abCD 012";
    uniform_int_distribution<int> lengthDistribution(0, maxLength);
    uniform_int_distribution<int> charDistribution(0,
                                                    static_cast<int>(alphabet.size()) - 1);
    int length = lengthDistribution(rng);
    string result;
    for (int i = 0; i < length; ++i) {
        result += alphabet[charDistribution(rng)];
    }
    return result;
}

TEST(RopeBuild, EmptyAndChunkedStringsKeepMetadata) {
    EXPECT_EQ(buildFromString(""), nullptr);

    for (int length = 1; length <= 160; ++length) {
        string source;
        for (int i = 0; i < length; ++i) {
            source += static_cast<char>('a' + (i % 26));
        }

        Node* root = buildFromString(source);
        string actual;
        EXPECT_EQ(validateTree(root, actual), length);
        EXPECT_EQ(actual, source);
        deleteTree(root);
    }
}

TEST(RopeJoin, PreservesOrderAndCoalescesSmallLeaves) {
    Node* small = join(makeLeaf("abcd"), makeLeaf("ef"));
    ASSERT_NE(small, nullptr);
    EXPECT_TRUE(small->isLeaf);
    EXPECT_EQ(small->text, "abcdef");
    deleteTree(small);

    string left(137, 'L');
    string right(91, 'R');
    Node* root = join(buildFromString(left), buildFromString(right));
    EXPECT_EQ(flatten(root), left + right);
    EXPECT_EQ(Node::nodeSize(root), static_cast<int>(left.size() + right.size()));
    deleteTree(root);
}

TEST(RopeSplit, EveryBoundaryCanBeRestored) {
    string source;
    for (int i = 0; i < 73; ++i) {
        source += static_cast<char>('A' + (i % 26));
    }

    for (int position = 0; position <= static_cast<int>(source.size()); ++position) {
        Node* root = buildFromString(source);
        SplitResult parts = split(root, position);
        EXPECT_EQ(flatten(parts.prefix), source.substr(0, position));
        EXPECT_EQ(flatten(parts.suffix), source.substr(position));
        root = join(parts.prefix, parts.suffix);
        EXPECT_EQ(flatten(root), source);
        EXPECT_EQ(Node::nodeSize(root), static_cast<int>(source.size()));
        deleteTree(root);
    }
}

TEST(RopeProgram, RunsAllOperationsWithExpectedOutput) {
    const string input =
        "abcdef\n"
        "5\n"
        "concat|XYZ\n"
        "split|3\n"
        "index|8\n"
        "insert|3|++\n"
        "delete|2|3\n";
    const string expected =
        "abcdef\n"
        "abcdefXYZ\n"
        "abc|defXYZ\n"
        "Z\n"
        "abc++defXYZ\n"
        "abdefXYZ\n";
    EXPECT_EQ(runProgram(input), expected);
}

TEST(RopeProgram, HandlesEmptyInputAndBoundaryPositions) {
    EXPECT_EQ(runProgram("\n3\nconcat|A\nsplit|0\nindex|0\n"),
              "\nA\n|A\nA\n");

    const string input =
        "abcdefghijk\n"
        "5\n"
        "split|0\n"
        "split|11\n"
        "split|4\n"
        "insert|11|!\n"
        "delete|0|12\n";
    const string expected =
        "abcdefghijk\n"
        "|abcdefghijk\n"
        "abcdefghijk|\n"
        "abcd|efghijk\n"
        "abcdefghijk!\n"
        "\n";
    EXPECT_EQ(runProgram(input), expected);
}

TEST(RopeProgram, RandomOperationSequencesMatchStdString) {
    mt19937 rng(2026);
    uniform_int_distribution<int> initialLength(0, 20);
    uniform_int_distribution<int> operationCount(30, 80);
    uniform_int_distribution<int> operationType(0, 4);

    for (int trial = 0; trial < 80; ++trial) {
        string model = randomText(rng, initialLength(rng));
        int count = operationCount(rng);
        ostringstream input;
        ostringstream expected;
        input << model << '\n' << count << '\n';
        expected << model << '\n';

        for (int step = 0; step < count; ++step) {
            int kind = operationType(rng);
            if (kind == 0) {
                string text = randomText(rng, 8);
                input << "concat|" << text << '\n';
                model += text;
                expected << model << '\n';
            } else if (kind == 1) {
                uniform_int_distribution<int> positionDistribution(
                    0, static_cast<int>(model.size()));
                int position = positionDistribution(rng);
                input << "split|" << position << '\n';
                expected << model.substr(0, position) << '|'
                         << model.substr(position) << '\n';
            } else if (kind == 2 && !model.empty()) {
                uniform_int_distribution<int> positionDistribution(
                    0, static_cast<int>(model.size()) - 1);
                int position = positionDistribution(rng);
                input << "index|" << position << '\n';
                expected << model[position] << '\n';
            } else if (kind == 3) {
                uniform_int_distribution<int> positionDistribution(
                    0, static_cast<int>(model.size()));
                int position = positionDistribution(rng);
                string text = randomText(rng, 8);
                input << "insert|" << position << '|' << text << '\n';
                model.insert(static_cast<size_t>(position), text);
                expected << model << '\n';
            } else {
                uniform_int_distribution<int> positionDistribution(
                    0, static_cast<int>(model.size()));
                int position = positionDistribution(rng);
                uniform_int_distribution<int> countDistribution(
                    0, static_cast<int>(model.size()) - position);
                int removeCount = countDistribution(rng);
                input << "delete|" << position << '|' << removeCount << '\n';
                model.erase(static_cast<size_t>(position),
                            static_cast<size_t>(removeCount));
                expected << model << '\n';
            }
        }

        EXPECT_EQ(runProgram(input.str()), expected.str()) << "trial " << trial;
    }
}

TEST(RopeValidation, InvalidArgumentsAreRejectedAndTreeIsUnchanged) {
    const string source = "Hello, beautiful world!";
    Node* root = buildFromString(source);

    // Чтение: пустое дерево и индексы вне диапазона.
    EXPECT_THROW(ropeIndex(nullptr, 0), out_of_range);
    EXPECT_THROW(ropeIndex(root, -1), out_of_range);
    EXPECT_THROW(ropeIndex(root, static_cast<int>(source.size())), out_of_range);
    EXPECT_EQ(ropeIndex(root, 0), 'H');
    EXPECT_EQ(ropeIndex(root, static_cast<int>(source.size()) - 1), '!');

    // Разбиение: позиция должна быть в диапазоне [0, длина].
    EXPECT_THROW(split(root, -1), out_of_range);
    EXPECT_THROW(split(root, static_cast<int>(source.size()) + 1), out_of_range);
    EXPECT_THROW(split(nullptr, 1), out_of_range);

    // Вставка: неверная позиция и запрещённые символы в тексте.
    EXPECT_THROW(ropeInsert(root, -1, "x"), out_of_range);
    EXPECT_THROW(ropeInsert(root, static_cast<int>(source.size()) + 1, "x"),
                 out_of_range);
    EXPECT_THROW(ropeInsert(root, 0, "a|b"), invalid_argument);
    EXPECT_THROW(ropeConcat(root, "a|b"), invalid_argument);

    // Удаление: отрицательные значения и выход диапазона за границу строки.
    int length = static_cast<int>(source.size());
    EXPECT_THROW(ropeDelete(root, -1, 1), out_of_range);
    EXPECT_THROW(ropeDelete(root, 0, -1), out_of_range);
    EXPECT_THROW(ropeDelete(root, length + 1, 0), out_of_range);
    EXPECT_THROW(ropeDelete(root, length - 2, 3), out_of_range);
    EXPECT_THROW(ropeDelete(root, 0, length + 1), out_of_range);

    // После всех отклонённых вызовов дерево осталось прежним и корректным.
    string actual;
    EXPECT_EQ(validateTree(root, actual), length);
    EXPECT_EQ(actual, source);

    // Граничные допустимые значения не вызывают исключений.
    root = ropeDelete(root, length, 0);
    root = ropeInsert(root, length, "!");
    EXPECT_EQ(flatten(root), source + "!");
    deleteTree(root);

    // Пустое дерево: допустимы только позиция 0 и пустое удаление.
    EXPECT_THROW(ropeDelete(nullptr, 0, 1), out_of_range);
    EXPECT_EQ(ropeDelete(nullptr, 0, 0), nullptr);
    Node* created = ropeInsert(nullptr, 0, "abc");
    EXPECT_EQ(flatten(created), "abc");
    deleteTree(created);
}

TEST(RopeValidation, ParseIntAcceptsOnlyWholeNumbers) {
    EXPECT_EQ(parseInt("42"), 42);
    EXPECT_EQ(parseInt("-7"), -7);
    EXPECT_THROW(parseInt(""), invalid_argument);
    EXPECT_THROW(parseInt("abc"), invalid_argument);
    EXPECT_THROW(parseInt("12abc"), invalid_argument);
    EXPECT_THROW(parseInt("99999999999"), out_of_range);
}

TEST(RopeProgram, InvalidCommandsPrintErrorAndKeepRope) {
    const string input =
        "abcdef\n"
        "11\n"
        "index|6\n"
        "index|-1\n"
        "split|7\n"
        "insert|9|x\n"
        "delete|4|5\n"
        "delete|2\n"
        "index|x\n"
        "unknown|1\n"
        "concat|a|b\n"
        "concat\n"
        "index|5\n";
    const string expected =
        "abcdef\n"
        "error: index: position out of range\n"
        "error: index: position out of range\n"
        "error: split: position out of range\n"
        "error: insert: position out of range\n"
        "error: delete: range out of bounds\n"
        "error: delete: count argument missing\n"
        "error: invalid number\n"
        "error: unknown command\n"
        "error: text contains forbidden character\n"
        "error: command without arguments\n"
        "f\n";
    EXPECT_EQ(runProgram(input), expected);

    // Чтение из пустой строки не приводит к падению.
    EXPECT_EQ(runProgram("\n2\nindex|0\ndelete|0|1\n"),
              "\nerror: index: position out of range\n"
              "error: delete: range out of bounds\n");
}
