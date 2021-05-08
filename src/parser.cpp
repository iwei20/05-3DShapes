#include "parser.hpp"

#include "parametric.hpp"
#include "shapes3D.hpp"
#include <iostream>
#include <istream>
#include <limits>
#include <sstream>
#include <string>
#include <fstream>

void parser::parse(screen& s, std::istream& in) {
    std::string line;
    while(in >> line) {
        if(line[0] == '#') {
            in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        if(line[0] == '/' && line[1] == '*') {
            in.ignore(std::numeric_limits<std::streamsize>::max(), '*');
        }
        if(line == "line") {
            double x1, y1, z1, x2, y2, z2;
            in >> x1 >> y1 >> z1 >> x2 >> y2 >> z2;
            _e.add_edge({x1, y1, z1}, {x2, y2, z2});
        }
        if(line == "ident") {
            _t.reset();
        }
        if(line == "scale") {
            double sx, sy, sz;
            in >> sx >> sy >> sz;
            _t.dilate(sx, sy, sz);
        }
        if(line == "move") {
            double tx, ty, tz;
            in >> tx >> ty >> tz;
            _t.translate(tx, ty, tz);
        }
        if(line == "rotate") {
            char axis;
            double degrees;
            in >> axis >> degrees;
            switch(axis) {
                case 'x':
                    _t.xRot(degrees);
                    break;
                case 'y':
                    _t.yRot(degrees);
                    break;
                case 'z':
                    _t.zRot(degrees);
                    break;
            }
        }
        if(line == "circle") {
            double cx, cy, cz, r;
            in >> cx >> cy >> cz >> r;
            circle_parametric c(cx, cy, r);
            _e.add_parametric(c.xfunc(), c.yfunc(), const_parametric(cz), 100);
        }
        if(line == "hermite") {
            double x0, y0, x1, y1, rx0, ry0, rx1, ry1;
            in >> x0 >> y0 >> x1 >> y1 >> rx0 >> ry0 >> rx1 >> ry1;
            hermite_parametric h({x0, y0}, {x1, y1}, {rx0, ry0}, {rx1, ry1});
            _e.add_parametric(h.xfunc(), h.yfunc(), const_parametric(0), 100);
        }
        if(line == "bezier") {
            double x0, y0, x1, y1, x2, y2, x3, y3;
            in >> x0 >> y0 >> x1 >> y1 >> x2 >> y2 >> x3 >> y3;
            bezier_parametric b({x0, y0}, {x1, y1}, {x2, y2}, {x3, y3});
            _e.add_parametric(b.xfunc(), b.yfunc(), const_parametric(0), 100);
        }
        if(line == "box") {
            double x, y, z, w, h, d;
            in >> x >> y >> z >> w >> h >> d;
            box b{{x, y, z}, w, h, d};
            b.add_to(_s);
        }
        if(line == "sphere") {
            double x, y, z, r;
            in >> x >> y >> z >> r;
            sphere s{{x, y, z}, r, (int)r/5};
            s.add_to(_s);
        }
        if(line == "torus") {
            double x, y, z, r1, r2;
            in >> x >> y >> z >> r1 >> r2;
            torus t{{x, y, z}, r2, r1, (int)r2/2, (int)r1/2};
            t.add_to(_s);
        }
        if(line == "clear") {
            _e = edge_matrix();
            _s = polygon_matrix();
        }
        if(line == "apply") {
            _e = _t.get_matrix() * _e;
            _s = _t.get_matrix() * _s;
        }
        if(line == "display") {
            s.clear();
            s.drawMatrix(_e, {255, 255, 255});
            s.drawMatrix(_s, {255, 255, 255});
            std::ofstream fout("temp.ppm");
            fout << s;
            fout.close();
            auto fp = popen("magick display temp.ppm", "w");
            pclose(fp);
            remove("temp.ppm");
        }
        if(line == "save") {
            std::string filename;
            in >> filename;
            std::ofstream fout(filename);
            fout << s;
            fout.close();
            std::cout << "Picture can be found at " << filename << "\n";
        }
    }
}