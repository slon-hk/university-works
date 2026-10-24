"""Строит полную UML-диаграмму классов (PlantUML) по заголовкам lab 3."""
import os
import re
import sys

SRC = sys.argv[1]
OUT = sys.argv[2]

PACKAGE_COLORS = {
    'program': '#FDECC8', 'game': '#DCEBFA', 'gui': '#E4F5E1', 'input': '#F9E1EC',
    'save': '#EDE4FA', 'logging': '#FFF4CC', 'robot': '#FBE3D6', 'board': '#E0F2F1',
    'abilities': '#F3E5F5', 'statuses': '#FFEBEE', 'items': '#E8EAF6',
}
PACKAGE_TITLES = {
    'program': 'program — запуск', 'game': 'game — приложение и логика уровней',
    'gui': 'gui — отрисовка (SDL3)', 'input': 'input — ввод (SDL3)', 'save': 'save — сохранение',
    'logging': 'logging — журнал', 'robot': 'robot — роботы', 'board': 'board — игровое поле',
    'abilities': 'abilities — способности', 'statuses': 'statuses — статусы', 'items': 'items — предметы',
}


def strip_comments(text):
    text = re.sub(r'/\*.*?\*/', '', text, flags=re.S)
    text = re.sub(r'//[^\n]*', '', text)
    return text


def find_block_end(text, start):
    """start указывает на '{'; возвращает индекс парной '}'."""
    depth = 0
    for i in range(start, len(text)):
        if text[i] == '{':
            depth += 1
        elif text[i] == '}':
            depth -= 1
            if depth == 0:
                return i
    return len(text) - 1


def clean_type(t):
    t = re.sub(r'\bstd::', '', t)
    t = re.sub(r'\s+', ' ', t).strip()
    t = t.replace(' &', '&').replace(' *', '*')
    return t


def clean_params(p):
    p = re.sub(r'=\s*[^,()]+', '', p)  # значения по умолчанию
    return clean_type(p)


class Cls:
    def __init__(self, name, kind, package):
        self.name, self.kind, self.package = name, kind, package
        self.template = None
        self.bases = []
        self.fields = []   # (vis, type, name, static)
        self.methods = []  # (vis, text, abstract, static)
        self.values = []
        self.pure = False


def parse_body(cls, body):
    vis = '-' if cls.kind == 'class' else '+'
    i = 0
    stmt = ''
    while i < len(body):
        c = body[i]
        if c == '{':
            end = find_block_end(body, i)
            handle_stmt(cls, stmt, vis, has_body=True)
            stmt = ''
            i = end + 1
            # после тела метода может стоять ';'
            continue
        if c == ';':
            handle_stmt(cls, stmt, vis, has_body=False)
            stmt = ''
            i += 1
            continue
        stmt += c
        m = re.search(r'\b(public|private|protected)\s*:\s*$', stmt)
        if m and not stmt.rstrip().endswith('::'):
            vis = {'public': '+', 'private': '-', 'protected': '#'}[m.group(1)]
            stmt = ''
        i += 1


def handle_stmt(cls, stmt, vis, has_body):
    s = re.sub(r'\s+', ' ', stmt).strip()
    if not s or s.startswith(('using ', 'friend ', 'static_assert', 'typedef ')):
        return
    if s.startswith(':') or s.startswith('='):
        return
    if re.search(r'=\s*delete\s*$', s):
        return
    static = bool(re.match(r'(static|static constexpr|static const)\b', s))
    if '(' in s and ('operator' in s or not re.match(r'^[\w:<>,\s*&]+\s+\w+\s*=', s)):
        # метод: отрезаем список инициализации конструктора
        s = re.sub(r'\)\s*:\s*[^()]*\(.*$', ')', s) if has_body else s
        abstract = bool(re.search(r'=\s*0\s*$', s))
        if abstract:
            cls.pure = True
        s = re.sub(r'\s*=\s*(0|default|delete)\s*$', '', s)
        s = re.sub(r'\b(virtual|explicit|inline|static|constexpr)\s+', '', s)
        s = re.sub(r'\s+(override|final|noexcept)\b', '', s)
        m = re.match(r'^(.*?)\b(~?\w+|operator\s*\S+)\s*\((.*)\)\s*(const)?\s*$', s)
        if not m:
            return
        ret, name, params, const = m.groups()
        ret = clean_type(ret)
        params = clean_params(params)
        text = f'{name}({params})'
        if const:
            text += ' const'
        if ret and name not in (cls.name, '~' + cls.name):
            text += f' : {ret}'
        cls.methods.append((vis, text, abstract, static))
        return
    # поле
    s = re.sub(r'\s*=.*$', '', s)
    s = re.sub(r'\{.*$', '', s)
    s = re.sub(r'^(static constexpr|static const|static|mutable)\s+', '', s)
    m = re.match(r'^(.*[\s*&>])(\w+)(\[[^\]]*\])?$', s)
    if not m:
        return
    t, name, arr = m.groups()
    t = clean_type(t) + (arr or '')
    cls.fields.append((vis, t, name, static))


