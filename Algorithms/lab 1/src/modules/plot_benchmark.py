import csv
import os
from collections import defaultdict

import numpy as np
import matplotlib.pyplot as plt

ROOT_DIR = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", ".."))
CSV_PATH = os.path.join(ROOT_DIR, "benchmark.csv")
OUT_DIR = os.path.join(ROOT_DIR, "plots")

BLUE = "#2563eb"
RED = "#dc2626"

plt.rcParams.update({
    "figure.facecolor": "white",
    "axes.facecolor": "white",
    "font.size": 11,
})

THEORY = {
    "push_back": ("O(1)", lambda n: np.ones_like(n, dtype=float)),
    "push_front": ("O(1)", lambda n: np.ones_like(n, dtype=float)),
    "pop_back": ("O(1)", lambda n: np.ones_like(n, dtype=float)),
    "pop_front": ("O(1)", lambda n: np.ones_like(n, dtype=float)),
    "front": ("O(1)", lambda n: np.ones_like(n, dtype=float)),
    "back": ("O(1)", lambda n: np.ones_like(n, dtype=float)),
    "resize": ("O(n)", lambda n: n.astype(float)),
}
def load_data(path):
    data = defaultdict(lambda: ([], []))
    with open(path, newline="") as f:
        reader = csv.DictReader(f)
        for row in reader:
            op = row["operation"]
            data[op][0].append(int(row["N"]))
            data[op][1].append(float(row["ns_per_op"]))
    return data

def fit_scale(theory_vals, measured):
    """Подбор коэффициента c: minimize ||c * theory - measured||."""
    theory_vals = np.asarray(theory_vals, dtype=float)
    measured = np.asarray(measured, dtype=float)
    denom = float(np.dot(theory_vals, theory_vals))
    if denom == 0:
        return 0.0
    return float(np.dot(theory_vals, measured)) / denom
def main():
    if not os.path.exists(CSV_PATH):
        raise SystemExit(f"Не найден {CSV_PATH}. Сначала запустите ./benchmark > {CSV_PATH}")

    os.makedirs(OUT_DIR, exist_ok=True)
    data = load_data(CSV_PATH)

    for op, (ns, ys) in data.items():
        order = np.argsort(ns)
        n = np.array(ns)[order]
        y = np.array(ys)[order]

        label, model = THEORY.get(op, ("?", lambda x: np.ones_like(x, dtype=float)))
        theory = model(n)
        c = fit_scale(theory, y)
        theory_scaled = c * theory

        fig, ax = plt.subplots(figsize=(8, 5))

        if op == "resize":
            ax.plot(n, y, "o-", color=BLUE, markersize=6, linewidth=2,
                    label="Эксперимент (замер)", zorder=3)
            ax.plot(n, theory_scaled, "--", color=RED, linewidth=2,
                    label=f"Теория {label}", zorder=2)
            ax.set_xscale("log")
            ax.set_yscale("log")
            ax.set_ylabel("Время одного resize, нс")
        else:
            ax.plot(n, y, "o-", color=BLUE, markersize=6, linewidth=2,
                    label="Эксперимент (замер)", zorder=3)
            mean_y = float(np.mean(y))
            ax.axhline(mean_y, color=RED, linestyle="--", linewidth=2,
                       label=f"Теория {label} ≈ {mean_y:.2f} нс", zorder=2)
            ax.set_ylim(bottom=0, top=max(y.max(), mean_y) * 1.35)
            ax.set_xlabel("Размер N (элементов)")
            ax.set_ylabel("Время на операцию, нс")

        ax.set_xlabel("Размер N (элементов)")
        ax.set_title(f"CircularDeque::{op} — сравнение с теорией {label}",
                     fontsize=13, fontweight="bold", pad=12)
        ax.grid(True, which="both", alpha=0.25, linestyle="-")
        ax.legend(frameon=True, fontsize=10, loc="best")
        ax.spines["top"].set_visible(False)
        ax.spines["right"].set_visible(False)
        ax.margins(x=0.02)

        fig.tight_layout()
        out = os.path.join(OUT_DIR, f"{op}.png")
        fig.savefig(out, dpi=130)
        plt.close(fig)
        print(f"Сохранён график: {out}")

    print(f"\nГотово. Графики в каталоге {OUT_DIR}")


if __name__ == "__main__":
    main()