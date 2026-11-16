#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

// Максимальная длина текста в одном листе Rope.
const int MAX_LEAF_SIZE = 8;

// Узел бинарного дерева Rope. Лист хранит фрагмент строки (1..8
// символов), внутренний узел хранит метаданные о своих поддеревьях.
struct Node {
    bool isLeaf;    // true - лист, false - внутренний узел
    string text;    // текст листа (у внутреннего узла пуст)
    Node* left;     // левое поддерево (nullptr у листа)
    Node* right;    // правое поддерево (nullptr у листа)
    int size;       // суммарная длина текста в поддереве
    int leftSize;   // суммарная длина текста в левом поддереве
    int height;     // высота поддерева (у листа равна 1)

    // Конструктор листа: value - текст фрагмента (не пустой, до 8
    // символов).
    explicit Node(const string& value)
        : isLeaf(true), text(value), left(nullptr), right(nullptr),
          size(static_cast<int>(value.size())), leftSize(0),
          height(1) {}

    // Конструктор внутреннего узла: leftChild и rightChild -
    // непустые поддеревья; метаданные вычисляются функцией update.
    Node(Node* leftChild, Node* rightChild)
        : isLeaf(false), text(), left(leftChild), right(rightChild),
          size(0), leftSize(0), height(1) {
        update(this);
    }

    // Возвращает размер поддерева; для пустого дерева (nullptr) -
    // 0.
    static int nodeSize(Node* node) {
        return node == nullptr ? 0 : node->size;
    }

    // Возвращает высоту поддерева; для пустого дерева (nullptr) -
    // 0.
    static int nodeHeight(Node* node) {
        return node == nullptr ? 0 : node->height;
    }

    // Пересчитывает leftSize, size и height внутреннего узла по его
    // детям. Для nullptr и листа ничего не делает.
    static void update(Node* node) {
        if (node == nullptr || node->isLeaf) {
            return;
        }

        node->leftSize = nodeSize(node->left);
        node->size = node->leftSize + nodeSize(node->right);

        int leftHeight = nodeHeight(node->left);
        int rightHeight = nodeHeight(node->right);
        int maxHeight = leftHeight > rightHeight ? leftHeight
                                                 : rightHeight;
        node->height = 1 + maxHeight;
    }
};

// Создаёт лист с текстом text. Для пустой строки возвращает nullptr
// (пустая строка представляется пустым деревом).
Node* makeLeaf(const string& text) {
    if (text.empty()) {
        return nullptr;
    }
    return new Node(text);
}

// Создаёт внутренний узел из двух поддеревьев. Если одно из них
// пустое, возвращает другое, поэтому внутренний узел всегда имеет
// двух детей.
Node* makeInternal(Node* left, Node* right) {
    if (left == nullptr) {
        return right;
    }
    if (right == nullptr) {
        return left;
    }
    return new Node(left, right);
}

// Правый поворот вокруг root. Возвращает новый корень поддерева.
// Если поворот невозможен (нет левого ребёнка), возвращает root без
// изменений.
Node* rotateRight(Node* root) {
    if (root == nullptr || root->left == nullptr) {
        return root;
    }

    Node* newRoot = root->left;
    root->left = newRoot->right;
    newRoot->right = root;

    Node::update(root);
    Node::update(newRoot);
    return newRoot;
}

// Левый поворот вокруг root. Возвращает новый корень поддерева.
// Если поворот невозможен (нет правого ребёнка), возвращает root
// без изменений.
Node* rotateLeft(Node* root) {
    if (root == nullptr || root->right == nullptr) {
        return root;
    }

    Node* newRoot = root->right;
    root->right = newRoot->left;
    newRoot->left = root;

    Node::update(root);
    Node::update(newRoot);
    return newRoot;
}

