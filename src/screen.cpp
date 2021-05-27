#include "screen.hpp"

#include "vector3D.hpp"
#include <iostream>
#include <algorithm>

zbuffer::zbuffer(unsigned long width, unsigned long height) : zvalues{height, std::vector<double>(width)} {}
std::vector<double>& zbuffer::operator[](int index) {
    return zvalues[index];
}

screen::screen(unsigned long width, unsigned long height) : colorData{height, std::vector<std::tuple<short, short, short>>(width)}, z{width, height} {}

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
}

void screen::drawMatrix(const edge_matrix& edges, const std::tuple<short, short, short>& color) {
    outbounds_message = true;
    for(int i = 0; i < edges.width() - 1; i += 2) {
        drawLine({edges.get(0, i), edges.get(1, i)}, {edges.get(0, i + 1), edges.get(1, i + 1)}, color, z);
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
            // Edges 
            drawLine({polygons.get(0, i), polygons.get(1, i)}, {polygons.get(0, i + 1), polygons.get(1, i + 1)}, color, z);
            drawLine({polygons.get(0, i + 1), polygons.get(1, i + 1)}, {polygons.get(0, i + 2), polygons.get(1, i + 2)}, color, z);
            drawLine({polygons.get(0, i + 2), polygons.get(1, i + 2)}, {polygons.get(0, i), polygons.get(1, i)}, color, z);
            
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

            // Iterating from bottom to mid
            int curr_y = std::get<1>(points[0]),
                mid_y = std::get<1>(points[1]),
                max_y = std::get<1>(points[2]);
            std::cout << curr_y << " " << mid_y << " " << max_y << "\n";
            double x_bt = std::get<0>(points[0]),
                   dx0 = (std::get<0>(points[2]) - std::get<0>(points[0])) / (std::get<1>(points[2]) - std::get<1>(points[0])),
                   x_bmt = std::get<0>(points[0]),
                   dx1 = (std::get<0>(points[1]) - std::get<0>(points[0])) / (std::get<1>(points[1]) - std::get<1>(points[0])),
                   dx2 = (std::get<0>(points[2]) - std::get<0>(points[1])) / (std::get<1>(points[2]) - std::get<1>(points[1]));
            for(; curr_y < mid_y; ++curr_y) {
                drawLine({x_bt, curr_y}, {x_bmt, curr_y}, fill, z); // Replace with drawScanLine
                x_bt += dx0;
                x_bmt += dx1;
            }
            x_bmt = std::get<0>(points[1]);
            for(; curr_y < max_y; ++curr_y) {
                drawLine({x_bt, curr_y}, {x_bmt, curr_y}, fill, z);
                x_bt += dx0;
                x_bmt += dx2;
            }

        }
    }
}

void screen::drawLine(const std::pair<int, int>& a, const std::pair<int, int>& b, const std::tuple<short, short, short>& color, zbuffer& zbuf) {
    int dx = (b.first - a.first), dy = (b.second - a.second);
    bool drawnOff = false;

    // Gentler slopes
    if(abs(dx) >= abs(dy)) {
        
        int max_x, x, y;
        if(a.first <= b.first) {
            std::tie(x, y) = a;
            max_x = b.first;
        } else {
            std::tie(x, y) = b;
            max_x = a.first;
            dx = -1 * dx;
            dy = -1 * dy;
        }

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
                } else {
                    colorData[y][x] = color;
                }
                if(midCompare >= 0) {
                    ++y;
                    midCompare -= 2 * dx;
                }
                midCompare += 2 * dy;
                ++x;
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
                } else {
                    colorData[y][x] = color;
                }
                if(midCompare <= 0) {
                    --y;
                    midCompare += 2 * dx;
                }
                midCompare += 2 * dy;
                ++x;
            }
        }
    
    // Steeper slopes
    } else {

        int max_y, x, y;
        if(a.second <= b.second) {
            std::tie(x, y) = a;
            max_y = b.second;
        } else {
            std::tie(x, y) = b;
            max_y = a.second;
            dx = -1 * dx;
            dy = -1 * dy;
        }

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
                } else {
                    colorData[y][x] = color;
                }
                if(midCompare <= 0) {
                    ++x;
                    midCompare += 2 * dy;
                }
                midCompare -= 2 * dx;
                ++y;
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
                } else {
                    colorData[y][x] = color;
                }
                if(midCompare >= 0) {
                    --x;
                    midCompare -= 2 * dy;
                }
                midCompare -= 2 * dx;
                ++y;
            }
        }
    }

}

void screen::drawScanLine(double x0, double x1, double y, const std::tuple<short, short, short>& color, zbuffer& zbuf) {

}