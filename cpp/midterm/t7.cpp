#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <vector>

template <typename T> class Matrix {
  private:
    std::vector<T> data;
    size_t rows, cols;

  public:
    Matrix(size_t r, size_t c, const T &init = T())
        : data(r * c, init), rows(r), cols(c) {}

    class MatrixSlice {
    // Code Here
      private:
        std::vector<T*> dt;
        size_t rows, cols;

      public:
        MatrixSlice(T* src, size_t allc, size_t r_start,
                    size_t c_start, size_t r, size_t c)
            : rows(r), cols(c) {
          for (size_t i = r_start; i < r_start + r; i++) {
            for (size_t j = c_start; j < c_start + c; j++) {
              int idx = i * allc + j;
              dt.push_back(&src[idx]);
            }
          }
        }
        void print() const {
          for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
              std::cout << std::setw(5) << *dt[i * cols + j] << " ";
            }
            std::cout << '\n';
          }
        }
        T& operator()(int i, int j) const { return *dt[i * cols + j]; }
    };

    T operator()(size_t i, size_t j) const { 
      return data[i * cols + j]; 
    }
    T& operator() (size_t i, size_t j) {
      return data[i * cols + j];
    }
    // Code End
    size_t rowCount() const { return rows; }
    size_t colCount() const { return cols; }

    void print() const {
      for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
          std::cout << std::setw(5) << (*this)(i, j) << " ";
        }
        std::cout << '\n';
      }
    }

    MatrixSlice slice(size_t row_start, size_t row_end, size_t col_start,
                      size_t col_end) {
      if (row_start > row_end || col_start > col_end || row_end > rows ||
          col_end > cols)
        throw std::out_of_range("Slice indices out of range");

      return MatrixSlice(data.data(), cols, row_start, col_start,
                         row_end - row_start, col_end - col_start);
    }
};

int main() {
  Matrix<int> mat(4, 5);
  for (size_t i = 0; i < mat.rowCount(); ++i)
    for (size_t j = 0; j < mat.colCount(); ++j)
      std::cin >> mat(i, j);

  std::cout << "Original Matrix:\n";
  mat.print();

  auto submat = mat.slice(1, 3, 2, 5);
  std::cout << "\nSliced Matrix (rows 1-2, cols 2-4):\n";
  submat.print();

  submat(0, 0) = 999;

  std::cout << "\nModified Original Matrix:\n";
  mat.print();

  return 0;
}