// Восстанавливает AVL-баланс в корне поддерева root одиночным или
// двойным поворотом. Возвращает новый корень поддерева.
Node* rebalance(Node* root) {
    if (root == nullptr || root->isLeaf) {
        return root;
    }

    Node::update(root);
    int balance = Node::nodeHeight(root->left) -
                  Node::nodeHeight(root->right);

    if (balance > 1) {
        // Левое поддерево выше: при необходимости сначала
        // поворачиваем его влево (двойной поворот).
        if (Node::nodeHeight(root->left->left) <
            Node::nodeHeight(root->left->right)) {
            root->left = rotateLeft(root->left);
        }
        return rotateRight(root);
    }

    if (balance < -1) {
        // Правое поддерево выше: симметричный случай.
        if (Node::nodeHeight(root->right->right) <
            Node::nodeHeight(root->right->left)) {
            root->right = rotateRight(root->right);
        }
        return rotateLeft(root);
    }

    return root;
}

// Соединяет деревья left и right, сохраняя порядок символов (left -
// слева). Любое из деревьев может быть пустым (nullptr). Два
// маленьких листа, суммарная длина которых не больше 8, сливаются в
// один. Возвращает корень результата; владение узлами переходит к
// нему.
Node* join(Node* left, Node* right) {
    if (left == nullptr) {
        return right;
    }
    if (right == nullptr) {
        return left;
    }

    // Слияние двух коротких листьев в один.
    if (left->isLeaf && right->isLeaf &&
        left->size + right->size <= MAX_LEAF_SIZE) {
        string combined = left->text + right->text;
        delete left;
        delete right;
        return makeLeaf(combined);
    }

    int leftHeight = Node::nodeHeight(left);
    int rightHeight = Node::nodeHeight(right);

    // Левое дерево заметно выше: присоединяем правое к его правой
    // ветви.
    if (leftHeight > rightHeight + 1) {
        left->right = join(left->right, right);
        return rebalance(left);
    }

    // Правое дерево заметно выше: присоединяем левое к его левой
    // ветви.
    if (rightHeight > leftHeight + 1) {
        right->left = join(left, right->left);
        return rebalance(right);
    }

    // Высоты близки: достаточно создать новый внутренний узел.
    return makeInternal(left, right);
}

// Результат разбиения: два самостоятельных дерева.
struct SplitResult {
    Node* prefix;   // первые i символов
    Node* suffix;   // остальные символы
};

// Разделяет дерево root после i символов (0 <= i <= длина строки).
// Узлы, пересекающие границу, перестраиваются; возвращённые деревья
// владеют своими узлами. При i < 0 или i > длины строки выбрасывает
// out_of_range, при этом дерево не изменяется.
SplitResult split(Node* root, int i) {
    if (i < 0 || i > Node::nodeSize(root)) {
        throw out_of_range("split: position out of range");
    }

    if (root == nullptr) {
        return {nullptr, nullptr};
    }

    // Граница совпадает с краем всего дерева.
    if (i == 0) {
        return {nullptr, root};
    }
    if (i == root->size) {
        return {root, nullptr};
    }

    // Граница внутри листа: делим текст на две части.
    if (root->isLeaf) {
        string prefixText = root->text.substr(0, i);
        string suffixText = root->text.substr(i);
        delete root;
        return {makeLeaf(prefixText), makeLeaf(suffixText)};
    }

    int leftSize = root->leftSize;
    Node* left = root->left;
    Node* right = root->right;

    // Граница точно между левым и правым поддеревьями.
    if (i == leftSize) {
        delete root;
        return {left, right};
    }

    // Граница в левом поддереве: правое присоединяется к суффиксу.
    if (i < leftSize) {
        SplitResult parts = split(left, i);
        delete root;
        return {parts.prefix, join(parts.suffix, right)};
    }

    // Граница в правом поддереве: левое присоединяется к префиксу.
    SplitResult parts = split(right, i - leftSize);
    delete root;
    return {join(left, parts.prefix), parts.suffix};
}

