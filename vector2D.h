#ifndef VECTOR_2D_H
#define VECTOR_2D_H

#include <stdexcept>

class Vector2D {
    private:
        float _x, _y, magnitude;
        void calculateMagnitude() {

        }
    public:
        Vector2D(float x, float y) : _x(x), _y(y) {}

        Vector2D unitVector() {
            return Vector2D( _x/magnitude, _y/magnitude);
        };

        float get_x() const {return _x;}
        float get_y() const {return _y;}

        float set_x(float value) {
            _x = value;
            calculateMagnitude();
        }

        float set_y(float value) {
            _y = value;
            calculateMagnitude();
        }

        float operator[](long long unsigned int index) {
            switch (index)
            {
                case 0  : return get_x();
                case 1  : return get_y();
                default : throw std::out_of_range("Index not in range [0,1]"); // Invalid Index
            }
        }

        /* OPERATIONS */

        // Add
        Vector2D operator+(const Vector2D& other) const {
            return Vector2D(
                _x + other.get_x(),
                _y + other.get_y()
            );
        }
        // Multuply By Scalar

        // Dot Product
        Vector2D dotProcut(Vector2D* other) {
            return Vector2D(
                _x * other->get_x(), 
                _y * other->get_y()
            );
        } 
        
};

#endif