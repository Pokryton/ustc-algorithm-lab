#!/usr/bin/env python3

import sys
import time
from math import hypot
from typing import List


Point = tuple[int, float, float]
Pair = tuple[Point, Point]


def read(line: str) -> Point:
    s = line.split()
    return int(s[0]), float(s[1]), float(s[2])


def dist(p1: Point, p2: Point) -> float:
    dx = p1[1] - p2[1]
    dy = p1[2] - p2[2]
    return hypot(dx, dy)


def brute_force(ps: List[Point]) -> tuple[float, Pair | None]:
    n = len(ps)
    d, pair = float("inf"), None
    for i in range(n - 1):
        for j in range(i + 1, n):
            if dist(ps[i], ps[j]) < d:
                d = dist(ps[i], ps[j])
                pair = ps[i], ps[j]

    return d, pair


def solve(xs: List[Point], ys: List[Point]) -> tuple[float, Pair | None]:
    n = len(xs)
    if n <= 1:
        return float("inf"), None
    if n == 2:
        return dist(*xs), tuple(xs)

    mid = n // 2
    xm = xs[mid - 1][1]

    ansl = solve(xs[:mid], [p for p in ys if p[1] <= xm])
    ansr = solve(xs[mid:], [p for p in ys if p[1] > xm])
    d, pair = min(ansl, ansr)

    ym = [p for p in ys if abs(p[1] - xm) < d]
    m = len(ym)
    for i in range(m - 1):
        for j in range(i + 1, min(i + 8, m)):
            if dist(ym[i], ym[j]) < d:
                d = dist(ym[i], ym[j])
                pair = ym[i], ym[j]

    return d, pair


def closest_pair(ps: List[Point]) -> tuple[float, Pair | None]:
    xs = sorted(ps, key=lambda p: p[1])
    ys = sorted(ps, key=lambda p: p[2])

    return solve(xs, ys)


if __name__ == "__main__":
    with open("data.txt") as f:
        data = [read(line) for line in f.readlines()]

        start = time.time()

        if len(sys.argv) < 2:
            d, pair = closest_pair(data)
        elif len(sys.argv) == 2 and sys.argv[1] == "brute_force":
            d, pair = brute_force(data)
        else:
            print("Usage: ./closest_pair.py [brute_force]")
            sys.exit(1)

        end = time.time()

        print(f"Algorithm finishes in {end - start}s.")
        print("Solution:")

        assert pair is not None
        print(*pair[0])
        print(*pair[1])
        print(f"d = {d}")
