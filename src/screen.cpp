#include "screen.hpp"

#include "vector3D.hpp"
#include <iostream>
#include <algorithm>
#include <numeric>

zbuffer::zbuffer(unsigned long width, unsigned long height) : zvalues{height, std::vector<double>(width, std::numeric_limits<double>::lowest())} {}
std::vector<double>& zbuffer::operator[](int index) {
    return zvalues[index];
}

void zbuffer::reset() {
    for(int i = 0; i < zvalues.size(); ++i) {
        for(int j = 0; j < zvalues[0].size(); ++j) {
            zvalues[i][j] = std::numeric_limits<double>::lowest();
        }
    }
}

screen::screen(unsigned long width, unsigned long height) : colorData{height, std::vector<std::tuple<short, short, short>>(width)}, zbuf{width, height} {}

bool screen::outbounds(int x, int y) {
    return x < 0 || x >= colorData[0].size() || y < 0 || y >= colorData.size();
}

std::ostream& operator<<(std::ostream& out, const screen& s) {
    out << "P3\n";
    out << s.colorData.size() << " " << s.colorData[0].size() << "\n";
    out << 255 << "\n";

    if(s._invert) {
        for(int i = s.colorData.size() - 1; i >= 0; --i) {
            for(int j = 0; j < s.colorData[i].size(); ++j) {
                short red, green, blue;
                std::tie(red, green, blue) = s.colorData[i][j];
                out << red << " " << green << " " << blue << (j == s.colorData[i].size() - 1 ? "\n" : " ");
            }
        }
    } else {
        for(int i = 0; i < s.colorData.size(); ++i) {
            for(int j = 0; j < s.colorData[i].size(); ++j) {
                short red, green, blue;
                std::tie(red, green, blue) = s.colorData[i][j];
                out << red << " " << green << " " << blue << (j == s.colorData[i].size() - 1 ? "\n" : " ");
            }
        }
    }
    return out;
}

void screen::invert(bool value) {
    _invert = value;
}

std::vector<std::tuple<short, short, short>>& screen::operator[](int index) {
    return colorData[index];
}

std::tuple<short, short, short> screen::get(int row, int col) const {
    if(row < 0 || row >= colorData.size() || col < 0 || col >= colorData[0].size()) {
        throw std::invalid_argument("Requested screen element out of bounds");
    }
    return colorData[row][col];
}

void screen::set(int row, int col, const std::tuple<short, short, short>& new_color) {
    if(row < 0 || row >= colorData.size() || col < 0 || col >= colorData[0].size()) {
        throw std::invalid_argument("Requested screen element out of bounds");
    }
    colorData[row][col] = new_color;
}

void screen::clear() {
    for(int i = 0; i < colorData.size(); ++i) {
        for(int j = 0; j < colorData[i].size(); ++j) {
            colorData[i][j] = {0, 0, 0};
        }
    }
    zbuf = zbuffer(colorData.size(), colorData[0].size());
}

void screen::drawMatrix(const edge_matrix& edges, const std::tuple<short, short, short>& color) {
    outbounds_message = true;
    for(int i = 0; i < edges.width() - 1; i += 2) {
        drawLine({edges.get(0, i), edges.get(1, i), edges.get(2, i)}, {edges.get(0, i + 1), edges.get(1, i + 1), edges.get(2, i + 1)}, color);
    }
}

bool screen::include_cull(const std::tuple<double, double, double>& a, const std::tuple<double, double, double>& b, const std::tuple<double, double, double>& c) {
    /*
    vector3D A{b - a}, B{c - a}; // Two vectors along edges of triangle
    vector3D N = vector3D::cross(A, B); // Normal
    vector3D V{0, 0, 1}; // View vector (normalized)
    */
    return vector3D::dot(vector3D::cross(b - a, c - a), vector3D{0, 0, 1}) > 0; // Dot product gives cosine
}

