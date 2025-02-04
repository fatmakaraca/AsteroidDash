#include "CelestialObject.h"
#define occupiedCellChar "██"
#define unoccupiedCellChar "▒▒"


// Constructor to initialize CelestialObject with essential properties
CelestialObject::CelestialObject(const vector<vector<bool>> &shape, ObjectType type, int start_row,
                                 int time_of_appearance)
        : shape(shape), object_type(type), starting_row(start_row), time_of_appearance(time_of_appearance) {
    // TODO: Your code here
    this->shape = shape;
    this->object_type = type;
    this->starting_row = start_row;
    this->time_of_appearance = time_of_appearance;

    for (const auto& row : shape) {
        for (bool cell : row) {
            if(cell==true){
                total_occupied_cell++;
            }
        }
    }

    CelestialObject* rotated_90_obj = new CelestialObject(*this);
    
    rotated_90_obj->shape = rotate_right(rotated_90_obj->shape);
    

    CelestialObject* rotated_180_obj = new CelestialObject(rotated_90_obj);
    
    rotated_180_obj->shape = rotate_right(rotated_180_obj->shape);
    
    
    CelestialObject* rotated_270_obj = new CelestialObject(rotated_180_obj);
    
    rotated_270_obj->shape = rotate_right(rotated_270_obj->shape);

    rotated_90_obj->next_celestial_object = this->next_celestial_object;
    rotated_180_obj->next_celestial_object = this->next_celestial_object;
    rotated_270_obj->next_celestial_object = this->next_celestial_object;

    this->right_rotation = this;
    this->left_rotation = this;

    if(!is_same_shape(this->shape, rotated_90_obj->shape)){
        this->right_rotation = rotated_90_obj;
        rotated_90_obj->left_rotation = this;
        this->left_rotation = rotated_90_obj;
        rotated_90_obj->right_rotation = this;
        //this->right_rotation->next_celestial_object=this->next_celestial_object;
        rotated_90_obj->next_celestial_object = this->next_celestial_object;
        
    }

    if(!is_same_shape(rotated_180_obj->shape, rotated_90_obj->shape) && !is_same_shape(rotated_180_obj->shape, this->shape)){
        rotated_90_obj->right_rotation = rotated_180_obj;
        rotated_180_obj->left_rotation = rotated_90_obj;
        rotated_180_obj->right_rotation = this;
        this->left_rotation = rotated_180_obj;
        //this->right_rotation->next_celestial_object=this->next_celestial_object;
        //this->right_rotation->right_rotation->next_celestial_object=this->next_celestial_object;
        rotated_180_obj->next_celestial_object = this->next_celestial_object;
        
    }

    if(!is_same_shape(rotated_270_obj->shape, rotated_180_obj->shape)&& !is_same_shape(rotated_270_obj->shape, rotated_90_obj->shape) && !is_same_shape(rotated_270_obj->shape, this->shape)){
        rotated_180_obj->right_rotation = rotated_270_obj;
        rotated_270_obj->left_rotation = rotated_180_obj;
        rotated_270_obj->right_rotation = this;
        this->left_rotation = rotated_270_obj;
        //this->right_rotation->next_celestial_object=this->next_celestial_object;
        //this->right_rotation->right_rotation->next_celestial_object=this->next_celestial_object;
        //this->right_rotation->right_rotation->right_rotation->next_celestial_object=this->next_celestial_object;
        rotated_270_obj->next_celestial_object = this->next_celestial_object;
        
    }

}


// Copy constructor for CelestialObject
CelestialObject::CelestialObject(const CelestialObject *other)
        : shape(other->shape),  // Copy the 2D vector shape
          object_type(other->object_type),  // Copy the object type
          starting_row(other->starting_row),  // Copy the starting row
          time_of_appearance(other->time_of_appearance)  // Copy the time of appearance
{
    // TODO: Your code here
    this->next_celestial_object=other->next_celestial_object;
    
}

bool CelestialObject::is_same_shape(const vector<vector<bool>>& shape1, const vector<vector<bool>>& shape2){
    if (shape1.size() != shape2.size()) {
        return false; 
    }
    for (size_t i = 0; i < shape1.size(); ++i) {
        if (shape1[i].size() != shape2[i].size()) {
            return false;  
        }

        for (size_t j = 0; j < shape1[i].size(); ++j) {
            if (shape1[i][j] != shape2[i][j]) {
                return false; 
            }
        }
    }
    return true;
}

vector<vector<bool>> CelestialObject::rotate_right(const vector<vector<bool>>& shape) {
    int rows = shape.size();
    int columns = shape[0].size();

    vector<vector<bool>> right_shape(columns, vector<bool>(rows));

    for(int i = 0; i < rows; ++i){
        for(int j= 0; j < columns; ++j){
            right_shape[j][rows - 1 - i] = shape[i][j];
        }
    }

    return right_shape;
}

// Function to delete rotations of a given celestial object. It should free the dynamically allocated
// memory for each rotation.
void CelestialObject::delete_rotations(CelestialObject *target) {
    // TODO: Your code here
    if (target == nullptr) return;

    // Deleting the right rotation if exists
    if (target->right_rotation != nullptr && target->right_rotation != target) {
        CelestialObject* right = target->right_rotation;
        target->right_rotation = right->right_rotation;
        right->right_rotation->left_rotation = target;

        delete right;
    }

    // Deleting the left rotation if exists
    if (target->left_rotation != nullptr && target->left_rotation != target) {
        CelestialObject* left = target->left_rotation;
        target->left_rotation = left->left_rotation;
        left->left_rotation->right_rotation = target;

        delete left;
    }
    
}