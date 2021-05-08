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
        /**
         * Constructs a box. The given coordinate is used as the coordinate of the left, top and front corner, and the given dimensions are used.
         **/
        box(const std::tuple<double, double, double>& lefttopfront, double width, double height, double depth);
        /**
         * Adds all the lines of this box to an edge matrix.
         **/
        void add_to(edge_matrix& e) const;
};

class sphere {
    private:
        std::tuple<double, double, double> m_center;
        double m_radius;
        int m_ppsc;
    public:
        sphere(const std::tuple<double, double, double>& center, double radius, int ppsc);
        /**
         * Adds all the points from get_points() into an edge matrix.
         */
        void add_to(polygon_matrix& e) const;
        /**
         * Gets points on the surface of a sphere, by rotating semicircles to get [2 * ppsc] semicircles and [ppsc] points per semicircle.
         */
        std::vector<std::tuple<double, double, double>> get_points() const;
};

class torus {
    private:
        std::tuple<double, double, double> m_center;
        double m_dist_radius, m_cross_radius;
    public:
        torus(const std::tuple<double, double, double>& center, double dist_radius, double cross_radius);
        /**
         * Adds all the points from get_points() into an edge matrix.
         */
        void add_to(edge_matrix& e, int spt, int ppslice) const;
        /**
         * Gets points on the surface of a torus, by rotating circles to get [spt] circles and [ppsc] points per circle.
         */
        std::vector<std::tuple<double, double, double>> get_points(int spt, int ppslice) const;
};