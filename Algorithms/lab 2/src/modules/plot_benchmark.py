import csv
import math
import os

import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np

BASE_DIR = os.path.dirname(os.path.abspath(__file__))
CSV_PATH = os.path.join(BASE_DIR, "results.csv")
OUT_DIR = os.path.join(BASE_DIR, "plots")

BLUE = "#2563eb"
RED = "#dc2626"
GREEN = "#16a34a"
ORANGE = "#ea580c"

CASE_NAMES = {
    "best": "лучший случай",
    "average": "средний случай",
    "worst": "худший случай",
    "partial": "частично упорядоченный массив",
}


def load_data(path):
    rows = []
    with open(path, newline="") as f:
        for row in csv.DictReader(f):
            row["n"] = int(row["n"])
            row["min_run"] = int(row["min_run"])
            row["gallop"] = int(row["gallop"])
            row["time_ns"] = float(row["time_ns"])
            row["comparisons"] = int(row["comparisons"])
            rows.append(row)
    return rows


def fit_scale(theory_vals, measured):
    theory_vals = np.asarray(theory_vals, dtype=float)
    measured = np.asarray(measured, dtype=float)
    denom = float(np.dot(theory_vals, theory_vals))
    if denom == 0:
        return 0.0
    return float(np.dot(theory_vals, measured)) / denom


def style_axes(ax, title):
    ax.set_title(title, fontsize=13, fontweight="bold", pad=12)
    ax.grid(True, which="both", alpha=0.25, linestyle="-")
    ax.legend(frameon=True, fontsize=10, loc="best")
    ax.spines["top"].set_visible(False)
    ax.spines["right"].set_visible(False)


def save(fig, name):
    fig.tight_layout()
    out = os.path.join(OUT_DIR, name)
    fig.savefig(out, dpi=130)
    plt.close(fig)
    print(f"Сохранён график: {out}")


def plot_case(rows, case, label, model, filename):
    data = sorted((r for r in rows if r["experiment"] == "size" and r["case"] == case), key=lambda r: r["n"])
    n = np.array([r["n"] for r in data], dtype=float)
    t = np.array([r["time_ns"] for r in data]) / 1000.0
    theory = model(n)
    c = fit_scale(theory, t)
    fig, ax = plt.subplots(figsize=(8, 5))
    ax.plot(n, t, "o-", color=BLUE, markersize=6, linewidth=2, label="Эксперимент (замер)", zorder=3)
    ax.plot(n, c * theory, "--", color=RED, linewidth=2, label=f"Теория {label}", zorder=2)
    ax.set_xscale("log")
    ax.set_yscale("log")
    ax.set_xlabel("Размер массива n (элементов)")
    ax.set_ylabel("Время сортировки, мкс")
    style_axes(ax, f"TimSort, {CASE_NAMES[case]} — сравнение с теорией {label}")
    save(fig, filename)


def plot_comparisons(rows):
    fig, ax = plt.subplots(figsize=(8, 5))
    models = {
        "best": (GREEN, lambda n, mr: (2 * n - 1) / n, "2n − 1"),
        "average": (BLUE, lambda n, mr: mr / 4 + np.log2(np.maximum(n / mr, 1)), "n·min_run/4 + n·log₂(n/min_run)"),
        "worst": (RED, lambda n, mr: mr / 2 + np.log2(np.maximum(n / mr, 1)), "n·min_run/2 + n·log₂(n/min_run)"),
    }
    for case, (color, model, label) in models.items():
        data = sorted((r for r in rows if r["experiment"] == "size" and r["case"] == case), key=lambda r: r["n"])
        n = np.array([r["n"] for r in data], dtype=float)
        mr = np.array([r["min_run"] for r in data], dtype=float)
        c = np.array([r["comparisons"] for r in data], dtype=float)
        ax.plot(n, c / n, "o-", color=color, markersize=5, linewidth=2, label=f"{CASE_NAMES[case].capitalize()} (замер)")
        ax.plot(n, model(n, mr), "--", color=color, linewidth=1.5, alpha=0.8, label=f"Теория {label}")
    ax.set_xscale("log")
    ax.set_xlabel("Размер массива n (элементов)")
    ax.set_ylabel("Сравнений на один элемент")
    style_axes(ax, "TimSort — число сравнений на элемент")
    ax.legend(frameon=True, fontsize=8.5, loc="upper left")
    save(fig, "comparisons.png")


