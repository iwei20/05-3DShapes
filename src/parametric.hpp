#pragma once

class parametric_func;
class const_parametric;
class circle_parametric;
class hermite_parametric;
class bezier_parametric;

#include "matrix.hpp"
#include <vector>

class parametric_func {
    public:
        virtual double get(double t) const = 0;
        std::vector<double> get_range(int num_vals) const;
};

class const_parametric : public parametric_func {
    private:
        double m_constant;
    public:
        const_parametric(double constant);
        double get(double t) const override;
};

class circle_parametric {
    private:
        class circle_parametric_x : public parametric_func {
            private:
                double m_center_x;
                double m_radius;
            public:
                circle_parametric_x(double center_x, double radius);
                double get(double t) const override;
        };

        class circle_parametric_y : public parametric_func {
            private:
                double m_center_y;
                double m_radius;
            public:
                circle_parametric_y(double center_y, double radius);
                double get(double t) const override;
        };
        circle_parametric_x parax;
        circle_parametric_y paray;
    public:
        circle_parametric(double center_x, double center_y, double radius);
        const circle_parametric_x& xfunc() const;
        const circle_parametric_y& yfunc() const;
};

class hermite_parametric {
    private:
        static matrix inverse_mult;
        class hermite_parametric_x : public parametric_func {
            private:
                matrix m;
            public:
                hermite_parametric_x(double x0, double x1, double rx0, double rx1);
                double get(double t) const override;
        };
        class hermite_parametric_y : public parametric_func {
            private:
                matrix m;
            public:
                hermite_parametric_y(double y0, double y1, double ry0, double ry1);
                double get(double t) const override;
        };
        hermite_parametric_x parax;
        hermite_parametric_y paray;
    public:
        hermite_parametric(
            const std::pair<double, double>& p0, 
            const std::pair<double, double>& p1, 
            const std::pair<double, double>& r0, 
            const std::pair<double, double>& r1
        );
        const hermite_parametric_x& xfunc() const;
        const hermite_parametric_y& yfunc() const;
};

class bezier_parametric {
    private:
        static matrix binomial_mult;
        class bezier_parametric_x : public parametric_func {
            private:
                matrix m;
                double m_x0, m_x1, m_x2, m_x3;
            public:
                bezier_parametric_x(double x0, double x1, double x2, double x3);
                double get(double t) const override;
        };
        class bezier_parametric_y : public parametric_func {
            private:
                matrix m;
                double m_y0, m_y1, m_y2, m_y3;
            public:
                bezier_parametric_y(double y0, double y1, double y2, double y3);
                double get(double t) const override;
        };
        bezier_parametric_x parax;
        bezier_parametric_y paray;
    public:
        bezier_parametric(
            const std::pair<double, double>& p0, 
            const std::pair<double, double>& p1, 
            const std::pair<double, double>& p2, 
            const std::pair<double, double>& p3
        );
        const bezier_parametric_x& xfunc() const;
        const bezier_parametric_y& yfunc() const;
};