#ifndef VECTOR_3D_H
#define VECTOR_3D_H

#include <cmath>
#include <exception>

class Vector3D {
    private:
        float _x, _y, _z, magnitude;
        void calculateMagnitude() {
            magnitude = std::sqrt(_x * _x + _y * _y + _z * _z);
        }
    public:
        Vector3D(float x, float y, float z) : _x(x), _y(y), _z(z) {
            calculateMagnitude();
        }

        Vector3D() : _x(0), _y(0), _z(0), magnitude(0) {}

        Vector3D unitVector() const {
            if (magnitude == 0) 
                return Vector3D(); 
            return Vector3D( 
                _x/magnitude, 
                _y/magnitude, 
                _z/magnitude);
        }

        float get_x() const {return _x;}
        float get_y() const {return _y;}
        float get_z() const {return _z;}

        float get_magnitude() const {return magnitude;}

        void set_x(float value) {
            _x = value;
            calculateMagnitude();
        }

        void set_y(float value) {
            _y = value;
            calculateMagnitude();
        }


        void set_z(float value) {
            _z = value;
            calculateMagnitude();
        }

        /* OPERATIONS */

        // Add
        Vector3D operator+(const Vector3D& other) const {
            return Vector3D(
                _x + other.get_x(),
                _y + other.get_y(),
                _z + other.get_z()
            );
        }

        // Subrtract
        Vector3D operator-(const Vector3D& other) const {
            return Vector3D(
                _x - other.get_x(),
                _y - other.get_y(),
                _z - other.get_z()
            );
        }

        Vector3D operator-(const float other) const {
            return Vector3D(
                _x * other,
                _y * other,
                _z * other
            );
        }

        Vector3D operator-=(const Vector3D& other) {
            _x = _x - other.get_x();
            _y = _y - other.get_y(),
            _z = _z - other.get_z()
        }


        // Multuply By Scalar

        // Dot Product
        Vector3D dotProcut(const Vector3D& other) const{
            return Vector3D(
                _x * other->get_x(), 
                _y * other->get_y(), 
                _z * other->get_z()
            );
        } 

        // Cross Product
        Vector3D crossProduct(const Vector3D& other) const {
            return Vector3D(
                _y * other.get_z() - _z * other.get_y(),
                _z * other.get_x() - _x * other.get_y(),
                _x * other.get_y() - _y * other.get_z()
            );
        }

        float operator[](size_t index) {
            if (index > 2) throw "Maximum index in a vector3D is 2";
            switch (index) {
                case 0: return _x;
                case 1: return _y;
                case 2: return _z;
            }
        }
        
};

#endif