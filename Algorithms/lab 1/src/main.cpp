#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "modules/circular_deque.h"

using namespace std;

static void print_help() {
    cout << "Команды:\n"
         << "  push_front X   — добавить X в начало\n"
         << "  push_back X    — добавить X в конец\n"
         << "  pop_front      — извлечь элемент из начала\n"
         << "  pop_back       — извлечь элемент из конца\n"
         << "  front          — первый элемент\n"
         << "  back           — последний элемент\n"
         << "  size           — количество элементов\n"
         << "  empty          — пуст ли дек\n"
         << "  full           — заполнен ли дек\n"
         << "  resize N       — изменить вместимость\n"
         << "  help           — эта справка\n"
         << "  exit           — выход\n";
}

int main() {
    size_t n;
    int force;
    cout << "Введите вместимость дека N и режим push_force (0/1): ";
    if (!(cin >> n >> force)) {
        cerr << "Ошибка: ожидались два числа" << endl;
        return 1;
    }

    CircularDeque* dq;
    try {
        dq = new CircularDeque(n, force != 0);
    } catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << endl;
        return 1;
    }

    print_help();

    string line;
    getline(cin, line);
    while (cout << "> " && getline(cin, line)) {
        istringstream in(line);
        string cmd;
        if (!(in >> cmd)) continue;

        try {
            if (cmd == "push_front" || cmd == "push_back" || cmd == "resize") {
                long long x;
                if (!(in >> x)) {
                    cout << "Ошибка: команде " << cmd << " нужен аргумент" << endl;
                    continue;
                }
                if (cmd == "push_front") dq->push_front(static_cast<int>(x));
                else if (cmd == "push_back") dq->push_back(static_cast<int>(x));
                else {
                    if (x <= 0) throw invalid_argument("new_cap must be > 0");
                    dq->resize(static_cast<size_t>(x));
                }
                cout << "OK" << endl;
            } else if (cmd == "pop_front") {
                cout << dq->pop_front() << endl;
            } else if (cmd == "pop_back") {
                cout << dq->pop_back() << endl;
            } else if (cmd == "front") {
                cout << dq->front() << endl;
            } else if (cmd == "back") {
                cout << dq->back() << endl;
            } else if (cmd == "size") {
                cout << dq->size() << endl;
            } else if (cmd == "empty") {
                cout << (dq->empty() ? "true" : "false") << endl;
            } else if (cmd == "full") {
                cout << (dq->full() ? "true" : "false") << endl;
            } else if (cmd == "help") {
                print_help();
            } else if (cmd == "exit" || cmd == "quit") {
                break;
            } else {
                cout << "Неизвестная команда: " << cmd << " (help — список команд)" << endl;
            }
        } catch (const exception& e) {
            cout << "Ошибка: " << e.what() << endl;
        }
    }

    delete dq;
    return 0;
}
