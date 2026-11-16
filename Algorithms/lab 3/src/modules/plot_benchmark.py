"""Create the two figures used in the Rope laboratory report."""

import csv
import math
import os
import tempfile
from pathlib import Path

os.environ.setdefault("MPLCONFIGDIR", str(Path(tempfile.gettempdir()) / "mpl-lab3-rope"))

import matplotlib.pyplot as plt


ROOT = Path(__file__).resolve().parent
FIGURES = ROOT / "figures"
FIGURES.mkdir(exist_ok=True)

with (ROOT / "results.csv").open(newline="", encoding="utf-8") as stream:
    records = list(csv.DictReader(stream))

operations = {}
for record in records:
    record["n"] = int(record["n"])
    record["rope_ns_per_op"] = float(record["rope_ns_per_op"])
    record["string_ns_per_op"] = float(record["string_ns_per_op"])
    operations.setdefault(record["operation"], []).append(record)

titles = {
    "build_and_destroy": "Построение и освобождение",
    "random_index": "Случайный индекс",
    "edit_pair": "Вставка + удаление",
    "edit_pair_with_output": "Редактирование с выводом текста",
}
sizes = sorted({r["n"] for r in records})

plt.rcParams.update({
    "font.family": "DejaVu Sans",
    "font.size": 8.5,
    "axes.titlesize": 9,
    "axes.labelsize": 8.5,
    "legend.fontsize": 7.5,
})

fig, axes = plt.subplots(2, 2, figsize=(10.8, 7.0), sharex=True)
for ax, operation in zip(axes.flat, titles):
    data = sorted(operations[operation], key=lambda row: row["n"])
    x = [row["n"] for row in data]
    ax.plot(x, [row["rope_ns_per_op"] for row in data], marker="o",
            linewidth=1.6, color="#2463a6", label="Rope")
    ax.plot(x, [row["string_ns_per_op"] for row in data], marker="s",
            linewidth=1.6, color="#d26a28", label="std::string")
    ax.set_title(titles[operation], pad=7)
    ax.set_xscale("log", base=2)
    ax.set_yscale("log")
    ax.set_xticks(sizes, [f"{n:,}".replace(",", " ") for n in sizes])
    ax.grid(True, which="both", color="#d9dfe7", linewidth=0.65)
    ax.legend(frameon=False, loc="best")
for ax in axes[-1, :]:
    ax.set_xlabel("Длина текста, байт")
for ax in axes[:, 0]:
    ax.set_ylabel("Время на сценарий, нс")
fig.suptitle("Сравнение замеров Rope и std::string", fontsize=12, y=0.99)
fig.tight_layout(rect=(0.02, 0.02, 0.99, 0.95))
fig.savefig(FIGURES / "figure1_operation_comparison.png", dpi=240,
            bbox_inches="tight", facecolor="white")
plt.close(fig)

build = sorted(operations["build_and_destroy"], key=lambda row: row["n"])
index = sorted(operations["random_index"], key=lambda row: row["n"])
x = [row["n"] for row in build]
n0 = x[0]
build_measured = [row["rope_ns_per_op"] / build[0]["rope_ns_per_op"] for row in build]
index_measured = [row["rope_ns_per_op"] / index[0]["rope_ns_per_op"] for row in index]
linear_reference = [n / n0 for n in x]
log_reference = [math.log2(n) / math.log2(n0) for n in x]

fig, ax = plt.subplots(figsize=(8.2, 4.8))
ax.plot(x, build_measured, marker="o", linewidth=1.8, color="#2463a6",
        label="Rope: построение и освобождение")
ax.plot(x, linear_reference, linestyle="--", linewidth=1.7, color="#2463a6",
        label="Ориентир O(n)")
ax.plot(x, index_measured, marker="s", linewidth=1.8, color="#d26a28",
        label="Rope: случайный index")
ax.plot(x, log_reference, linestyle="--", linewidth=1.7, color="#d26a28",
        label="Ориентир O(log n)")
ax.set_xscale("log", base=2)
ax.set_yscale("log")
ax.set_xticks(sizes, [f"{n:,}".replace(",", " ") for n in sizes])
ax.set_xlabel("Длина текста, байт")
ax.set_ylabel("Время относительно замера при n = 256")
ax.set_title("Практические замеры и теоретические ориентиры")
ax.grid(True, which="both", color="#d9dfe7", linewidth=0.65)
ax.legend(frameon=False, ncol=2, loc="upper left")
fig.tight_layout()
fig.savefig(FIGURES / "figure2_scaling.png", dpi=240,
            bbox_inches="tight", facecolor="white")
plt.close(fig)

print(FIGURES / "figure1_operation_comparison.png")
print(FIGURES / "figure2_scaling.png")
