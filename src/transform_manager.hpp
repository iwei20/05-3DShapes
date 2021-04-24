#pragma once

class transform_manager;

#include "matrix.hpp"

class transform_manager {
    private:
        matrix m;
    public:
        /**
         * Creates an object that holds a transformation matrix, initialized to an identity matrix, which can be operated on.
         **/
        transform_manager();

        /**
         * Applies a translation to the matrix.
         **/
        void translate(double x_units, double y_units, double z_units);

        /**
         * Applies a dilation to the matrix.
         **/
        void dilate(double x_scale, double y_scale, double z_scale);

        /**
         * Applies a rotation around the x-axis to the matrix.
         **/
        void xRot(double degrees);

        /**
         * Applies a rotation around the y-axis to the matrix.
         **/
        void yRot(double degrees);

        /**
         * Applies a rotation around the z-axis to the matrix.
         **/
        void zRot(double degrees);

        /**
         * Returns a reference to the matrix.
         **/
        const matrix& get_matrix();

        /**
         * Replaces the matrix reference with a new identity matrix.
         **/
        void reset();
};