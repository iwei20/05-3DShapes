#include "matrix.hpp"

#include <stdexcept>
#include <tuple>

matrix::matrix(int rows, int cols) : data(rows, std::vector<double>(cols)) {}

matrix::matrix(const std::vector<std::vector<double>>& vector_2d) : data(vector_2d) {
    // Check if data has uniform column size
    if(data.size() > 1) {
        int col_size = data[0].size();
        for(int i = 1; i < data.size(); ++i) {
            if(data[i].size() != col_size) {
                throw std::invalid_argument("Input 2D vector does not have uniform column size");
            }
        }
    }
}

matrix& matrix::ident(int size) {
    if(size < 0) {
        throw std::invalid_argument("Negative identity matrix size");
    }
    matrix* result = new matrix(size, size);
    for(int i = 0; i < size; ++i) {
        (*result)[i][i] = 1;
    }
    return (*result);
}

std::ostream& operator<<(std::ostream& out, const matrix& mat) {
    for(const std::vector<double>& row : mat.data) {
        for(double item : row) {
            out << item << " ";
        }
        out << "\n";
    }
    return out;
}

matrix operator*(const matrix& a, const matrix& b) {
    if(a.width() != b.height()) {
        throw std::invalid_argument("Number of columns in matrix a should be equal to rows in matrix b");
    }
    matrix result(a.height(), b.width());
    for(int i = 0; i < a.height(); ++i) {
        for(int j = 0; j < b.width(); ++j) {
            double sum = 0;
            for(int k = 0; k < a.width(); ++k) {
                sum += a.get(i, k) * b.get(k, j);
            }
            result[i][j] = sum;
        }
    }
    return result;
}

std::vector<double>& matrix::operator[](int index) {
    return data[index];
}

double matrix::get(int row, int col) const {
    if(row < 0 || row >= data.size() || col < 0 || col >= data[0].size()) {
        throw std::invalid_argument("Requested matrix element out of bounds");
    }
    return data[row][col];
}

void matrix::set(int row, int col, double num) {
    if(row < 0 || row >= data.size() || col < 0 || col >= data[0].size()) {
        throw std::invalid_argument("Requested matrix element out of bounds");
    }
    data[row][col] = num;
}

int matrix::width() const {
    return data[0].size();
}

int matrix::height() const {
    return data.size();
}

edge_matrix::edge_matrix() : matrix(4, 0) {}

void edge_matrix::add_point(const std::tuple<double, double, double>& point) {
    double x, y, z;
    std::tie(x, y, z) = point;
    data[0].push_back(x);
    data[1].push_back(y);
    data[2].push_back(z);
    data[3].push_back(1);
}

void edge_matrix::add_edge(const std::tuple<double, double, double>& a, const std::tuple<double, double, double>& b) {
    add_point(a);
    add_point(b);
};

void edge_matrix::add_parametric(const parametric_func& x, const parametric_func& y, const parametric_func& z, int num_points) {
    std::vector<double> xpoints = x.get_range(num_points);
    std::vector<double> ypoints = y.get_range(num_points);
    std::vector<double> zpoints = z.get_range(num_points);

    for(int i = 0; i < num_points - 1; ++i) {
        add_edge({xpoints[i], ypoints[i], zpoints[i]}, {xpoints[i + 1], ypoints[i + 1], zpoints[i + 1]});
    }
}