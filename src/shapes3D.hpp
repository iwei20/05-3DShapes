#pragma once

class shape;
class box;
class sphere;
class torus;

#include <tuple>
#include "matrix.hpp"

class shape {
    public:
        virtual void add(edge_matrix& e) const = 0;
};

class box : public shape {
    private:
        std::tuple<double, double, double> m_lefttopfront;
        double m_width, m_height, m_depth;
    public:
        box(const std::tuple<double, double, double>& lefttopfront, double width, double height, double depth);
        void add(edge_matrix& e) const;
};

class sphere : public shape {
    private:
        std::tuple<double, double, double> m_center;
        double m_radius;
    public:
        sphere(const std::tuple<double, double, double>& center, double radius);
        void add(edge_matrix& e) const;
};

class torus : public shape {
    private:
        std::tuple<double, double, double> m_center;
        double m_dist_radius, m_cross_radius;
    public:
        torus(const std::tuple<double, double, double>& center, double dist_radius, double cross_radius);
        void add(edge_matrix& e) const;
};