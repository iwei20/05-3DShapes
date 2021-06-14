class light_controller;

#include "vector3D.hpp"
#include <vector>

class light_controller {
    private:
        std::vector<std::tuple<vector3D, std::tuple<short, short, short>>> lightvectors;
        std::tuple<double, double, double> m_ka;
        std::tuple<double, double, double> m_kd;
        std::tuple<double, double, double> m_ks;
        vector3D m_view;

        std::tuple<short, short, short> calc_ambient();
        std::tuple<short, short, short> calc_diffuse();
        std::tuple<short, short, short> calc_specular();
    public:
        light_controller(
            const std::tuple<double, double, double>& ka, 
            const std::tuple<double, double, double>& kd,
            const std::tuple<double, double, double>& ks,
            vector3D view={0,0,1}
        );
        void add_light(vector3D vec, std::tuple<short, short, short> color);
        std::tuple<short, short, short> calculate(const vector3D& normal) const;
};