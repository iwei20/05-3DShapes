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

sphere::sphere(const std::tuple<double, double, double>& center, double radius, int ppsc)
: m_center{center}, m_radius{radius}, m_ppsc{ppsc} {}

void sphere::add_to(polygon_matrix& e) const {
    std::vector<std::tuple<double, double, double>> points = get_points();
    // Amogus each row
    for (int i = 0; i < points.size(); i += m_ppsc) {
        // Beginning one triangle
        e.add_triangle(points[i], points[i + 1], points[i + m_ppsc + 1]);
        // End one triangle
        e.add_triangle(points[i + m_ppsc - 2], points[i + m_ppsc - 1], points[i + 2 * m_ppsc - 1]);
        // Triangles in between
        for(int j = i + 1; j < i + m_ppsc - 2; ++j) {
            e.add_triangle(points[j], points[j + 1], points[j + m_ppsc + 1]);
            e.add_triangle(points[j], points[j + m_ppsc + 1], points[j + m_ppsc]);
        }
    }
}

std::vector<std::tuple<double, double, double>> sphere::get_points() const {
    std::vector<std::tuple<double, double, double>> points;
    const double MULT_PPSC = M_PI / m_ppsc;
    for (int rot = 0; rot < 2 * m_ppsc; ++rot) {
        for (int cir = 0; cir < m_ppsc ; ++cir) {
            std::tuple<double, double, double> point{
                m_radius * cos(cir * MULT_PPSC) + std::get<0>(m_center),
                m_radius * sin(cir * MULT_PPSC) * cos(rot * MULT_PPSC) + std::get<1>(m_center),
                m_radius * sin(cir * MULT_PPSC) * sin(rot * MULT_PPSC) + std::get<2>(m_center)
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
            double pi_rot_spt = M_PI * rot * MULT_SPT;
            double pi_cir_pps = M_PI * cir * MULT_PPS;
            std::tuple<double, double, double> point{
                cos(pi_rot_spt) * (m_cross_radius * cos(pi_cir_pps) + m_dist_radius) + std::get<0>(m_center),
                m_cross_radius * sin(pi_cir_pps) + std::get<1>(m_center),
                -sin(pi_rot_spt) * (m_cross_radius * cos(pi_cir_pps) + m_dist_radius) + std::get<2>(m_center)
            };
            points.push_back(point);
        }
    }
    return points;
}