def parse_file(path, package, classes):
    text = strip_comments(open(path, encoding='utf-8').read())
    for m in re.finditer(r'(template\s*<([^>]*)>\s*)?\b(class|struct|enum class)\s+(\w+)\s*([^;{]*)\{', text):
        template, tparams, kind, name, rest = m.groups()
        start = m.end() - 1
        end = find_block_end(text, start)
        body = text[start + 1:end]
        cls = classes.get(name) or Cls(name, 'enum' if kind == 'enum class' else kind, package)
        cls.package = package
        if tparams:
            cls.template = ', '.join(p.strip().split()[-1] for p in tparams.split(','))
        if kind == 'enum class':
            cls.values = [v.strip().split('=')[0].strip() for v in body.split(',') if v.strip()]
        else:
            bm = re.match(r'\s*(?:final\s*)?:\s*(.*)', rest)
            if bm:
                cls.bases = [b.strip().split()[-1] for b in bm.group(1).split(',')]
            parse_body(cls, body)
        classes[name] = cls


classes = {}
for root, _, files in os.walk(SRC):
    for f in sorted(files):
        if f.endswith('.h'):
            package = os.path.relpath(root, SRC).split(os.sep)[0]
            parse_file(os.path.join(root, f), package, classes)

names = set(classes)


def refs(t):
    return [w for w in re.findall(r'\b\w+\b', t) if w in names]


lines = ['@startuml', '!pragma layout smetana', 'skinparam dpi 110', 'skinparam shadowing false',
         'skinparam classAttributeIconSize 0', 'skinparam packageStyle rectangle',
         'skinparam defaultFontName Helvetica', 'skinparam classFontSize 13', 'skinparam classAttributeFontSize 11',
         'skinparam ArrowColor #555555', 'skinparam classBorderColor #666666', 'skinparam classBackgroundColor #FFFFFF',
         'hide empty members', 'left to right direction',
         'title <size:28>Lab 3 — полная диаграмма классов</size>\\n<size:14>сгенерирована по заголовкам src/**/includes/*.h</size>', '']


def decl_name(c):
    return f'{c.name}<{c.template}>' if c.template else c.name


for package in PACKAGE_COLORS:
    members = [c for c in classes.values() if c.package == package]
    if not members:
        continue
    lines.append(f'package "{PACKAGE_TITLES[package]}" as pkg_{package} {PACKAGE_COLORS[package]} {{')
    for c in sorted(members, key=lambda c: c.name):
        if c.kind == 'enum':
            lines.append(f'  enum {c.name} {{')
            lines += [f'    {v}' for v in c.values]
            lines.append('  }')
            continue
        is_interface = c.pure and not c.fields and all(ab for _, text, ab, _ in c.methods if not text.startswith('~'))
        kw = 'interface' if is_interface else ('abstract class' if c.pure else 'class')
        lines.append(f'  {kw} {decl_name(c)} {{')
        for vis, t, n, st in c.fields:
            lines.append(f'    {vis}{"{static} " if st else ""}{n} : {t}')
        for vis, text, ab, st in c.methods:
            mods = ('{abstract} ' if ab else '') + ('{static} ' if st else '')
            lines.append(f'    {vis}{mods}{text}')
        lines.append('  }')
    lines.append('}')
    lines.append('')

edges = set()
for c in classes.values():
    for b in c.bases:
        if b in names:
            arrow = '..|>' if classes[b].pure and not classes[b].fields else '--|>'
            edges.add(f'{c.name} {arrow} {b}')
    for vis, t, n, st in c.fields:
        for r in refs(t):
            if r == c.name:
                continue
            if classes[r].kind == 'enum':
                edges.add(f'{c.name} --> {r}')
                continue
            many = re.search(r'\b(vector|map|list|array|deque)\b', t) or '[' in t
            shared = re.search(r'\b(shared_ptr|weak_ptr)\b', t) or t.endswith('&') or t.endswith('*')
            mult = ' "*"' if many else ''
            edges.add(f'{c.name} {"o--" if shared else "*--"}{mult} {r}')

# связи шаблонов, которые не видны по полям
edges.add('Game_mode_catalog ..> Game_template : создаёт')
edges.add('Game_template ..> File_level_generator : Level_generator')
edges.add('Game_template ..> Random_level_generator : Level_generator')
edges.add('Game_view ..> Sdl_renderer : Renderer')
edges.add('Game_controller ..> Sdl_input : Input')
edges.add('File_logger ..> Single_file_output : Output')
edges.add('File_logger ..> Split_file_output : Output')
edges.add('Program ..> Application : создаёт')
edges.add('Program ..> Game_view : создаёт')
edges.add('Program ..> Game_controller : создаёт')
edges.add('Game_view ..> Application : читает')
edges.add('Game_controller ..> Application : handle()')
edges.add('Save_slot ..> Game_serializer')

lines += sorted(edges)
lines.append('@enduml')
open(OUT, 'w', encoding='utf-8').write('\n'.join(lines) + '\n')

n_cls = sum(1 for c in classes.values() if c.kind != 'enum')
n_enum = sum(1 for c in classes.values() if c.kind == 'enum')
print(f'classes={n_cls} enums={n_enum} edges={len(edges)} '
      f'fields={sum(len(c.fields) for c in classes.values())} methods={sum(len(c.methods) for c in classes.values())}')
