#pragma once

class parser;

#include "screen.hpp"
#include "matrix.hpp"
#include "transform_manager.hpp"
#include <istream>

class parser {
    private:
        edge_matrix _e;
        transform_manager _t;
    public:
        /**
         * Reads through and performs all operations from a stream and performs them on screen s.
         **/
        void parse(screen& s, std::istream& in);
};