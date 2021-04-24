#include "shapes3D.hpp"

box::box(const std::tuple<double, double, double>& lefttopfront, double width, double height, double depth) 
: m_lefttopfront{lefttopfront}, m_width{width}, m_height{height}, m_depth{depth} {}

void box::add(edge_matrix& e) const {
    double x0, y0, z0;
    std::tie(x0, y0, z0) = m_lefttopfront;
    double x1 = x0 + m_width, y1 = y0 + m_height, z1 = z0 + m_depth;
    
    e.add_edge({x0, y0, z0}, {x1, y0, z0});
    e.add_edge({x0, y0, z0}, {x0, y1, z0});
    e.add_edge({x0, y0, z0}, {x0, y0, z1});

    e.add_edge({x1, y0, z0}, {x1, y1, z0});
    e.add_edge({x1, y0, z0}, {x1, y0, z1});
    
    e.add_edge({x0, y1, z0}, {x1, y1, z0});
    e.add_edge({x0, y1, z0}, {x0, y1, z1});

    e.add_edge({x0, y0, z1}, {x1, y0, z1});
    e.add_edge({x0, y0, z1}, {x0, y1, z1});

    e.add_edge({x0, y1, z1}, {x1, y1, z1});
    e.add_edge({x1, y0, z1}, {x1, y1, z1});
    e.add_edge({x1, y1, z0}, {x1, y1, z1});
}

sphere::sphere(const std::tuple<double, double, double>& center, double radius)
: m_center{center}, m_radius{radius} {}

void sphere::add(edge_matrix& e) const {

}

torus::torus(const std::tuple<double, double, double>& center, double dist_radius, double cross_radius)
: m_center{center}, m_dist_radius{dist_radius}, m_cross_radius{cross_radius} {}

void torus::add(edge_matrix& e) const {

}