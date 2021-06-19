#pragma once

class vector3D;

#include <tuple>

class vector3D {
    private:
        double m_x, m_y, m_z;
    public:
        vector3D(double x, double y, double z);
        vector3D(const std::tuple<double, double, double>& xyz);

        double x() const;
        double y() const;
        double z() const;

        void operator+=(const vector3D& other);
        void operator-=(const vector3D& other);

        static vector3D normalize(const vector3D& vec);
        static double dot(const vector3D& a, const vector3D& b);
        static vector3D cross(const vector3D& a, const vector3D& b);
};
vector3D operator+(const vector3D& a, const vector3D& b);
vector3D operator-(const vector3D& a, const vector3D& b);
vector3D operator*(const double scale, const vector3D& vec);