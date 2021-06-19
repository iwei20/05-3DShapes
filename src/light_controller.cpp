#include "light_controller.hpp"

light_controller::light_controller(
    const std::tuple<double, double, double>& ka, 
    const std::tuple<double, double, double>& kd,
    const std::tuple<double, double, double>& ks,
    const vector3D& view = {0, 0, 1},
    const std::tuple<short, short, short>& amb
) : m_ka{ka}, m_kd{kd}, m_ks{ks}, m_view{view}, m_amb{amb} {}

std::tuple<short, short, short> light_controller::calc_ambient(const vector3D& normal) {
    short r, g, b;
    double kr, kg, kb;
    std::tie(r, g, b) = m_amb;
    std::tie(kr, kg, kb) = m_ka;
    return {kr * r, kg * g, kb * b};
}

std::tuple<short, short, short> light_controller::calc_diffuse(const vector3D& normal) {

}
std::tuple<short, short, short> light_controller::calc_specular(const vector3D& normal) {

}

void light_controller::add_light(const vector3D& vec, const std::tuple<short, short, short>& color) {

}

std::tuple<short, short, short> light_controller::calculate(const vector3D& normal, const std::tuple<short, short, short>& color) const {
    
}