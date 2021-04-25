#pragma once

class shape3D;
class box;
class sphere;
class torus;

#include <tuple>
#include "matrix.hpp"

class box {
    private:
        std::tuple<double, double, double> m_lefttopfront;
        double m_width, m_height, m_depth;
    public:
        box(const std::tuple<double, double, double>& lefttopfront, double width, double height, double depth);
        void add_to(edge_matrix& e) const;
};

class sphere {
    private:
        std::tuple<double, double, double> m_center;
        double m_radius;
    public:
        sphere(const std::tuple<double, double, double>& center, double radius);
        /**
         * Adds a sphere to the edge matrix, with [2 * ppsc] semicircles and [ppsc] points per semicircle.
         */
        void add_to(edge_matrix& e, int ppsc) const;
        std::vector<std::tuple<double, double, double>> get_points(int ppsc) const;
};

class torus {
    private:
        std::tuple<double, double, double> m_center;
        double m_dist_radius, m_cross_radius;
    public:
        torus(const std::tuple<double, double, double>& center, double dist_radius, double cross_radius);

        /**
         * Adds a torus to the edge matrix, with [spt] circles and [ppslice] points per circle.
         */
        void add_to(edge_matrix& e, int spt, int ppslice) const;
        std::vector<std::tuple<double, double, double>> get_points(int spt, int ppslice) const;
};