#include "light_controller.hpp"

#include <cmath>
light_controller::light_controller(
    const std::tuple<double, double, double>& ka, 
    const std::tuple<double, double, double>& kd,
    const std::tuple<double, double, double>& ks,
    int spec_x,
    const std::tuple<short, short, short>& amb,
    const vector3D& view
) : m_ka{ka}, m_kd{kd}, m_ks{ks}, m_spec_x{spec_x}, m_view{view}, m_amb{amb} {}

std::tuple<short, short, short> light_controller::calc_ambient() const {
    short r, g, b;
    double kr, kg, kb;
    std::tie(r, g, b) = m_amb;
    std::tie(kr, kg, kb) = m_ka;
    return {kr * r, kg * g, kb * b};
}

std::tuple<short, short, short> light_controller::calc_diffuse(const vector3D& normal) const {
    short rr=0, rg=0, rb=0;
    for(const std::tuple<vector3D, std::tuple<short, short, short>>& light : lightvectors) {
        short r, g, b;
        double kr, kg, kb;
        std::tie(r, g, b) = std::get<1>(light);
        std::tie(kr, kg, kb) = m_kd;

        double multiplier = std::max(0.0, vector3D::dot(normal, std::get<0>(light)));

        if(rr < 255) rr += std::min(255.0, rr + kr * r * multiplier);
        if(rg < 255) rg += std::min(255.0, rg + kg * g * multiplier);
        if(rb < 255) rb += std::min(255.0, rb + kb * b * multiplier);
    }

    return {rr, rg, rb};
}

std::tuple<short, short, short> light_controller::calc_specular(const vector3D& normal) const {
    short rr=0, rg=0, rb=0;
    for(const std::tuple<vector3D, std::tuple<short, short, short>>& light : lightvectors) {
        short r, g, b;
        double kr, kg, kb;
        std::tie(r, g, b) = std::get<1>(light);
        std::tie(kr, kg, kb) = m_ks;

        double multiplier = 
            std::pow(
                std::max(0.0, vector3D::dot(2 * vector3D::dot(normal, std::get<0>(light)) * normal - std::get<0>(light), m_view)), 
                m_spec_x
            );
        
        if(rr < 255) rr += std::min(255.0, rr + kr * r * multiplier);
        if(rg < 255) rg += std::min(255.0, rg + kg * g * multiplier);
        if(rb < 255) rb += std::min(255.0, rb + kb * b * multiplier);
    }

    return {rr, rg, rb};
}

void light_controller::add_light(const vector3D& vec, const std::tuple<short, short, short>& color) {
    lightvectors.push_back({vector3D::normalize(vec), color});
}

std::tuple<short, short, short> light_controller::calculate(const vector3D& normal) const {
    std::tuple<short, short, short> ambient = calc_ambient();
    std::tuple<short, short, short> diffuse = calc_diffuse(vector3D::normalize(normal));
    std::tuple<short, short, short> specular = calc_specular(vector3D::normalize(normal));

    return {
        std::min(255, std::get<0>(ambient) + std::get<0>(diffuse) + std::get<0>(specular)),
        std::min(255, std::get<1>(ambient) + std::get<1>(diffuse) + std::get<1>(specular)),
        std::min(255, std::get<2>(ambient) + std::get<2>(diffuse) + std::get<2>(specular)),
    };
}