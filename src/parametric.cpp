#include "parametric.hpp"

#include <cmath>
#include <iostream> 
std::vector<double> parametric_func::get_range(int num_vals) const {
    std::vector<double> range;
    double increment = 1.0 / (num_vals - 1); 
    for(int i = 0; i < num_vals; ++i) {
        range.push_back(get(i * increment));
    }
    return range;
}

const_parametric::const_parametric(double constant) : m_constant{constant} {}
double const_parametric::get(double t) const {
    return m_constant;
}

circle_parametric::circle_parametric(double center_x, double center_y, double radius) : parax{center_x, radius}, paray{center_y, radius} {}

const circle_parametric::circle_parametric_x& circle_parametric::xfunc() const {
    return parax;
}

const circle_parametric::circle_parametric_y& circle_parametric::yfunc() const {
    return paray;
}

circle_parametric::circle_parametric_x::circle_parametric_x(double center_x, double radius) : m_center_x{center_x}, m_radius{radius} {}
circle_parametric::circle_parametric_y::circle_parametric_y(double center_y, double radius) : m_center_y{center_y}, m_radius{radius} {}

double circle_parametric::circle_parametric_x::get(double t) const {
    return m_radius * cos(2 * M_PI * t) + m_center_x;
}

double circle_parametric::circle_parametric_y::get(double t) const {
    return m_radius * sin(2 * M_PI * t) + m_center_y;
}

matrix hermite_parametric::inverse_mult(
    std::vector<std::vector<double>>
    {
        { 2, -2,  1,  1}, 
        {-3,  3, -2, -1}, 
        { 0,  0,  1,  0}, 
        { 1,  0,  0,  0}
    }
);

hermite_parametric::hermite_parametric(
    const std::pair<double, double>& p0, 
    const std::pair<double, double>& p1, 
    const std::pair<double, double>& r0, 
    const std::pair<double, double>& r1
) : parax{p0.first, p1.first, r0.first, r1.first}, paray{p0.second, p1.second, r0.second, r1.second} {}

const hermite_parametric::hermite_parametric_x& hermite_parametric::xfunc() const {
    return parax;
}

const hermite_parametric::hermite_parametric_y& hermite_parametric::yfunc() const {
    return paray;
}

hermite_parametric::hermite_parametric_x::hermite_parametric_x(double x0, double x1, double rx0, double rx1) 
: m{inverse_mult * std::vector<std::vector<double>>{{x0}, {x1}, {rx0}, {rx1}}} {}

hermite_parametric::hermite_parametric_y::hermite_parametric_y(double y0, double y1, double ry0, double ry1) 
: m{inverse_mult * std::vector<std::vector<double>>{{y0}, {y1}, {ry0}, {ry1}}} {}

double hermite_parametric::hermite_parametric_x::get(double t) const {
    return m.get(0, 0) * t * t * t + m.get(1, 0) * t * t + m.get(2, 0) * t + m.get(3, 0);
}

double hermite_parametric::hermite_parametric_y::get(double t) const {
    return m.get(0, 0) * t * t * t + m.get(1, 0) * t * t + m.get(2, 0) * t + m.get(3, 0);
}

matrix bezier_parametric::binomial_mult(std::vector<std::vector<double>>{
    {-1,  3, -3,  1},
    { 3, -6,  3,  0},
    {-3,  3,  0,  0},
    { 1,  0,  0,  0}
});

bezier_parametric::bezier_parametric(
    const std::pair<double, double>& p0, 
    const std::pair<double, double>& p1, 
    const std::pair<double, double>& p2, 
    const std::pair<double, double>& p3
) : parax{p0.first, p1.first, p2.first, p3.first}, paray{p0.second, p1.second, p2.second, p3.second} {}

const bezier_parametric::bezier_parametric_x& bezier_parametric::xfunc() const {
    return parax;
}

const bezier_parametric::bezier_parametric_y& bezier_parametric::yfunc() const {
    return paray;
}

bezier_parametric::bezier_parametric_x::bezier_parametric_x(double x0, double x1, double x2, double x3) 
: m{binomial_mult * std::vector<std::vector<double>>{{x0}, {x1}, {x2}, {x3}}} {}

bezier_parametric::bezier_parametric_y::bezier_parametric_y(double y0, double y1, double y2, double y3) 
: m{binomial_mult * std::vector<std::vector<double>>{{y0}, {y1}, {y2}, {y3}}} {}

double bezier_parametric::bezier_parametric_x::get(double t) const {
    return m.get(0, 0) * t * t * t + m.get(1, 0) * t * t + m.get(2, 0) * t + m.get(3, 0);
}

double bezier_parametric::bezier_parametric_y::get(double t) const {
    return m.get(0, 0) * t * t * t + m.get(1, 0) * t * t + m.get(2, 0) * t + m.get(3, 0);
}
