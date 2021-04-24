#pragma once

class screen;

#include "matrix.hpp"
#include <ostream>
#include <tuple>

class screen {
    private:
        std::vector<std::vector<std::tuple<short, short, short>>> colorData;
        bool _invert = false;
        bool outbounds(int x, int y);
    public:
        screen(int width, int height);
        /**
         * Prints out contents of screen in a PPM ASCII format.
         **/
        friend std::ostream& operator<<(std::ostream& out, const screen& s);

        /**
         * Inverts the screen vertically so you can have nice smiley face.
         * By default, y increases as the screen goes down. If invert is true, just flips the screen when drawing.
         **/
        void invert(bool value);

        /**
         * Reference to a point
         **/
        std::vector<std::tuple<short, short, short>>& operator[](int index);

        /**
         * Get the color of a point
         **/
        std::tuple<short, short, short> get(int row, int col) const;

        /**
         * Set the color of a point
         **/
        void set(int row, int col, const std::tuple<short, short, short>& new_color);

        /**
         * Fills the screen with black
         **/
        void clear();

        /**
         * Draws edge matrix to a screen.  Currently only takes in the x and y coordinates.
         **/
        void drawMatrix(const edge_matrix& edges, const std::tuple<short, short, short>& color);

        /**
         * Draws a line from point a to point b. Keep in mind that these points are not (row, col), but (x, y). 
         **/ 
        void drawLine(const std::pair<int, int>& a, const std::pair<int, int>& b, const std::tuple<short, short, short>& color);
};