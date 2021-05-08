#pragma once

class matrix;
class edge_matrix;

#include <vector>
#include <ostream>

class matrix {
protected:
    std::vector<std::vector<double>> data;
public:
    /**
     * Initializes the matrix with the specified dimensions.
     **/
    matrix(unsigned long rows, unsigned long cols);

    matrix(const std::vector<std::vector<double>>& vec_2d);
    /**
     * Creates a size x size identity matrix
     **/
    static matrix& ident(int size);

    /**
     * Outputs contents of matrix, where spaces separate entries in a row and rows are separated by a space followed by a newline.
     **/
    friend std::ostream& operator<<(std::ostream& out, const matrix& mat);

    /**
     * Performs matrix multiplication and returns the result.
     **/
    friend matrix operator*(const matrix& a, const matrix& b);

    /**
     * Reference to matrix entry
     **/
    std::vector<double>& operator[](int index);

    /**
     * Returns matrix entry
     **/
    double get(int row, int col) const;

    /**
     * Sets matrix entry
     **/
    void set(int row, int col, double num);

    /**
     * Returns the width (number of columns) of this matrix
     **/
    int width() const;

    /**
     * Retursn the height (number of rows) of this matrix
     **/
    int height() const;
};

#include "parametric.hpp"

class edge_matrix : public matrix {
    private:
        /**
         * Given tuple (x, y, z), appends x, y, z, and 1 to the end of each row in the edge matrix.
         **/
        void add_point(const std::tuple<double, double, double>& point);
    public:
        edge_matrix();

        /**
         * Adds points a and b to the end of the matrix.
         **/
        void add_edge(const std::tuple<double, double, double>& a, const std::tuple<double, double, double>& b);

        /**
         * Takes three parametric functions for x, y, and z, and plots them evaluated at 0 to 1 lerping over num_points points.
         **/
        void add_parametric(const parametric_func& x, const parametric_func& y, const parametric_func& z, int num_points);
        using matrix::operator=;
};

class polygon_matrix : public matrix {
    private:
        /**
         * Given tuple (x, y, z), appends x, y, z, and 1 to the end of each row in the edge matrix.
         **/
        void add_point(const std::tuple<double, double, double>& point);
    public:
        polygon_matrix();
        /**
         * Points of the tirangle must be ccw. 
         **/
        void add_triangle(const std::tuple<double, double, double>& a, const std::tuple<double, double, double>& b, const std::tuple<double, double, double>& c);
        using matrix::operator=;
};
