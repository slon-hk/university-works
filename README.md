# University Works

## 🇷🇺 Описание (Русский)

Этот репозиторий содержит учебные работы, выполненные в рамках обучения в **СПБГЭТУ «ЛЭТИ»**.  
Факультет **КТИ** (Компьютерных технологий и информатики).  
Направление подготовки **ПМИ** (Прикладная математика и информатика).

Репозиторий предназначен для хранения и систематизации лабораторных, практических и курсовых работ по различным дисциплинам, изучаемым в ходе обучения.

### 📁 Структура репозитория

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
├── OOP/                                     # Объектно-ориентированное программирование (C++, SDL3)
│   ├── lab 1/                               # игра с роботами на игровом поле + редактор карт
│   │   ├── отчет.docx
│   │   ├── src/
│   │   │   ├── CMakeLists.txt
│   │   │   ├── main.cpp
│   │   │   ├── GUI.cpp, GameSession.cpp, Game_board.cpp,
│   │   │   │   MapEditor.cpp, Robot.cpp, Robot_factory.cpp
│   │   │   ├── include/                     # Direction.h, GUI.h, GameSession.h, Game_board.h,
│   │   │   │                                # MapEditor.h, Robot.h, Robot_factory.h
│   │   │   └── fonts/Samson.ttf
│   │   └── vendor/                          # сторонние библиотеки (исходники)
│   │       ├── CMakeLists.txt
│   │       ├── SDL/                         # SDL3
│   │       └── SDL_ttf/                     # SDL3_ttf
│   │
│   └── lab 2/                               # развитие игры: способности, статусы, предметы, ИИ врагов
│       ├── CMakeLists.txt
│       ├── Отчет ЛР2.docx
│       └── src/
│           ├── CMakeLists.txt
│           ├── main.cpp
│           ├── abilities/                   # Ability_catalog, Around_attack, Heal,
│           │                                # Long_attack, Teleport, Robot_abilities
│           ├── board/                       # Game_board, Cell_status_generator, Direction
│           ├── game/                        # Game_session, Game_template, Demo_level,
│           │                                # Enemy_controller, Event_log, Map_editor, Level_result
│           ├── gui/                         # Gui, Canvas, Layout, Palette, Assets, Sprite_catalog,
│           │                                # Board_view, Hud_view, Dialog_view,
│           │                                # Display_mode(_selector), Player_controller
│           ├── items/                       # Item, Inventory, Attack/Energy/Health_potion
│           ├── robot/                       # Robot, Robot_factory, Relation
│           ├── statuses/                    # Base_status, Status_list, Burn, Shield, Slow,
│           │                                # Strength, Overload, Item_pickup, Robot_status
│           ├── fonts/Samson.ttf
│           └── assets/Tiny Swords (Free Pack)/  # спрайты: Buildings, Particle FX,
│                                                # Terrain, UI Elements, Units
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
│       ├── lab 1.docx
│       ├── lab 4.docx
│       ├── lab 9.docx
│       └── lab 12.docx
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
    │           └── include/                 # one.h, two.h, three.h, four.h,
    │                                        # pred_obrabotka.h, slon_tools.h
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

### 📄 Описание содержимого

- **Algorithms (Алгоритмы и структуры данных)** — реализация структур данных и алгоритмов на C++ с тестами, бенчмарками и графиками
- **Computer Science (Информатика)** — лабораторные работы на Python (NumPy, Pillow, ООП, scikit-learn)
- **OOP (Объектно-ориентированное программирование)** — пошаговая игра с роботами на C++20 с графикой на SDL3
- **ORGEVM (Организация ЭВМ)** — лабораторные работы на ассемблере
- **Physics (Физика)** — отчёты по лабораторным работам
- **Programming (Программирование)** — лабораторные и курсовые работы на C
- **Job_condition.md** — условие задания лабораторной работы
- **src/** — исходный код решения
- **\*.docx / \*.pdf / \*.ipynb** — отчёты по работам
- **vendor/** — исходники сторонних библиотек (SDL3, SDL3_ttf), не авторский код

### 🔨 Сборка

- Проекты на C/C++ с `CMakeLists.txt` (Algorithms, OOP):
  ```
  cmake -S . -B build
  cmake --build build
  ```
- Проекты с `Makefile` (Programming): `make` в папке `src/`
- Python-работы: `python3 main.py`

---

## 🇬🇧 Description (English)

This repository contains academic works completed during studies at  
**Saint Petersburg Electrotechnical University “LETI” (SPbETU LETI)**.  
Faculty of **Computer Technologies and Informatics (CTI)**.  
Program **Applied Mathematics and Informatics**.

The repository is intended for storing and organizing laboratory, practical and course works for various university courses.

### 📁 Repository Structure

The full file tree is shown in the Russian section above. Top-level overview:

```
university-works/
├── Algorithms/          # lab 1 (circular deque), lab 2 (Timsort) — C++, CMake, GoogleTest
├── Computer Science/    # 1 Semester (labs 1–3), 2 Semester (labs 1–3) — Python
├── OOP/                 # lab 1, lab 2 — robot game in C++20 with SDL3
├── ORGEVM/              # lab 1 — assembly
├── Physics/             # 2 sem — lab reports (.docx)
└── Programming/         # Semester 1 (labs 1–4, coursework), Semester 2 (labs 1–4, coursework) — C
```

### 📄 Content Description

- **Algorithms** — data structures and algorithms in C++ with tests, benchmarks and plots
- **Computer Science** — Python labs (NumPy, Pillow, OOP, scikit-learn)
- **OOP** — turn-based robot game in C++20 using SDL3
- **ORGEVM** — computer organization labs in assembly
- **Physics** — physics lab reports
- **Programming** — C labs and course works
- **Job_condition.md** — task description and requirements
- **src/** — source code of the solution
- **\*.docx / \*.pdf / \*.ipynb** — reports
- **vendor/** — third-party library sources (SDL3, SDL3_ttf), not authored code

---

## 📌 Notes

All materials in this repository were created for educational purposes as part of the university curriculum.
