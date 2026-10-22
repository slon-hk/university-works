#include <iostream>
#include <vector>
#include <cmath>

using namespace std;
// Пока n >= MINRUN_LIMIT, n делится на 2 при вычислении min_run
const int MINRUN_LIMIT = 16;
// Сколько элементов подряд из одной части нужно взять, чтобы включился режим галопа
const int GALLOP = 3;
// Номер очередного слияния для вывода
int mergeCounter = 0;
// Отрезок массива (run): длина и индекс начала
struct Run {
    int len;
    int start;
};

// Выводит элементы через пробел
void printRange(const vector<int>& a, int lo, int hi) {
    for (int i = lo; i < hi; i++) {
        if (i > lo) cout << " ";
        cout << a[i];
    }
    cout << "\n";
}

bool before(int a, int b) {
    return abs(a) > abs(b);
}
// Вычисляет min_run, старшие биты n плюс 1, если среди отброшенных битов была единица
int calc_minrun(int n) {
    int r = 0;
    while (n >= MINRUN_LIMIT) {
        r |= n & 1;
        n >>= 1;
    }
    return n + r;
}


void reverseRange(vector<int>& arr, int lo, int hi) {
    hi--;
    while (lo < hi) {
        int t = arr[lo];
        arr[lo] = arr[hi];
        arr[hi] = t;
        hi--;
        lo++;
    }
}
// Ищет естественный run, начиная с lo, и возвращает индекс его конца (не включая).
int find_run(vector<int>& arr, int lo, int n) {
    int hi = lo + 1;
    if (hi == n) return hi;
    if (abs(arr[lo]) < abs(arr[hi])) {
        while (hi < n && abs(arr[hi]) > abs(arr[hi - 1])) hi++;
        reverseRange(arr, lo, hi);
    } else {
        while (hi < n && abs(arr[hi]) <= abs(arr[hi - 1])) hi++;
    }
    return hi;
}
// Сортировка вставками отрезка.
void insertion_sort(vector<int>& arr, int lo, int hi) {
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
// true, если x можно поставить раньше key
bool goesFirst(int x, int key) {
    return abs(x) >= abs(key);
}

// Галоп: считает, сколько элементов подряд идут раньше key.
// Сначала шаги находят границу, затем бинпоиск уточняет её
int gallop(const vector<int>& arr, int from, int end, int key) {
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

// Сливает соседние отсортированные отрезки arr[lo..mid) и arr[mid..hi).
// Возвращает число срабатываний галопа
int merge_runs(vector<int>& arr, int lo, int mid, int hi) {
    // Копии обеих частей, результат пишется сразу в arr
    vector<int> left(arr.begin() + lo, arr.begin() + mid);
    vector<int> right(arr.begin() + mid, arr.begin() + hi);
    int n_left = left.size();
    int n_right = right.size();
    int i = 0, j = 0, k = lo;
    // count_left / count_right - сколько элементов подряд взято из левой / правой части
    int count_left = 0, count_right = 0, gallops = 0;
    while (i < n_left && j < n_right) {
        // При равных модулях берём из левой части - так сохраняется устойчивость
        if (!before(right[j], left[i])) {
            arr[k++] = left[i++];
            count_left++;
            count_right = 0;
        } else {
            arr[k++] = right[j++];
            count_right++;
            count_left = 0;
        }
        // GALLOP элементов подряд из одной части - переносим сразу целый блок
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
    // Дописываем остаток той части, которая не закончилась
    while (i < n_left) arr[k++] = left[i++];
    while (j < n_right) arr[k++] = right[j++];
    return gallops;
}
// Сливает отрезки stack[k] и stack[k + 1], выводит результат и заменяет их в стеке одним
void mergeAt(vector<int>& arr, vector<Run>& stack, int k) {
    int lo = stack[k].start;
    int mid = lo + stack[k].len;
    int hi = mid + stack[k + 1].len;
    int gallops = merge_runs(arr, lo, mid, hi);
    cout << "Gallops " << mergeCounter << ": " << gallops << "\n";
    cout << "Merge " << mergeCounter << ": ";
    printRange(arr, lo, hi);
    mergeCounter++;
    stack[k].len += stack[k + 1].len;
    stack.erase(stack.begin() + k + 1);
}
// Восстанавливает инварианты стека для трёх верхних отрезков X, Y, Z (Z - верхний):
// X > Y + Z и Y > Z. При нарушении Y сливается с меньшим из соседей
void merge_collapse(vector<int>& arr, vector<Run>& stack) {
    while (stack.size() > 1) {
        int k = stack.size() - 2;
        // Вторая проверка смотрит на отрезок глубже - без неё инвариант
        // может нарушиться ниже вершины стека
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
// Сливает все оставшиеся в стеке отрезки в один после обработки всего массива
void mergeForceCollapse(vector<int>& arr, vector<Run>& stack) {
    while (stack.size() > 1) {
        int k = stack.size() - 2;
        if (k > 0 && stack[k - 1].len < stack[k + 1].len) k--;
        mergeAt(arr, stack, k);
    }
}
void tim_sort(vector<int>& arr, int n) {
    int min_run = calc_minrun(n);
    int lo = 0;
    int part = 0;
    vector<Run> runs;
    // Этап 1: разбиваем массив на отрезки длиной не меньше min_run
    // (короткий run добирается следующими элементами) и сортируем их вставками
    while (lo < n) {
        int hi = find_run(arr, lo, n);
        if (hi - lo < min_run) hi = min(lo + min_run, n);
        insertion_sort(arr, lo, hi);
        cout << "Part " << part << ": ";
        printRange(arr, lo, hi);
        part++;
        runs.push_back({hi - lo, lo});
        lo = hi;
    }
    // Этап 2: кладём отрезки в стек по одному, поддерживая инварианты,
    // а в конце сливаем всё, что осталось
    vector<Run> stack;
    for (int r = 0; r < (int)runs.size(); r++) {
        stack.push_back(runs[r]);
        merge_collapse(arr, stack);
    }
    mergeForceCollapse(arr, stack);
    cout << "Answer: ";
    printRange(arr, 0, n);
}
// Ввод: n, затем n целых чисел
int main() {
    int n;
    cin >> n;
    vector<int> arr(n);
    for (int i = 0; i < n; i++) cin >> arr[i];
    tim_sort(arr, n);
    return 0;
}
