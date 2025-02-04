#ifndef CELESTIALOBJECT_H
#define CELESTIALOBJECT_H

#include <vector>
#include <iostream>
#include <ostream>


using namespace std;

// Enum to represent the type of the object (asteroid, life-up, or ammo)
enum ObjectType {
    ASTEROID = 0,
    LIFE_UP = 1,
    AMMO = 2
};

class CelestialObject {
public:

    void print() const {
        std::cout << "Type: " << object_type << ", s: " << starting_row << " t: " << time_of_appearance << "\nShape:\n";
        for (const auto& row : shape) {
            for (bool cell : row) {
                std::cout << (cell ? "1 " : "0 ");
            }
            std::cout << std::endl;
        }
    }

    // Constructor to initialize CelestialObject with essential properties
    CelestialObject(const vector<vector<bool>> &shape, ObjectType type, int start_row, int time_of_appearance);

    // Copy constructor for CelestialObject
    CelestialObject(const CelestialObject *other);

    // Shape of the object represented as a 2D boolean vector
    vector<vector<bool>> shape;

    // Pointer to the object's clockwise neighbor (its right rotation)
    CelestialObject *right_rotation = nullptr;

    // Pointer to the object's clockwise neighbor (its left rotation)
    CelestialObject *left_rotation = nullptr;

    // Pointer to the next celestial object in the list
    CelestialObject *next_celestial_object = nullptr;

    // Type of the object (asteroid, life-up, or ammo)
    ObjectType object_type;

    // Row in the space grid where the object's top will appear
    int starting_row;

    int starting_column_from_right = 0;

    // The step in the game after which the object will appear on the grid
    int time_of_appearance;

    bool is_same_shape(const vector<vector<bool>>& shape1, const vector<vector<bool>>& shape2);
    vector<vector<bool>> rotate_right(const vector<vector<bool>>& shape);

    // Function to delete rotations of a given celestial object. It should free the dynamically allocated
    // space for each rotation.
    static void delete_rotations(CelestialObject *target);
    int total_occupied_cell=0;
};

#endif // CELESTIALOBJECT_H