// Строит сбалансированное дерево из строки text, разбивая её на
// листья по 8 символов. Для пустой строки возвращает nullptr.
Node* buildFromString(const string& text) {
    vector<Node*> leaves;

    int length = static_cast<int>(text.size());
    for (int i = 0; i < length; i += MAX_LEAF_SIZE) {
        leaves.push_back(makeLeaf(text.substr(i, MAX_LEAF_SIZE)));
    }

    if (leaves.empty()) {
        return nullptr;
    }

    // Строим сбалансированное дерево из листьев: на каждом шаге
    // соединяем пары соседних узлов.
    while (leaves.size() > 1) {
        vector<Node*> nextLevel;

        for (size_t i = 0; i < leaves.size(); i += 2) {
            if (i + 1 < leaves.size()) {
                nextLevel.push_back(
                    makeInternal(leaves[i], leaves[i + 1]));
            } else {
                nextLevel.push_back(leaves[i]);
            }
        }

        leaves = nextLevel;
    }

    return leaves[0];
}

// Дописывает в result текст всего дерева root (обход листьев слева
// направо).
void appendText(Node* root, string& result) {
    if (root == nullptr) {
        return;
    }

    if (root->isLeaf) {
        result += root->text;
        return;
    }

    appendText(root->left, result);
    appendText(root->right, result);
}

// Рекурсивно освобождает память всех узлов дерева root.
void deleteTree(Node* root) {
    if (root == nullptr) {
        return;
    }

    if (!root->isLeaf) {
        deleteTree(root->left);
        deleteTree(root->right);
    }

    delete root;
}

// Проверяет, что текст допустим для операций: в нём нет символа '|'
// и перевода строки. Иначе выбрасывает invalid_argument.
void checkText(const string& text) {
    if (text.find('|') != string::npos ||
        text.find('\n') != string::npos) {
        throw invalid_argument("text contains forbidden character");
    }
}

