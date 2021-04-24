#include "parser.hpp"

#include "parametric.hpp"
#include <istream>
#include <string>
#include <fstream>

void parser::parse(screen& s, std::istream& in) {
    std::string line;
    while(in >> line) {
        if(line[0] == '#') {
            continue;
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
        if(line == "apply") {
            _e = _t.get_matrix() * _e;
        }
        if(line == "display") {
            s.clear();
            s.drawMatrix(_e, {255, 255, 255});
        }
        if(line == "save") {
            std::string filename;
            in >> filename;
            std::ofstream fout(filename);
            fout << s;
            fout.close();
        }
    }
}