def plot_minrun(rows, n_value, filename):
    data = sorted((r for r in rows if r["experiment"] == "minrun" and r["n"] == n_value), key=lambda r: r["min_run"])
    mr = np.array([r["min_run"] for r in data], dtype=float)
    t = np.array([r["time_ns"] for r in data]) / 1000.0
    best = data[int(np.argmin(t))]
    fig, ax = plt.subplots(figsize=(8, 5))
    ax.plot(mr, t, "o-", color=BLUE, markersize=6, linewidth=2, label="Эксперимент (замер)", zorder=3)
    standard = next(r for r in rows if r["experiment"] == "size" and r["case"] == "average" and r["n"] == n_value)["min_run"]
    ax.axvline(standard, color=RED, linestyle="--", linewidth=2, label=f"Стандартный min_run = {standard}", zorder=2)
    ax.plot([best["min_run"]], [best["time_ns"] / 1000.0], "o", color=ORANGE, markersize=11, zorder=4,
            label=f"Минимум: min_run = {best['min_run']}")
    ax.set_xscale("log", base=2)
    ax.set_xlabel("min_run (логарифмическая шкала)")
    ax.set_ylabel("Время сортировки, мкс")
    style_axes(ax, f"TimSort, n = {n_value} — зависимость времени от min_run")
    save(fig, filename)


def plot_minrun_comparisons(rows, n_value, filename):
    data = sorted((r for r in rows if r["experiment"] == "minrun" and r["n"] == n_value), key=lambda r: r["min_run"])
    mr = np.array([r["min_run"] for r in data], dtype=float)
    c = np.array([r["comparisons"] for r in data], dtype=float)
    model = n_value * mr / 4 + n_value * np.log2(np.maximum(n_value / mr, 1))
    fig, ax = plt.subplots(figsize=(8, 5))
    ax.plot(mr, c, "o-", color=BLUE, markersize=6, linewidth=2, label="Эксперимент (замер)", zorder=3)
    ax.plot(mr, model, "--", color=RED, linewidth=2, label="Модель n·min_run/4 + n·log₂(n/min_run)", zorder=2)
    ax.set_xscale("log", base=2)
    ax.set_yscale("log")
    ax.set_xlabel("min_run (логарифмическая шкала)")
    ax.set_ylabel("Число сравнений")
    style_axes(ax, f"TimSort, n = {n_value} — число сравнений от min_run")
    save(fig, filename)


def main():
    if not os.path.exists(CSV_PATH):
        raise SystemExit(f"Не найден {CSV_PATH}. Сначала запустите ./benchmark > {CSV_PATH}")
    os.makedirs(OUT_DIR, exist_ok=True)
    rows = load_data(CSV_PATH)
    plot_case(rows, "best", "O(n)", lambda n: n, "best.png")
    plot_case(rows, "average", "O(n log n)", lambda n: n * np.log2(n), "average.png")
    plot_case(rows, "worst", "O(n log n)", lambda n: n * np.log2(n), "worst.png")
    plot_comparisons(rows)
    plot_minrun(rows, 100000, "minrun_100000.png")
    plot_minrun(rows, 1000, "minrun_1000.png")
    plot_minrun_comparisons(rows, 100000, "minrun_comparisons.png")
    print(f"\nГотово. Графики в каталоге {OUT_DIR}")


if __name__ == "__main__":
    main()
