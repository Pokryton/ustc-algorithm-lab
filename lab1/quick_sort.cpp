#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <random>
#include <algorithm>
#include <chrono>

template <typename It>
void insertion_sort(It first, It last) {
    for (It i = std::next(first); i != last; ++i) {
        It j = i;
        while (j != first && *i < *std::prev(j)) {
            --j;
        }
        std::rotate(j, i, std::next(i));
    }
}

template <typename It, typename T>
std::pair<It, It> partition3(It first, It last, T pivot) {
    for (It middle = first; middle != last; ) {
        if (*middle < pivot) {
            std::iter_swap(middle++, first++);
        } else if (pivot < *middle) {
            std::iter_swap(middle, --last);
        } else {
            ++middle;
        }
    }

    return {first, last};
}

template <typename It>
inline static auto pivot_select(It first, [[maybe_unused]] It last) {
#ifdef SELECT3
    auto middle = std::next(first, std::distance(first, last) / 2);
    return std::max(std::min(*first, *middle),
                    std::min(std::max(*first, *middle), *std::prev(last)));
#else
    return *first;
#endif
}

template <typename It>
void quick_almost_sort(It first, It last) {
    if (std::distance(first, last) <= 16) {
        return;
    }

    auto pivot = pivot_select(first, last);
    auto [left, right] = partition3(first, last, pivot);
    quick_almost_sort(first, left);
    quick_almost_sort(right, last);
}

template <typename It>
void quick_sort(It first, It last) {
#ifdef RAND
    static std::random_device rd;
    static std::mt19937 g(rd());

    std::shuffle(first, last, g);
#endif

    quick_almost_sort(first, last);
    insertion_sort(first, last);
}

int main() {
    std::ifstream fin("data.txt");
    std::ofstream fout("sorted.txt");

    if (!fin || !fout) {
        return 1;
    }

    size_t len;
    std::vector<int> data;

    fin >> len;
    data.reserve(len);
    std::copy(std::istream_iterator<int>(fin),
              std::istream_iterator<int>(),
              std::back_inserter(data));

    auto t1 = std::chrono::high_resolution_clock::now();

#if defined(USE_STD_SORT)
    std::sort(data.begin(), data.end());
#elif defined(USE_STD_QSORT)
    std::qsort(data.data(), data.size(), sizeof(int),
               [](const void* a, const void* b) -> int {
                   return *static_cast<const int*>(a) - *static_cast<const int*>(b);
               });
#else
    quick_sort(data.begin(), data.end());
#endif

    auto t2 = std::chrono::high_resolution_clock::now();

    std::cerr << "sort an array of " << data.size() << " ints in "
              << std::chrono::duration<double>(t2-t1).count()
              << "s\n";

    std::copy(data.begin(), data.end(),
              std::ostream_iterator<int>(fout, " "));

    return 0;
}
