#include "transform_manager.hpp"

#include <cmath>

transform_manager::transform_manager() : m{matrix::ident(4)} {}

const double convert_rad = M_PI / 180;
void transform_manager::translate(double x_units, double y_units, double z_units) {
    matrix to_mult = matrix::ident(4);
    to_mult[0][3] = x_units;
    to_mult[1][3] = y_units;
    to_mult[2][3] = z_units;
    m = to_mult * m;
}

void transform_manager::dilate(double x_scale, double y_scale, double z_scale) {
    matrix to_mult = matrix::ident(4);
    to_mult[0][0] = x_scale;
    to_mult[1][1] = y_scale;
    to_mult[2][2] = z_scale;
    m = to_mult * m;
}

void transform_manager::xRot(double degrees) {
    matrix to_mult = matrix::ident(4);
    double radians = degrees * convert_rad;
    to_mult[1][1] = cos(radians);
    to_mult[1][2] = -sin(radians);
    to_mult[2][1] = sin(radians);
    to_mult[2][2] = cos(radians);
    m = to_mult * m;
}

void transform_manager::yRot(double degrees) {
    matrix to_mult = matrix::ident(4);
    double radians = degrees * convert_rad;
    to_mult[2][2] = cos(radians);
    to_mult[2][0] = -sin(radians);
    to_mult[0][2] = sin(radians);
    to_mult[0][0] = cos(radians);
    m = to_mult * m;
}

void transform_manager::zRot(double degrees) {
    matrix to_mult = matrix::ident(4);
    double radians = degrees * convert_rad;
    to_mult[0][0] = cos(radians);
    to_mult[0][1] = -sin(radians);
    to_mult[1][0] = sin(radians);
    to_mult[1][1] = cos(radians);
    m = to_mult * m;
}

const matrix& transform_manager::get_matrix() {
    return m;
}

void transform_manager::reset() {
    m = matrix::ident(4);
}