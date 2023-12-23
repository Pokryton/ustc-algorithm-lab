#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <chrono>
#include <span>
#include <ranges>

struct Point {
    int id;
    float x;
    float y;

    Point()
        : id(-1), x(0.0f), y(0.0f) {}

    Point(int id, float x, float y)
        : id(id), x(x), y(y) {}
};

float dist(const Point& p1, const Point& p2)
{
    float dx = p1.x - p2.x;
    float dy = p1.y - p2.y;
    return std::hypot(dx, dy);
}

static std::pair<float, std::pair<Point, Point>> brute_force(std::span<Point> points)
{
    float d = std::numeric_limits<float>::infinity();
    size_t p1, p2;
    for (size_t i = 0; i < points.size() - 1; ++i) {
        for (size_t j = i + 1; j < points.size(); ++j) {
            if (dist(points[i], points[j]) < d) {
                d = dist(points[i], points[j]);
                p1 = i;
                p2 = j;
            }
        }
    }
    return {d, {points[p1], points[p2]}};
}

static std::pair<float, std::pair<Point, Point>> solve(std::span<Point> points)
{
    namespace ranges = std::ranges;

    size_t n = points.size();
    if (n < 32) {
        ranges::sort(points, {}, &Point::y);
        return brute_force(points);
    }

    const auto ansl = solve(points.first(n / 2));
    const auto ansr = solve(points.subspan(n / 2));
    auto [d, pair] = ansl.first < ansr.first ? ansl : ansr;

    const auto middle = ranges::next(points.begin(), n / 2);
    ranges::inplace_merge(points, middle, {}, &Point::y);

    const float middle_x = (middle - 1)->x;
    std::vector<Point> middle_points;
    ranges::copy_if(
        points,
        std::back_inserter(middle_points),
        [middle_x, d](float x) { return std::abs(x - middle_x) < d; },
        &Point::x);

    size_t m = middle_points.size();
    for (size_t i = 0; i < m - 1; ++i) {
        for (size_t j = i + 1; j < std::min(i + 8, m); ++j) {
            if (dist(points[i], points[j]) < d) {
                d = dist(points[i], points[j]);
                pair = {points[i], points[j]};
            }
        }
    }
    return {d, pair};
}

std::pair<float, std::pair<Point, Point>> closest_pair(std::span<Point> points)
{
    std::ranges::sort(points, {}, &Point::x);
    return solve(points);
}

int main()
{
    std::ifstream fin("data.txt");
    if (!fin) {
        std::cerr << "Cannot load data" << std::endl;
        return 1;
    }

    int id;
    float x, y;
    std::vector<Point> points;

    while (fin >> id >> x >> y) {
        points.emplace_back(id, x, y);
    }

    auto start = std::chrono::high_resolution_clock::now();

    auto [d, pair] = closest_pair(points);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;
    std::cout << "Algorithm finishes in " << diff << '\n';

    std::cout << "Solution:\n";
    std::cout << pair.first.id << " " << pair.first.x << " " << pair.first.y << '\n';
    std::cout << pair.second.id << " " << pair.second.x << " " << pair.second.y << '\n';
    std::cout << "d = " << d << '\n';
    return 0;
}
