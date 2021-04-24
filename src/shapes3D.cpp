#include "shapes3D.hpp"

box::box(const std::tuple<double, double, double>& frontlefttop, double width, double height, double depth) 
: m_frontlefttop{frontlefttop}, m_width{width}, m_height{height}, m_depth{depth} {}

sphere::sphere(const std::tuple<double, double, double>& center, double radius)
: m_center{center}, m_radius{radius} {}

torus::torus(const std::tuple<double, double, double>& center, double dist_radius, double cross_radius)
: m_center{center}, m_dist_radius{dist_radius}, m_cross_radius{cross_radius} {}