void screen::drawMatrix(const polygon_matrix& polygons, const std::tuple<short, short, short>& color, const std::tuple<short, short, short>& fill) {
    outbounds_message = true;
    for(int i = 0; i < polygons.width() - 2; i += 3) {
        if(include_cull(
            {polygons.get(0, i), polygons.get(1, i), polygons.get(2, i)},
            {polygons.get(0, i + 1), polygons.get(1, i + 1), polygons.get(2, i + 1)},
            {polygons.get(0, i + 2), polygons.get(1, i + 2), polygons.get(2, i + 2)}
        )) {
            // Sort by bottom, middle, top
            std::tuple<double, double, double> points[3] = {
                {polygons.get(0, i), polygons.get(1, i), polygons.get(2, i)},
                {polygons.get(0, i + 1), polygons.get(1, i + 1), polygons.get(2, i + 1)},
                {polygons.get(0, i + 2), polygons.get(1, i + 2), polygons.get(2, i + 2)}
            };
            std::sort(points, points + 3, 
                [](const std::tuple<double, double, double>& a, const std::tuple<double, double, double>& b) -> bool {
                    return std::get<1>(a) < std::get<1>(b);
                }
            );

            std::tuple<short, short, short> randColor = {rand() % 255, rand() % 255, rand() % 255};
            // Iterating from bottom to mid
            int curr_y = std::get<1>(points[0]),
                mid_y = std::get<1>(points[1]),
                max_y = std::get<1>(points[2]);
            double x_bt = std::get<0>(points[0]),
                   dx0 = (std::get<0>(points[2]) - std::get<0>(points[0])) / (std::get<1>(points[2]) - std::get<1>(points[0])),
                   x_bmt = std::get<0>(points[0]),
                   dx1 = (std::get<0>(points[1]) - std::get<0>(points[0])) / (std::get<1>(points[1]) - std::get<1>(points[0])),
                   dx2 = (std::get<0>(points[2]) - std::get<0>(points[1])) / (std::get<1>(points[2]) - std::get<1>(points[1]));
            double z_bt = std::get<2>(points[0]),
                   z_bmt = std::get<2>(points[0]),
                   dz0 = (std::get<2>(points[2]) - std::get<2>(points[0])) / (std::get<1>(points[2]) - std::get<1>(points[0])),
                   dz1 = (std::get<2>(points[1]) - std::get<2>(points[0])) / (std::get<1>(points[1]) - std::get<1>(points[0])),
                   dz2 = (std::get<2>(points[2]) - std::get<2>(points[1])) / (std::get<1>(points[2]) - std::get<1>(points[1]));
            curr_y++;
            for(; curr_y <= mid_y; curr_y++) {
                drawScanLine(x_bt, x_bmt, curr_y, z_bt, z_bmt, randColor); // Replace with drawScanLine
                x_bt += dx0;
                x_bmt += dx1;
                z_bt += dz0;
                z_bmt += dz1;
            }
            x_bmt = std::get<0>(points[1]);
            z_bmt = std::get<2>(points[1]);
            for(; curr_y <= max_y; curr_y++) {
                drawScanLine(x_bt, x_bmt, curr_y, z_bt, z_bmt, randColor);
                x_bt += dx0;
                x_bmt += dx2;
                z_bt += dz0;
                z_bmt += dz2;
            }

            // Edges 
            
            drawLine({polygons.get(0, i), polygons.get(1, i), polygons.get(2, i)}, {polygons.get(0, i + 1), polygons.get(1, i + 1), polygons.get(2, i + 1)}, color);
            drawLine({polygons.get(0, i + 1), polygons.get(1, i + 1),  polygons.get(2, i + 1)}, {polygons.get(0, i + 2), polygons.get(1, i + 2), polygons.get(2, i + 2)}, color);
            drawLine({polygons.get(0, i + 2), polygons.get(1, i + 2),  polygons.get(2, i + 2)}, {polygons.get(0, i), polygons.get(1, i), polygons.get(2, i)}, color);
            
        }
    }
}

