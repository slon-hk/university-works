Создайте API (application programming interface - в данном случае набор функций) для работы с циклическим однонаправленным списком.

Список хранит значения типа int. Структуры списка выглядят следующим образом:

struct ListNode {
    struct ListNode *next;  // указатель на следующий элемент
    int data;  // данные для хранения в узле
};

struct ListStruct {
    struct ListNode *head;  // указатель на голову
};

Структуры объявлять не нужно! Они уже есть в проверяющем коде.

Необходимо реализовать следующий API (функции) для взаимодействия со списоком:

    struct ListStruct *list_init();
        Создаёт новый список и возврщает на него указатель. Список должен быть пустым
    void list_destroy(struct ListStruct *list);
        Очищает список list и всего его элементы
    void list_push_front(struct ListStruct *list, int data);
        Добавляет новый элемент в начало списка
        list -- список, в который необходимо добавить элемент
        data -- данные, которые нужно добавить
    void list_push_back(struct ListStruct *list, int data);
        Добавляет новый элемент в конец списка
        list -- список, в который необходимо добавить элемент
        data -- данные, которые нужно добавить
    void list_push(struct ListStruct *list, struct ListNode *prev_node, int data);
        Добавляет новый элемент после переданного
        list -- список, в который необходимо добавить элемент
        prev_node -- узел, после которого нужно вставить новый элемент
        data -- данные, которые нужно добавить
        Если prev_node равняется NULL, то элемент необходимо добавить в конец списка
    void list_pop(struct ListStruct *list, struct ListNode *node_to_remove);
        Удаляет элемент из списка
        list -- список, из которого необходимо удалить элемент
        node_to_remove -- указатель на элемент, который необходимо удалить
    int list_count(struct ListStruct *list);
        Подсчитывает количество элементов в списке
        list -- список, для которого необходимо посчитать количество элементов
    struct ListNode *list_get_head(struct ListStruct *list);
        Возвращает указатель на первый элемент в списке
        list -- список, для которого необходимо вернуть указатель на первый элемент
    struct ListNode *list_get_tail(struct ListStruct *list);
        Возвращает указатель на последний элемент в списке
        list -- список, для которого необходимо вернуть указатель на последний элемент
    struct ListNode *list_get(struct ListStruct *list, int index);
        Возвращает указатель на элемент под индексом index
        list -- список, для которого необходимо вернуть указатель на последний элемент
        index -- индекс элемента, нумерация начинается с нуля
        Если index оказывается больше, чем длина списка, то необходимо вернуть NULL
    bool list_is_empty(struct ListStruct *list);
        Проверяет пуст ли список или нет
        Если список пуст, то необходимо вернуть true, иначе false
        list -- список, для проверки на пустоту
    void list_sort(struct ListStruct *list);
        Сортирует список по возрастанию значения поля data в элементах
        list -- список, который необходимо отсортировать

Функцию main реализовывать не нужно! Она реализована в проверке.

Обозначения в проверяющей системе:

    isEmpty -- вызывает list_is_empty и печатает результат
    getHead -- вызывает list_get_head и печатает содержимое элемента
    getTail -- вызывает list_get_tail и печатает содержимое элемента
    sort -- вызывает list_sort, ничего не печатает
    count -- вызывает list_count, печатает результат
    pop <num> -- вызывает list_pop для элемента под индексом num. Указатель на элемент берётся из функции list_get. Ничего не печатает
    push <index> <num> -- вызывает list_push с аргументами data=<num>  и prev_node из функции list_get для индекса <index>. (в коде выглядит примерно следующим образом list_push(list, list_get(list, index), num) ). Ничего не печатает
    pushHead <num> -- вызывает list_push_front с аргументом data=<num>, ничего не печатает
    pushTail <num> -- вызывает list_push_back с аргументом data=<num>, ничего не печатает
    end -- завершение последовательности команд, печатается весь список несколько раз.

Пример:

Test | Input            | Result                |
________________________________________________|
#1   |  getHead         | List is empty!        |
     |  push 8 -26624   | List is empty!        |
     |  push 5 29731    | 0                     |
     |  pushTail 90008  | Index out of range!   |
     |  sort            | 96978                 |
     |  pop 2           | 1                     |
     |  getHead         | 96978                 |
     |  count           | Index out of range!   |
     |  sort            | 1                     |
     |  pop 0           | 96978                 |
     |  pushTail -75695 | -26624                |
     |  push 1 7157     | 3                     |
     |  push 0 20954    | 5                     |
     |  count           | Index out of range!   |
     |  sort            | 92086                 |
     |  pushTail 92086  | 92086                 |
     |  pushHead -84396 | 0                     |
     |  pop 5           | 6                     |
     |  pop 10          | List (2 cycles):      |
     |  getTail         | -84396                |
     |  getTail         | -75695                |
     |  isEmpty         | 7157                  |
     |  count           | 20954                 |
     |  end             | 29731                 |
     |                  | 92086                 |
     |                  | -84396                |
     |                  | -75695                |
     |                  | 7157                  |
     |                  | 20954                 |
     |                  | 29731                 |
     |                  | 92086                 |
_________________________________________________

                