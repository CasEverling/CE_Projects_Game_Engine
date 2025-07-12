#ifndef VECTOR_2D_H
#define VECTOR_2D_H

class Vector2D {
    private:
        float _x, _y, _z, magnitude;
        void calculateMagnitude() {

        }
    public:
        Vector2D(float x, float y, float z) : _x(x), _y(y), _z(z);

        Vector2D unitVector() {
            return Vector2D( _x/magnitude, _y/magnitude, _z/magnitude);
        };

        float get_x() {return _x;}
        float get_y() {return _y;}
        float get_z() {return _z;}

        float set_x(float value) {
            _x = value;
            calculateMagnitude();
        }

        float set_y(float value) {
            _y = value;
            calculateMagnitude();
        }


        float set_z(float value) {
            _z = value;
            calculateMagnitude();
        }

        /* OPERATIONS */

        // Add
        Vector2D operator+(const Vector2D& other) const {
            return Vector2D(
                
            )
        }
        // Multuply By Scalar

        // Dot Product
        Vector2D dotProcut(Vector2D* other) {
            return Vector2D(
                _x * other->get_x(), 
                _y * other->get_y(), 
                _z * other->get_z());
        } 

        // Cross Product
        Vector2D crossProduct(Vector2D* other) {
            return Vector2D(
                _y * other->get_z() - _z * other->get_y(),
                _z * other->get_x() - _x * other->get_y(),
                _x * other->get_y() - _y * other->get_z()
            );
        }
        
};

#endif