void screen::drawLine(const std::tuple<int, int, double>& a, const std::tuple<int, int, double>& b, const std::tuple<short, short, short>& color) {
    int dx = (std::get<0>(b) - std::get<0>(a)), dy = (std::get<1>(b) - std::get<1>(a));
    double dz = (std::get<2>(b) - std::get<2>(a));
    bool drawnOff = false;

    // Gentler slopes
    if(abs(dx) >= abs(dy)) {
        
        int max_x, x, y;
        double z, dzppx;
        if(std::get<0>(a) <= std::get<0>(b)) {
            std::tie(x, y, z) = a;
            max_x = std::get<0>(b);
        } else {
            std::tie(x, y, z) = b;
            max_x = std::get<0>(a);
            dx *= -1;
            dy *= -1;
            dz *= -1;
        }
        dzppx = dz / (max_x - x + 1);

        // Octant 1 and 5
        if(dy >= 0) {
            int midCompare = 2 * dy - dx; // 2A + B
            while(x <= max_x) {
                if(outbounds(x, y)) {
                    if(!drawnOff) {
                        if(outbounds_message) {
                            std::cout << "Warning: drawing off screen\n";
                            outbounds_message = false;
                        }
                        drawnOff = true;
                    } 
                } else if (z > zbuf[y][x]) {
                    colorData[y][x] = color;
                    zbuf[y][x] = z;
                }
                if(midCompare >= 0) {
                    ++y;
                    midCompare -= 2 * dx;
                }
                midCompare += 2 * dy;
                ++x;
                z += dzppx;
            }
        // Octant 4 and 8
        } else {
            int midCompare = 2 * dy + dx; // 2A - B
            while(x <= max_x) {
                if(outbounds(x, y)) {
                    if(!drawnOff) {
                        if(outbounds_message) {
                            std::cout << "Warning: drawing off screen\n";
                            outbounds_message = false;
                        }
                        drawnOff = true;
                    } 
                } else if (z > zbuf[y][x]) {
                    colorData[y][x] = color;
                    zbuf[y][x] = z;
                }
                if(midCompare <= 0) {
                    --y;
                    midCompare += 2 * dx;
                }
                midCompare += 2 * dy;
                ++x;
                z += dzppx;
            }
        }
    
    // Steeper slopes
    } else {

        int max_y, x, y;
        double z, dzppx;
        if(std::get<1>(a) <= std::get<1>(b)) {
            std::tie(x, y, z) = a;
            max_y = std::get<1>(b);
        } else {
            std::tie(x, y, z) = b;
            max_y = std::get<1>(a);
            dx *= -1;
            dy *= -1;
            dz *= -1;
        }
        dzppx = dz / (max_y - y + 1);

        // Octant 2 and 6
        if(dx >= 0) {
            int midCompare = dy - 2 * dx; // A + 2B
            while(y <= max_y) {
                if(outbounds(x, y)) {
                    if(!drawnOff) {
                        if(outbounds_message) {
                            std::cout << "Warning: drawing off screen\n";
                            outbounds_message = false;
                        }
                        drawnOff = true;
                    } 
                } else if (z > zbuf[y][x]) {
                    colorData[y][x] = color;
                    zbuf[y][x] = z;
                }
                if(midCompare <= 0) {
                    ++x;
                    midCompare += 2 * dy;
                }
                midCompare -= 2 * dx;
                ++y;
                z += dzppx;
            }
        // Octant 3 and 7
        } else {
            int midCompare = dy + 2 * dx; // A - 2B
            while(y <= max_y) {
                if(outbounds(x, y)) {
                    if(!drawnOff) {
                        if(outbounds_message) {
                            std::cout << "Warning: drawing off screen\n";
                            outbounds_message = false;
                        }
                        drawnOff = true;
                    } 
                } else if (z > zbuf[y][x]) {
                    colorData[y][x] = color;
                    zbuf[y][x] = z;
                }
                if(midCompare >= 0) {
                    --x;
                    midCompare -= 2 * dy;
                }
                midCompare -= 2 * dx;
                ++y;
                z += dzppx;
            }
        }
    }

}

void screen::drawScanLine(int x0, int x1, int y, double z0, double z1, const std::tuple<short, short, short>& color) {
    int left_x, right_x;
    double left_z, right_z; 
    if(x0 <= x1) {
        left_x = x0, right_x = x1;
        left_z = z0, right_z = z1;
    } else {
        left_x = x1, right_x = x0;
        left_z = z1, right_z = z0;
    }
    double dz = (right_z - left_z) / (right_x - left_x + 1);
    double curr_z = left_z + dz;
    for(int i = left_x; i <= right_x; ++i) {
        if(curr_z > zbuf[y][i]) {
            colorData[y][i] = color;
            zbuf[y][i] = curr_z;
        }
        curr_z += dz;
    }
}