#include <iostream>
#include <fstream>
#include <new>

struct IntArray {
  IntArray(int i);
  ~IntArray();
  void add(int i);
  int get(size_t id) const noexcept;
  size_t size() const noexcept;
  int last() const noexcept;

  int* a;
  size_t k;
};

IntArray::~IntArray() {
  delete[] a;
}

IntArray::IntArray(int i)
  : a(new (std::nothrow) int[1]),
    k(1)
{
  if (!a) {
    return;
  }
  *a = i;
}

void IntArray::add(int i) {
  size_t old_size = size();
  int* tmp = new (std::nothrow) int[old_size + 1];
  if (!tmp) return;
  for (size_t j = 0; j < old_size; ++j) {
    tmp[j] = get(j);
  }
  tmp[old_size] = i;
  delete[] a;
  a = tmp;
  ++k;
}

int IntArray::get(size_t id) const noexcept {
  return a[id];
}

size_t IntArray::size() const noexcept {
  return k;
}

int IntArray::last() const noexcept {
  return get(size() - 1);
}

class IntMatrix : public IntArray {
public:
  IntMatrix(size_t rows, size_t cols)
    : IntArray(0), rows_(rows), cols_(cols)
  {
    a = new (std::nothrow) int[rows * cols];
    k = rows * cols;
    if (a) {
      for (size_t i = 0; i < k; ++i)
        a[i] = 0;
    }
  }

  ~IntMatrix() override {
  }

  size_t getRows() const noexcept { return rows_; }
  size_t getCols() const noexcept { return cols_; }

  int get(size_t r, size_t c) const noexcept {
    return a[r * cols_ + c];
  }

  void set(size_t r, size_t c, int v) noexcept {
    a[r * cols_ + c] = v;
  }

  bool addRowAfter(size_t row_after, int fill_val) {
    if (row_after > rows_) return false;
    size_t new_rows = rows_ + 1;
    size_t new_size = new_rows * cols_;
    int* new_a = new (std::nothrow) int[new_size];
    if (!new_a) return false;
    for (size_t r = 0; r < rows_; ++r) {
      for (size_t c = 0; c < cols_; ++c) {
        size_t src_idx = r * cols_ + c;
        size_t dst_r = (r < row_after) ? r : r + 1;
        size_t dst_idx = dst_r * cols_ + c;
        new_a[dst_idx] = a[src_idx];
      }
    }
    for (size_t c = 0; c < cols_; ++c) {
      new_a[row_after * cols_ + c] = fill_val;
    }
    delete[] a;
    a = new_a;
    k = new_size;
    rows_ = new_rows;
    return true;
  }

  bool addRowAndColAfter(size_t row_after, size_t col_after) {
    if (row_after > rows_ || col_after > cols_) return false;
    size_t new_rows = rows_ + 1;
    size_t new_cols = cols_ + 1;
    size_t new_size = new_rows * new_cols;
    int* new_a = new (std::nothrow) int[new_size];
    if (!new_a) return false;
    for (size_t i = 0; i < new_size; ++i)
      new_a[i] = 0;
    for (size_t r = 0; r < rows_; ++r) {
      for (size_t c = 0; c < cols_; ++c) {
        size_t src = r * cols_ + c;
        size_t dst_r = (r < row_after) ? r : r + 1;
        size_t dst_c = (c < col_after) ? c : c + 1;
        size_t dst = dst_r * new_cols + dst_c;
        new_a[dst] = a[src];
      }
    }
    delete[] a;
    a = new_a;
    k = new_size;
    rows_ = new_rows;
    cols_ = new_cols;
    return true;
  }

  void print() const {
    for (size_t r = 0; r < rows_; ++r) {
      for (size_t c = 0; c < cols_; ++c) {
        std::cout << get(r, c);
        if (c + 1 < cols_) std::cout << ' ';
      }
      std::cout << '\n';
    }
  }

private:
  size_t rows_;
  size_t cols_;
};

int main(int argc, char* argv[]) {
  if (argc != 2) {
    return 1;
  }
  char* filename = argv[1];
  std::ifstream file(filename);
  if (!file.is_open()) {
    return 1;
  }
  size_t rows, cols;
  if (!(file >> rows >> cols)) {
    file.close();
    return 1;
  }
  IntMatrix m(rows, cols);
  if (!m.a) {
    file.close();
    return 2;
  }
  for (size_t r = 0; r < rows; ++r) {
    for (size_t c = 0; c < cols; ++c) {
      int val;
      if (!(file >> val)) {
        file.close();
        return 1;
      }
      m.set(r, c, val);
    }
  }
  file.close();
  int cmd, p1, p2;
  while (std::cin >> cmd >> p1 >> p2) {
    if (cmd == 1) {
      if (p1 < 0) return 3;
      if (!m.addRowAfter(static_cast<size_t>(p1), p2)) return 3;
    } else if (cmd == 3) {
      if (p1 < 0 || p2 < 0) return 3;
      if (!m.addRowAndColAfter(static_cast<size_t>(p1), static_cast<size_t>(p2))) return 3;
    } else {
      return 3;
    }
    m.print();
    std::cout << '\n';
  }
  if (std::cin.fail() && !std::cin.eof()) {
    return 1;
  }
  return 0;
}
