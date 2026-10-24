<div align="center">

# 🎓 University Works

**Учебные работы — СПбГЭТУ «ЛЭТИ»**
Факультет компьютерных технологий и информатики (КТИ) · Прикладная математика и информатика (ПМИ)

![C++](https://img.shields.io/badge/C%2B%2B-20-00599C?logo=cplusplus&logoColor=white)
![C](https://img.shields.io/badge/C-A8B9CC?logo=c&logoColor=black)
![Python](https://img.shields.io/badge/Python-3-3776AB?logo=python&logoColor=white)
![Assembly](https://img.shields.io/badge/Assembly-x86-6E4C13)
![CMake](https://img.shields.io/badge/CMake-064F8C?logo=cmake&logoColor=white)
![GoogleTest](https://img.shields.io/badge/GoogleTest-4285F4?logo=google&logoColor=white)
![SDL3](https://img.shields.io/badge/SDL-3-1E90FF)

[Русский](#-о-репозитории) · [English](#-english)

</div>

---

## 📖 О репозитории

Лабораторные, практические и курсовые работы, выполненные во время учёбы. У каждой работы своя папка: исходный код в `src/`, рядом отчёт (`.docx` / `.pdf`), а где есть — условие задания (`Job_condition.md`).

<table>
  <tr>
    <td align="center"><b>26</b><br><sub>работ</sub></td>
    <td align="center"><b>6</b><br><sub>дисциплин</sub></td>
    <td align="center"><b>≈ 17 тыс.</b><br><sub>строк C++</sub></td>
    <td align="center"><b>≈ 2 тыс.</b><br><sub>строк C</sub></td>
    <td align="center"><b>≈ 0,9 тыс.</b><br><sub>строк Python</sub></td>
  </tr>
</table>

<sub>Строки посчитаны без сторонних библиотек (`vendor/`) и ассетов.</sub>

---

## ⭐ Главный проект — Robot Tactics

<table>
  <tr>
    <td width="55%">
      <a href="OOP"><img src="OOP/docs/screenshots/lab3_game_turns.png" alt="Robot Tactics"></a>
    </td>
    <td>
      <b>Пошаговая тактическая игра с роботами</b> на C++20 и SDL3. Игра развивалась на протяжении трёх лабораторных по ООП.<br><br>
      🗺️ фиксированные, случайные и бесконечные уровни<br>
      ⚔️ способности, статусы, инвентарь, ИИ врагов<br>
      💾 сохранение с контрольной суммой, журнал событий<br>
      ⌨️ настраиваемое управление с сочетаниями клавиш<br>
      🧬 83 класса и полная UML-диаграмма<br><br>
      <a href="OOP"><b>→ Подробнее, скриншоты и UML</b></a>
    </td>
  </tr>
</table>

---

## 📚 Дисциплины

| | Дисциплина | Язык и инструменты | Работы |
|:-:|---|---|:-:|
| 🎮 | [**Объектно-ориентированное программирование**](#-объектно-ориентированное-программирование) | C++20, SDL3, CMake | 3 |
| 🧮 | [**Алгоритмы и структуры данных**](#-алгоритмы-и-структуры-данных) | C++, CMake, GoogleTest | 2 |
| 💻 | [**Программирование**](#-программирование) | C, Make | 8 + 2 курсовые |
| 🐍 | [**Информатика**](#-информатика) | Python, NumPy, Pillow, scikit-learn | 6 |
| 🔌 | [**Организация ЭВМ и систем**](#-организация-эвм-и-систем) | Ассемблер x86 | 1 |
| 🔬 | [**Физика**](#-физика) | отчёты | 4 |

---

### 🎮 Объектно-ориентированное программирование

<sub>C++20 · SDL3 · CMake → [подробный README с UML](OOP/README.md)</sub>

Одна игра, которая усложняется от работы к работе.

| Работа | Тема | Ключевые идеи |
|---|---|---|
| [lab 1](OOP/lab%201) | Робот, поле, пошаговый цикл, заводы роботов, туман войны, редактор карт | классы, инкапсуляция, отделение отрисовки от модели |
| [lab 2](OOP/lab%202) | Способности, статусы, клетки со статусами, инвентарь, ИИ врагов, спрайты | интерфейсы, наследование, полиморфизм |
| [lab 3](OOP/lab%203) | Шаблонная игра с тремя режимами, меню, сложность, сохранение, журнал, настраиваемое управление | шаблоны классов, независимые подсистемы ввода и отрисовки |

### 🧮 Алгоритмы и структуры данных

<sub>3 семестр · C++ · CMake · GoogleTest</sub>

| Работа | Тема | Что внутри |
|---|---|---|
| [lab 1](Algorithms/lab%201) | Циклический дек (`CircularDeque`) | реализация, интерактивный режим, юнит-тесты, бенчмарк и график на Python |
| [lab 2](Algorithms/lab%202) | Сортировка Timsort с режимом галопа | реализация, юнит-тесты, бенчмарк, результаты в `results.csv`, график |

### 💻 Программирование

<sub>1–2 семестр · C</sub>

| Семестр | Работа | Тема |
|:-:|---|---|
| 1 | [Laboratory work 1](Programming/Semester%201/Laboratory%20work%201) | Обработка массива, подзадачи в функциях |
| 1 | [Laboratory work 2](Programming/Semester%201/Laboratory%20work%202) | То же, но многофайловый проект с Makefile |
| 1 | [Laboratory work 3](Programming/Semester%201/Laboratory%20work%203) | Форматирование текста |
| 1 | [Laboratory work 4](Programming/Semester%201/Laboratory%20work%204) | Сортировки и сравнение времени работы |
| 1 | 🏆 [**Coursework**](Programming/Semester%201/Coursework) | Курсовая: обработка текста (вариант 4.11) |
| 2 | [Labatory work 1](Programming/Semester%202/Labatory%20work%201) | Регулярные выражения (`regex.h`) |
| 2 | [Labatory work 2](Programming/Semester%202/Labatory%20work%202) | Сортировка `qsort` |
| 2 | [Labatory work 3](Programming/Semester%202/Labatory%20work%203) | Рекурсивный обход директорий |
| 2 | [Laboratory work 4](Programming/Semester%202/Laboratory%20work%204) | Проверка валидности HTML-тегов (стек, C++) |
| 2 | 🏆 [**Coursework**](Programming/Semester%202/Coursework) | Курсовая: CLI-редактор PNG (вариант 4.20) |

### 🐍 Информатика

<sub>1–2 семестр · Python</sub>

| Семестр | Работа | Тема |
|:-:|---|---|
| 1 | [Laboratory work 1](Computer%20Science/1%20Semester/Laboratory%20work%201) | NumPy: линейная алгебра |
| 1 | [Laboratory work 2](Computer%20Science/1%20Semester/Laboratory%20work%202) | Pillow: рисование и обработка изображений |
| 1 | [Laboratory work 3](Computer%20Science/1%20Semester/Laboratory%20work%203) | Обработка строк |
| 2 | [Laboratory work 1](Computer%20Science/2%20Semester/Laboratory%20work%201) | ООП: классы и наследование |
| 2 | [Laboratory work 2](Computer%20Science/2%20Semester/Laboratory%20work%202) | Связный список |
| 2 | [Laboratory work 3](Computer%20Science/2%20Semester/Laboratory%20work%203) | Машинное обучение: scikit-learn, датасет Iris |

### 🔌 Организация ЭВМ и систем

<sub>Ассемблер x86</sub>

| Работа | Тема |
|---|---|
| [lab 1](ORGEVM/lab%201) | Первые программы на ассемблере: `HELLO1.ASM`, `HELLO2.ASM` |

### 🔬 Физика

<sub>2 семестр · отчёты по лабораторным</sub>

[lab 1](Physics/2%20sem/lab%201.docx) · [lab 4](Physics/2%20sem/lab%204.docx) · [lab 9](Physics/2%20sem/lab%209.docx) · [lab 12](Physics/2%20sem/lab%2012.docx)

---

## 🗂 Структура

<details>
<summary><b>Показать полное дерево файлов</b></summary>

```
university-works/
│
├── .gitignore
├── README.md
│
├── Algorithms/                              # Алгоритмы и структуры данных (3 семестр)
│   ├── lab 1/                               # Циклический дек (CircularDeque)
│   │   ├── CMakeLists.txt
│   │   ├── Stepanov_Leonid_lb1.docx / .pdf  # отчёт
│   │   └── src/
│   │       ├── main.cpp                     # интерактивная работа с деком
│   │       ├── tests.cpp                    # юнит-тесты (GoogleTest)
│   │       └── modules/
│   │           ├── README.md                # сборка и запуск
│   │           ├── circular_deque.h
│   │           ├── benchmark.cpp
│   │           └── plot_benchmark.py
│   │
│   └── lab 2/                               # Сортировка Timsort (с режимом галопа)
│       ├── CMakeLists.txt
│       ├── Stepanov_Leonid_lb2.docx / .pdf  # отчёт
│       └── src/
│           ├── main.cpp
│           ├── tests.cpp
│           └── modules/
│               ├── timsort.h
│               ├── benchmark.cpp
│               ├── plot_benchmark.py
│               └── results.csv              # результаты бенчмарка
│
├── Computer Science/                        # Информатика (Python)
│   ├── 1 Semester/
│   │   ├── Laboratory work 1/src/main.py    # NumPy: линейная алгебра
│   │   ├── Laboratory work 2/src/main.py    # Pillow: рисование и обработка изображений
│   │   └── Laboratory work 3/src/main.py    # обработка строк
│   │
│   └── 2 Semester/
│       ├── Laboratory work 1/src/           # ООП: классы и наследование
│       │   ├── main.py
│       │   └── 538723СТЕПАНОВ_ОТЧЕТ_ЛР1.docx / .pdf
│       ├── Laboratory work 2/src/           # связный список
│       │   ├── main.py
│       │   └── 538723СТЕПАНОВ_ОТЧЕТ_ЛР2.docx / .pdf
│       └── Laboratory work 3/               # машинное обучение (scikit-learn, Iris)
│           ├── src/main.py
│           └── 538723СТЕПАНОВ_ОТЧЕТ_ЛР3.docx / .ipynb / .pdf
│
├── OOP/                                     # Объектно-ориентированное программирование (C++20, SDL3)
│   ├── README.md                            # описание игры, скриншоты, UML
│   ├── docs/
│   │   ├── screenshots/                     # скриншоты всех трёх версий
│   │   └── uml/                             # диаграммы классов, генератор, исходник PlantUML
│   │
│   ├── lab 1/                               # игра с роботами на игровом поле + редактор карт
│   │   ├── отчет.docx
│   │   ├── src/                             # Robot, Robot_factory, Game_board,
│   │   │                                    # GameSession, MapEditor, GUI
│   │   └── vendor/                          # сторонние библиотеки: SDL3, SDL3_ttf
│   │
│   ├── lab 2/                               # способности, статусы, предметы, ИИ врагов
│   │   ├── CMakeLists.txt
│   │   ├── Отчет ЛР2.docx
│   │   └── src/                             # abilities, board, game, gui, items,
│   │                                        # robot, statuses, assets, fonts
│   │
│   └── lab 3/                               # шаблонная игра: режимы, меню, сложность,
│       ├── CMakeLists.txt                   # сохранение, журнал, настраиваемое управление
│       ├── Отчет ЛР3.docx
│       └── src/
│           ├── program/                     # Program, Command_line_options — точка входа
│           ├── game/                        # Application, Game_template, Game_session,
│           │                                # генераторы уровней, сложность
│           ├── input/                       # Sdl_input, Key_bindings
│           ├── gui/                         # Sdl_renderer, Board_view, Hud_view, Dialog_view
│           ├── save/                        # сериализация и проверка сохранений
│           ├── logging/                     # Logger, File_logger<Output>, Log_settings
│           ├── robot/, board/, abilities/, statuses/, items/
│           ├── levels/                      # level_1.txt … level_3.txt
│           ├── controls.txt                 # раскладка клавиш
│           ├── assets/                      # спрайты Tiny Swords (Free Pack)
│           └── fonts/Samson.ttf
│
├── ORGEVM/                                  # Организация ЭВМ и систем (ассемблер)
│   └── lab 1/
│       ├── Stepanov_Leonid_lb1.docx / .pdf
│       └── src/
│           ├── HELLO1.ASM
│           └── HELLO2.ASM
│
├── Physics/                                 # Физика
│   └── 2 sem/
│       └── lab 1.docx, lab 4.docx, lab 9.docx, lab 12.docx
│
└── Programming/                             # Программирование (C)
    ├── Semester 1/
    │   ├── Laboratory work 1/               # обработка массива, подзадачи в функциях
    │   │   ├── Job_condition.md
    │   │   └── src/main.c
    │   ├── Laboratory work 2/               # то же, но многофайловый проект с Makefile
    │   │   ├── Job_condition.md
    │   │   └── src/
    │   │       ├── Makefile
    │   │       ├── menu.c
    │   │       └── abs_max.c/.h, abs_min.c/.h, diff.c/.h, sum.c/.h
    │   ├── Laboratory work 3/               # форматирование текста
    │   │   ├── Job_condition.md
    │   │   └── src/main.c
    │   ├── Laboratory work 4/               # сортировки и сравнение времени
    │   │   ├── Job_condition.md
    │   │   └── src/main.c
    │   └── Coursework/                      # курсовая: обработка текста (вариант 4.11)
    │       ├── cw_report.docx / .pdf
    │       └── src/
    │           ├── Makefile
    │           ├── main.c
    │           ├── one.c, two.c, three.c, four.c,
    │           │   pred_obrabotka.c, slon_tools.c
    │           └── include/                 # заголовки модулей
    │
    └── Semester 2/
        ├── Labatory work 1/src/main.c       # регулярные выражения (regex.h)
        ├── Labatory work 2/src/main.c       # сортировка qsort
        ├── Labatory work 3/src/main.c       # рекурсивный обход директорий
        ├── Laboratory work 4/               # проверка валидности HTML-тегов (стек, C++)
        │   ├── Job_condition.md
        │   └── src /main.cpp
        └── Coursework/                      # курсовая: CLI-редактор PNG (вариант 4.20)
            ├── cw_report.docx / .pdf
            └── src/
                ├── Makefile
                ├── main.c
                ├── png_utils.c
                └── include/png_utils.h
```

</details>

---

## 🔨 Сборка и запуск

| Тип проекта | Где | Команды |
|---|---|---|
| CMake (C++) | `Algorithms/*`, `OOP/*` | `cmake -S . -B build && cmake --build build` |
| Makefile (C) | `Programming/*/src` | `make` |
| Python | `Computer Science/*/src` | `python3 main.py` |

> [!TIP]
> Подробная инструкция для OOP lab 3, включая подключение SDL и аргументы командной строки, — в [OOP/README.md](OOP/README.md#-сборка-и-запуск).

---

## 🇬🇧 English

Academic works completed at **Saint Petersburg Electrotechnical University "LETI"**: Faculty of Computer Technologies and Informatics, program *Applied Mathematics and Informatics*.

| | Course | Stack | Highlights |
|:-:|---|---|---|
| 🎮 | [Object-Oriented Programming](OOP) | C++20, SDL3, CMake | **Robot Tactics**, a turn-based tactics game built over 3 labs: class templates, save/load with checksums, logging, configurable key bindings, 83 classes with a full [UML diagram](OOP/README.md#-uml-диаграмма-всего-кода) |
| 🧮 | [Algorithms & Data Structures](Algorithms) | C++, GoogleTest | Circular deque, Timsort with galloping mode; unit tests and benchmarks |
| 💻 | [Programming](Programming) | C, Make | 8 labs and 2 course works: text processing, regex, directory traversal, a CLI PNG editor |
| 🐍 | [Computer Science](Computer%20Science) | Python | NumPy, Pillow, OOP, linked list, scikit-learn (Iris) |
| 🔌 | [Computer Organization](ORGEVM) | x86 Assembly | First assembly programs |
| 🔬 | [Physics](Physics) | — | Lab reports |

Each work lives in its own folder: source code in `src/`, the report (`.docx` / `.pdf`) next to it, and the task statement (`Job_condition.md`) where available. `vendor/` folders contain third-party library sources (SDL3, SDL3_ttf), not authored code.

---

<div align="center">
<sub>Все материалы созданы в учебных целях · All materials were created for educational purposes</sub>
</div>
