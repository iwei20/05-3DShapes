#include "shapes3D.hpp"

#include <cmath>

box::box(const std::tuple<double, double, double>& lefttopfront, double width, double height, double depth) 
: m_lefttopfront{lefttopfront}, m_width{width}, m_height{height}, m_depth{depth} {}

void box::add_to(edge_matrix& e) const {
    double x0, y0, z0;
    std::tie(x0, y0, z0) = m_lefttopfront;
    double x1 = x0 + m_width, y1 = y0 - m_height, z1 = z0 - m_depth;

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

void sphere::add_to(edge_matrix& e, int ppsc) const {
    for (const std::tuple<double, double, double>& point : get_points(ppsc)) {
        e.add_edge(point, point);
    }
}

std::vector<std::tuple<double, double, double>> sphere::get_points(int ppsc) const {
    std::vector<std::tuple<double, double, double>> points;
    const double MULT_PPSC = 1.0 / ppsc;
    for (int rot = 0; rot < 2 * ppsc; ++rot) {
        for (int cir = 0; cir < ppsc ; ++cir) {
            std::tuple<double, double, double> point{
                m_radius * cos(M_PI * cir * MULT_PPSC) + std::get<0>(m_center),
                m_radius * sin(M_PI * cir * MULT_PPSC) * cos(M_PI * rot * MULT_PPSC) + std::get<1>(m_center),
                m_radius * sin(M_PI * cir * MULT_PPSC) * sin(M_PI * rot * MULT_PPSC) + std::get<2>(m_center)
            };
            points.push_back(point);
        }
    }
    return points;
}

torus::torus(const std::tuple<double, double, double>& center, double dist_radius, double cross_radius)
: m_center{center}, m_dist_radius{dist_radius}, m_cross_radius{cross_radius} {}

void torus::add_to(edge_matrix& e, int spt, int ppslice) const {
    for (const std::tuple<double, double, double>& point : get_points(spt, ppslice)) {
        e.add_edge(point, point);
    }
}

std::vector<std::tuple<double, double, double>> torus::get_points(int spt, int ppslice) const {
    std::vector<std::tuple<double, double, double>> points;
    const double MULT_SPT = 2.0 / spt;
    const double MULT_PPS = 2.0 / ppslice;
    for (int rot = 0; rot < spt; ++rot) {
        for (int cir = 0; cir < ppslice; ++cir) {
            std::tuple<double, double, double> point{
                cos(M_PI * rot * MULT_SPT) * (m_cross_radius * cos(M_PI * cir * MULT_PPS) + m_dist_radius) + std::get<0>(m_center),
                cos(M_PI * cir * MULT_PPS) + std::get<1>(m_center),
                -sin(M_PI * rot * MULT_SPT) * (m_cross_radius * sin(M_PI * cir * MULT_PPS) + m_dist_radius) + std::get<2>(m_center)
            };
            points.push_back(point);
        }
    }
    return points;
}