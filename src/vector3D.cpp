#include "vector3D.hpp"

vector3D::vector3D(double x, double y, double z) : m_x{x}, m_y{y}, m_z{z} {}
vector3D::vector3D(const std::tuple<double, double, double>& xyz) : 
m_x{std::get<0>(xyz)}, m_y{std::get<1>(xyz)}, m_z{std::get<2>(xyz)} {}

double vector3D::x() const {return m_x;}
double vector3D::y() const {return m_y;}
double vector3D::z() const {return m_z;}

vector3D operator+(const vector3D& a, const vector3D& b) {
    return vector3D(a.x() + b.x(), a.y() + b.y(), a.z() + b.z());
}

vector3D operator-(const vector3D& a, const vector3D& b) {
    return vector3D(a.x() - b.x(), a.y() - b.y(), a.z() - b.z());
}

void vector3D::operator+=(const vector3D& other) {
    m_x += other.x();
    m_y += other.y();
    m_z += other.z();
}

void vector3D::operator-=(const vector3D& other) {
    m_x -= other.x();
    m_y -= other.y();
    m_z -= other.z();
}

double vector3D::dot(const vector3D& a, const vector3D& b) {
    return a.x() * b.x() + a.y() * b.y() + a.z() * b.z();
}

vector3D vector3D::cross(const vector3D& a, const vector3D& b) {
    return vector3D(
        a.y() * b.z() - a.z() * b.y(),
        a.z() * b.x() - a.x() * b.z(),
        a.x() * b.y() - a.y() * b.x()
    );
}