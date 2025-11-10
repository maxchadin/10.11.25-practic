#include <iostream>

struct IntArray {
    void add(int i);
    int at(size_t id) const noexcept;
    int get(size_t id) const noexcept;
    size_t size() const noexcept;
    int last() const noexcept;
    IntArray(int i);
    ~IntArray();

    int* a;
    size_t k;
};

IntArray::~IntArray() {
    delete[] a;
}

IntArray::IntArray(int i)
    : a(new int[1]),
    k(1)
{
    *a = i;
}

int IntArray::get(size_t id) const noexcept {
    return a[id];
}

int IntArray::at(size_t id) const noexcept {
    return a[id];
}

size_t IntArray::size() const noexcept {
    return k;
}

int IntArray::last() const noexcept {
    return get(size() - 1);
}

void IntArray::add(int i) {
    int* tmp = new int[size() + 1];
    for (size_t j = 0; j < size(); ++j) {
        tmp[j] = get(j);
    }
    tmp[size()] = i;
    delete[] a;
    a = tmp;
    ++k;
}

int main() {
    int next = 0;
    if (!(std::cin >> next)) {
        return 1;
    }

    try {
        IntArray a(next);
        while (std::cin >> next) {
            a.add(next);
        }

        if (std::cin.fail() && !std::cin.eof()) {
            return 1;
        }

        int last_num = a.last();
        size_t count = 0;
        for (size_t i = 0; i < a.size() - 1; ++i) {
            int d = a.get(i);
            if (last_num != 0 && d % last_num == 0) {
                ++count;
            }
        }

        std::cout << count << "\n";
    }
    catch (const std::bad_alloc&) {
        return 2;
    }

    return 0;
}
