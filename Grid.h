#ifndef GRID_H_INCLUDED
#define GRID_H_INCLUDED

#include <vector>
#include <cassert>
#include <iostream>

template<typename T>
class Grid {
private:
    std::vector<std::vector<T>> data;

    class ProxyRow {
    private:
        std::vector<T>& row_ref;
    public:
        explicit ProxyRow(std::vector<T>& row) : row_ref(row) {}
        
        auto operator[](int col) -> decltype(row_ref[col]) {
            assert(col >= 0 && size_t(col) < row_ref.size());
            return row_ref[col];
        }
    };

    class ConstProxyRow {
    private:
      const std::vector<T>& row_ref;
    public:
        explicit ConstProxyRow(const std::vector<T>& row) : row_ref(row) {}
        
        auto operator[](int col) const -> decltype(row_ref[col]) {
            assert(col >= 0 && size_t(col) < row_ref.size());
            return row_ref[col];
    };
};

public:
    Grid() : data() {}
    Grid(int rows, int cols) : data(rows, std::vector<T>(cols)) {}
    Grid(int rows, int cols, T t) : data(rows, std::vector<T>(cols, t)) {}
    
    ProxyRow operator[](int row) {
        assert(row >= 0 && size_t(row) < data.size());
        return ProxyRow(data[row]);
    }
    
    ConstProxyRow operator[](int row) const {
        assert(row >= 0 && size_t(row) < data.size());
        return ConstProxyRow(data[row]);
    }

    size_t numRows() const { return data.size(); }
    size_t numCols() const { return data.empty() ? 0 : data[0].size(); }

    bool inBounds(int row, int col) const {
      return row >= 0 && size_t(row) < numRows() &&
        col >= 0 && size_t(col) < numCols();
    }

    friend std::ostream& operator<<(std::ostream& os, const Grid<T>& grid) {
        for (size_t i = 0; i < grid.numRows(); ++i) {
            for (size_t j = 0; j < grid.numCols(); ++j) {
                os << grid[i][j];
                if (j != grid.numCols() - 1) os << " ";
            }
            os << "\n";
        }
        return os;
    }
};

#endif // GRID_H_INCLUDED