// Возвращает символ с индексом index (0 <= index < длина строки).
// Для пустого дерева или индекса вне диапазона выбрасывает
// out_of_range, поэтому поиск по дереву не начинается при неверных
// данных.
char ropeIndex(Node* root, int index) {
    if (index < 0 || index >= Node::nodeSize(root)) {
        throw out_of_range("index: position out of range");
    }

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

// Приписывает text справа к дереву root. Возвращает корень
// результата. При недопустимом тексте выбрасывает invalid_argument
// (дерево не меняется).
Node* ropeConcat(Node* root, const string& text) {
    checkText(text);
    return join(root, buildFromString(text));
}

// Вставляет text перед символом с индексом index (0 <= index <=
// длина). Возвращает корень результата. При неверных данных
// выбрасывает out_of_range или invalid_argument; дерево при этом не
// изменяется.
Node* ropeInsert(Node* root, int index, const string& text) {
    if (index < 0 || index > Node::nodeSize(root)) {
        throw out_of_range("insert: position out of range");
    }
    checkText(text);

    SplitResult parts = split(root, index);
    Node* middle = buildFromString(text);
    return join(join(parts.prefix, middle), parts.suffix);
}

// Удаляет count символов начиная с index, то есть полуинтервал
// [index, index + count). Требуется 0 <= index <= длина, count >= 0
// и index + count <= длина. Возвращает корень результата. При
// неверных данных выбрасывает out_of_range; дерево при этом не
// изменяется.
Node* ropeDelete(Node* root, int index, int count) {
    int length = Node::nodeSize(root);
    if (index < 0 || index > length || count < 0 ||
        count > length - index) {
        throw out_of_range("delete: range out of bounds");
    }

    SplitResult firstSplit = split(root, index);
    SplitResult secondSplit = split(firstSplit.suffix, count);

    deleteTree(secondSplit.prefix);
    return join(firstSplit.prefix, secondSplit.suffix);
}

// Преобразует строку в целое число. Строка должна целиком состоять
// из десятичного числа (допускается знак); иначе выбрасывается
// invalid_argument или out_of_range.
int parseInt(const string& text) {
    if (text.empty()) {
        throw invalid_argument("number expected");
    }

    size_t processed = 0;
    int value = 0;
    try {
        value = stoi(text, &processed);
    } catch (const invalid_argument&) {
        throw invalid_argument("invalid number");
    } catch (const out_of_range&) {
        throw out_of_range("number is too large");
    }
    if (processed != text.size()) {
        throw invalid_argument("invalid number");
    }
    return value;
}

// Убирает завершающий символ '\r' (входные файлы могут быть в
// формате Windows).
void trimCarriageReturn(string& line) {
    if (!line.empty() && line.back() == '\r') {
        line.pop_back();
    }
}

// Выполняет одну команду драйвера над деревом root и записывает в
// output строку результата (без перевода строки). Команда
// разбирается по символу '|'. При ошибке формата или недопустимых
// значениях выбрасывает исключение, при этом дерево root остаётся
// корректным и неизменным.
void executeCommand(Node*& root, const string& line,
                    string& output) {
    size_t firstBar = line.find('|');
    if (firstBar == string::npos) {
        throw invalid_argument("command without arguments");
    }

    string command = line.substr(0, firstBar);

    if (command == "concat") {
        // concat|S
        root = ropeConcat(root, line.substr(firstBar + 1));
        appendText(root, output);
    } else if (command == "split") {
        // split|i - вывод prefix|suffix; Rope после команды не
        // меняется.
        int index = parseInt(line.substr(firstBar + 1));
        SplitResult parts = split(root, index);

        string prefix;
        string suffix;
        appendText(parts.prefix, prefix);
        appendText(parts.suffix, suffix);
        output = prefix + '|' + suffix;

        // Восстанавливаем текущее дерево: split не меняет Rope.
        root = join(parts.prefix, parts.suffix);
    } else if (command == "index") {
        // index|i
        int index = parseInt(line.substr(firstBar + 1));
        output = string(1, ropeIndex(root, index));
    } else if (command == "insert") {
        // insert|i|S
        size_t secondBar = line.find('|', firstBar + 1);
        if (secondBar == string::npos) {
            throw invalid_argument("insert: text argument missing");
        }
        int index = parseInt(
            line.substr(firstBar + 1, secondBar - firstBar - 1));
        root = ropeInsert(root, index, line.substr(secondBar + 1));
        appendText(root, output);
    } else if (command == "delete") {
        // delete|i|k
        size_t secondBar = line.find('|', firstBar + 1);
        if (secondBar == string::npos) {
            throw invalid_argument(
                "delete: count argument missing");
        }
        int index = parseInt(
            line.substr(firstBar + 1, secondBar - firstBar - 1));
        int count = parseInt(line.substr(secondBar + 1));
        root = ropeDelete(root, index, count);
        appendText(root, output);
    } else {
        throw invalid_argument("unknown command");
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Первая строка - начальная строка (может быть пустой).
    string initial;
    if (!getline(cin, initial)) {
        return 0;
    }
    trimCarriageReturn(initial);

    // Вторая строка - число команд q; оно должно быть
    // неотрицательным.
    string line;
    int q = 0;
    if (!getline(cin, line)) {
        cerr << "error: number of commands is missing\n";
        return 1;
    }
    trimCarriageReturn(line);
    try {
        q = parseInt(line);
        if (q < 0) {
            throw out_of_range("negative number of commands");
        }
    } catch (const exception& error) {
        cerr << "error: invalid number of commands ("
             << error.what() << ")\n";
        return 1;
    }

    // Начальная строка не должна содержать разделитель команд.
    if (initial.find('|') != string::npos) {
        cerr << "error: initial string contains '|'\n";
        return 1;
    }

    Node* root = buildFromString(initial);
    cout << initial << '\n';

    for (int operation = 0; operation < q; ++operation) {
        // Если команд меньше заявленного, завершаем чтение.
        if (!getline(cin, line)) {
            break;
        }
        trimCarriageReturn(line);

        string result;
        try {
            executeCommand(root, line, result);
            cout << result << '\n';
        } catch (const exception& error) {
            // Ошибочная команда не меняет Rope; в вывод идёт одна
            // строка.
            cout << "error: " << error.what() << '\n';
        }
    }

    deleteTree(root);
    return 0;
}
