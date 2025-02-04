#include "AsteroidDash.h"
#include <fstream>      
#include <sstream>      
#include <string>       
#include <iostream>     
#include <stdexcept>
#include <ostream>
#include <limits>
#include <algorithm> 
#include <utility>   



using namespace std;


// Constructor to initialize AsteroidDash with the given parameters
AsteroidDash::AsteroidDash(const string &space_grid_file_name,
                           const string &celestial_objects_file_name,
                           const string &leaderboard_file_name,
                           const string &player_file_name,
                           const string &player_name)

        : leaderboard_file_name(leaderboard_file_name), leaderboard(Leaderboard()) {

    read_player(player_file_name, player_name);  // Initialize player using the player.dat file
    read_space_grid(space_grid_file_name);  // Initialize the grid after the player is loaded
    read_celestial_objects(celestial_objects_file_name);  // Load celestial objects
    leaderboard.read_from_file(leaderboard_file_name);
}



// Function to read the space grid from a file
void AsteroidDash::read_space_grid(const string &input_file) {
    // TODO: Your code here
    std::ifstream file(input_file);
    if (!file.is_open()) { 
        throw std::runtime_error("File cannot open!");
    }
    std::string line;
    while(getline(file, line)){
        vector<int> row;
        std::stringstream ss(line);

        int value;
        while (ss >> value) { 
            row.push_back(value);
        }

        space_grid.push_back(row);
    }
    file.close();

    
}

// Function to read the player from a file
void AsteroidDash::read_player(const string &player_file_name, const string &player_name) {
    // TODO: Your code here
    std::ifstream file(player_file_name);
    if (!file.is_open()) { 
        throw std::runtime_error("File cannot open!");
    }

    int line_number = 0;
    int p_row = 0;
    int p_col = 0;
    vector<vector<bool>> shape;

    std::string line;
    while(getline(file, line)){

        if (line_number == 0) {
            std::istringstream iss(line);
            iss >> p_row >> p_col;
        }
        else{
            std::vector<bool> row;
            std::stringstream ss(line);

            int value;
            while (ss >> value) { 
                row.push_back(value);
            }

            shape.push_back(row);
        }
        line_number++;
        
    }

    

    file.close();

    
    player = new Player(shape, p_row, p_col, player_name, 10, 3);
    
}

// Function to read celestial objects from a file
void AsteroidDash::read_celestial_objects(const string &input_file) {
    // TODO: Your code here
    std::ifstream file(input_file);
    if (!file.is_open()) { 
        throw std::runtime_error("File cannot open!");
    }

    std::string line;
    int s = 0, t = 0;
    std::string e = "";
    vector<vector<bool>> shape;
    CelestialObject *current = nullptr;
    bool new_celestial_object = false;


    while (getline(file, line)) {
        if (line.empty()) {
            
            if(new_celestial_object == true){
                
                CelestialObject* co = nullptr;
                
                if(e == "LIFE_UP"){
                    co = new CelestialObject(shape, LIFE_UP, s, t);
                    
                }
                else if(e == "AMMO"){
                    co = new CelestialObject(shape, AMMO, s, t);
                }
                else{
                    co = new CelestialObject(shape, ASTEROID, s, t);
                }
                if(celestial_objects_list_head == nullptr){
                    celestial_objects_list_head = co;
                    current = celestial_objects_list_head;
                }else{
                    current->next_celestial_object = co;
                    current = co;
                }
                
                shape.clear();
                e = "";
                s = 0;
                t = 0;
                new_celestial_object = false;
                continue;
            }
            else{
                continue;
            }
        }

        else if (line.front() == 's') {
            s = stoi(line.substr(2));
            continue;
        }
        else if (line.front() == 't'){
            t = stoi(line.substr(2));
            continue;
        }
        else if (line.front() == 'e'){
            if (line.substr(2) == "life") {
                e = "LIFE_UP";  
            } else if (line.substr(2) == "ammo") {
                e = "AMMO";  
            }
            continue;
        }
        

        string matrixLine = line;
        if (matrixLine.front() == '{' || matrixLine.front() == '[') {
            matrixLine = matrixLine.substr(1, matrixLine.size() - 1);
            new_celestial_object = true;
        }

        if (matrixLine.back() == '}' || matrixLine.back() == ']') {
        matrixLine = matrixLine.substr(0, matrixLine.size() - 1);
        }

        vector<bool> row;

        for(char c : matrixLine){
            row.push_back(c == '1');
        }
        
        shape.push_back(row);

    }


    if (new_celestial_object) {
        CelestialObject* co = nullptr;
        if (e == "LIFE_UP") {
            co = new CelestialObject(shape, LIFE_UP, s, t);
        } else if (e == "AMMO") {
            co = new CelestialObject(shape, AMMO, s, t);
        } else {
            co = new CelestialObject(shape, ASTEROID, s, t);
        }

        if (celestial_objects_list_head == nullptr) {
            celestial_objects_list_head = co;
            current=co;
        } else {
            current->next_celestial_object = co;
        }
    }
    
    file.close();
    
}


void AsteroidDash::print_celestial_objects() {
    CelestialObject* current = celestial_objects_list_head;

    
    while (current != nullptr) {
        
        std::cout << "Original:" << std::endl;
        current->print();
        std::cout << "--------------------" << std::endl;

        
        CelestialObject* rotated = current->right_rotation;
        int rotation_count = 90;  

        
        while (rotated != current) {  
            std::cout << "Rotated right " << rotation_count << " degrees:" << std::endl;
            rotated->print();
            std::cout << "--------------------" << std::endl;

            
            rotated = rotated->right_rotation;
            rotation_count += 90;
        }

        
        current = current->next_celestial_object;
    }
}

// Print the entire space grid
void AsteroidDash::print_space_grid() const {
    // TODO: Your code here
    if (player == nullptr) {
        std::cout << "Player object is not initialized!" << std::endl;
        return;
    }
    std::cout << "Tick: " << game_time << std::endl;
    std::cout << "Lives: " << player->lives << std::endl;
    std::cout << "Ammo: " << player->current_ammo << std::endl;
    std::cout << "Score: " << current_score << std::endl;
    unsigned long score = 0;
    if(leaderboard.head_leaderboard_entry != nullptr){
        score = leaderboard.head_leaderboard_entry->score;
    }
    std::cout << "High Score: "<< score << std::endl; 
    for (const auto& row : space_grid) {
        for (bool cell : row) {
            std::cout << (cell ? occupiedCellChar  : unoccupiedCellChar);
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    
}

void AsteroidDash::print_game_over(bool is_game_over){
    // TODO: Your code here
    if (player == nullptr) {
        std::cout << "Player object is not initialized!" << std::endl;
        return;
    }
    std::cout << "Tick: " << game_time << std::endl;
    std::cout << "Lives: " << player->lives << std::endl;
    std::cout << "Ammo: " << player->current_ammo << std::endl;
    std::cout << "Score: " << current_score << std::endl;
    unsigned long score = 0;
    if(leaderboard.head_leaderboard_entry != nullptr){
        score = leaderboard.head_leaderboard_entry->score;
    }
    std::cout << "High Score: "<< score << std::endl; 
    std::cout << "Player: "<< player->player_name << std::endl;

    if(is_game_over == true){
        for (int i = 0; i < player->spacecraft_shape.size(); i++) {
            for (int j = 0; j < player->spacecraft_shape[i].size(); j++) {
                if (player->spacecraft_shape[i][j] == true) {
                    space_grid[player->position_row + i][player->position_col + j] = 0; 
                }
            }
        }
    }

    for (const auto& row : space_grid) {
        for (bool cell : row) {
            std::cout << (cell ? occupiedCellChar  : unoccupiedCellChar);
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    
    
}

void AsteroidDash::resetSpaceGrid() {
    for (int i = 0; i < space_grid.size(); i++) {
        for (int j = 0; j < space_grid[i].size(); j++) {
            space_grid[i][j] = 0;
        }
    }
}

void AsteroidDash::placePlayerOnGrid() {
    for (int i = 0; i < player->spacecraft_shape.size(); i++) {
        for (int j = 0; j < player->spacecraft_shape[i].size(); j++) {
            if (player->spacecraft_shape[i][j] == true) {
                space_grid[player->position_row + i][player->position_col + j] = 1; 
            }
        }
    }
}

void AsteroidDash::placeBulletsOnGrid() {
    for (auto it = bullets.begin(); it != bullets.end();) {
        int x = it->first;
        int y = it->second;
        if (x >= 0 && x < space_grid.size() && y >= 0 && y < space_grid[0].size()) {
            space_grid[x][y] = 1;
            ++it; 
        } else {
            it = bullets.erase(it);
            ++it; 
        }
    }
}

// Function to update the space grid with player, celestial objects, and any other changes
// It is called in every game tick before moving on to the next tick.
void AsteroidDash::update_space_grid() {
    
   for (auto it = bullets.begin(); it != bullets.end();) {
        
        if(it->second == space_grid[0].size() - 1){
            
            it->second += 1;
            
            it = bullets.erase(it);
            continue;
        
        }
        else if(it->second < space_grid[0].size() - 1){
            
            if(space_grid[it->first][it->second+1]==false){
                
                it->second += 1; 
            }else if(space_grid[it->first][it->second+1]==true){
                
                bool increase = true;
                
                
                CelestialObject *current = celestial_objects_list_head;
                while(current!=nullptr){
                    if(game_time >= current->time_of_appearance){
                        int starting_column =  space_grid[0].size() - 1 - current->starting_column_from_right;
                        
                        for (int i = 0; i < current->shape.size(); i++) {
                            for (int j = 0; j < current->shape[i].size(); j++) { 
                                int index_i_space_grid = i + current->starting_row;
                                int index_j_space_grid= j + starting_column;
                                if(index_i_space_grid == it->first && index_j_space_grid == it->second + 1){
                                    
                                    if(current->object_type == 0){
                                        increase = false;
                                        
                                    }
                                }
                            }
                        }

                    }
                    
                    current = current->next_celestial_object;
                    
                }
                if(increase==true){
                    it->second += 1;
                }
            }
            ++it;
        }

    }
    

    resetSpaceGrid();
    placePlayerOnGrid();
    placeBulletsOnGrid();


    
    CelestialObject *current = celestial_objects_list_head;
    CelestialObject *prev = nullptr;
    while(current!=nullptr){
        bool is_collide_with_player = false;
        
        if(game_time >= current->time_of_appearance){
            int starting_column =  space_grid[0].size() - 1 - current->starting_column_from_right;
            for (int i = 0; i < current->shape.size(); i++) {
                for (int j = 0; j < current->shape[i].size(); j++) { 
                    
                    int index_i_space_grid = i + current->starting_row;
                    int index_j_space_grid= j + starting_column;
                    bool is_collide_with_bullet = false;
                    
                    if(index_i_space_grid < space_grid.size() && index_j_space_grid < space_grid[0].size()){
                        if (current->shape[i][j] == true) {
                            space_grid[index_i_space_grid][index_j_space_grid] = 1;

                            if(index_i_space_grid >= player->position_row && index_i_space_grid < player->position_row + player->spacecraft_shape.size() && index_j_space_grid >= player->position_col && index_j_space_grid < player->position_col + player->spacecraft_shape[0].size()){

                                if(player->spacecraft_shape[index_i_space_grid - player->position_row][index_j_space_grid - player->position_col] == true){
                                    
                                    is_collide_with_player = true;
                                    

                                    if(current->object_type == 0){
                                        player->lives--;
                                    }
                                    else if(current->object_type == 1){
                                        player->lives++;
                                    }
                                    else if(current->object_type == 2){
                                        player->current_ammo=player->max_ammo;
                                    }
                                    
                                    current = deleteCelestialObject(celestial_objects_list_head, current);
                                    if(player->lives == 0){
                                        game_over = true;
                                    }
                                    
                                }
                                
                            }
                            else{ 
                                if(current->object_type==0){
                                    for (auto it = bullets.begin(); it != bullets.end();) {
                                        int x = it->first;
                                        int y = it->second;

                                        if (x == index_i_space_grid && y== index_j_space_grid) {
                                            is_collide_with_bullet = true;
                                            if(has_only_one_true(current->shape)==true){
                                                current_score+=current->total_occupied_cell*100;
                                                
                                            }
                                            current_score +=10;
                                            current->shape[i][j] = false;
                                            it = bullets.erase(it); 

                                            if(current->shape.size() % 2 == 0){
                                                if(i < current->shape.size() / 2){
                                                    int first_total_occupied_cell=current->total_occupied_cell;
                                                    CelestialObject* new_current = new CelestialObject(rotate_right(current->shape), ASTEROID, current->starting_row, current->time_of_appearance);
                                                    new_current->starting_column_from_right = current->starting_column_from_right;
                                                    new_current->next_celestial_object=current->next_celestial_object;
                                                    new_current->total_occupied_cell=first_total_occupied_cell;
                                                    if(prev!=nullptr){
                                                        prev->next_celestial_object=new_current;
                                                        CelestialObject* temp = current;
                                                        current = new_current;
                                                        delete temp;
                                                        temp = nullptr;
                                                    }else{
                                                       
                                                        new_current->next_celestial_object=current->next_celestial_object;
                                                        CelestialObject* temp = current;
                                                        current = new_current;
                                                        delete temp;
                                                        celestial_objects_list_head = current;
                                                    }
                                                    
                                                }
                                                else if(i >= current->shape.size() / 2){
                                                    int first_total_occupied_cell=current->total_occupied_cell;
                                                    CelestialObject* new_current = new CelestialObject(rotate_right(rotate_right(rotate_right(current->shape))), ASTEROID, current->starting_row, current->time_of_appearance);
                                                    new_current->starting_column_from_right = current->starting_column_from_right;
                                                    new_current->next_celestial_object=current->next_celestial_object;
                                                    new_current->total_occupied_cell=first_total_occupied_cell;
                                                    if(prev!=nullptr){
                                                        prev->next_celestial_object=new_current;
                                                        CelestialObject* temp = current;
                                                        current = new_current;
                                                        delete temp;
                                                        temp = nullptr;
                                                    }else{
                            
                                                        new_current->next_celestial_object=current->next_celestial_object;
                                                        CelestialObject* temp = current;
                                                        current = new_current;
                                                        delete temp;
                                                        celestial_objects_list_head = current;
                                                    }
                                                   
                                                    
                                                }
                                            }else{
                                                if(i < current->shape.size() / 2){
                                                    int first_total_occupied_cell=current->total_occupied_cell;
                                                    CelestialObject* new_current = new CelestialObject(rotate_right(current->shape), ASTEROID, current->starting_row, current->time_of_appearance);
                                                    new_current->starting_column_from_right = current->starting_column_from_right;
                                                    new_current->next_celestial_object=current->next_celestial_object;
                                                    new_current->total_occupied_cell=first_total_occupied_cell;
                                                    if(prev!=nullptr){
                                                        prev->next_celestial_object=new_current;
                                                        CelestialObject* temp = current;
                                                        current = new_current;
                                                        delete temp;
                                                        temp = nullptr;
                                                    }else{
                                                        new_current->next_celestial_object=current->next_celestial_object;
                                                        CelestialObject* temp = current;
                                                        current = new_current;
                                                        delete temp;
                                                        celestial_objects_list_head = current;
                                                    }
                                                }
                                                else if(i > current->shape.size() / 2){
                                                    int first_total_occupied_cell=current->total_occupied_cell;
                                                    CelestialObject* new_current = new CelestialObject(rotate_right(rotate_right(rotate_right(current->shape))), ASTEROID, current->starting_row, current->time_of_appearance);
                                                    new_current->starting_column_from_right = current->starting_column_from_right;
                                                    new_current->next_celestial_object=current->next_celestial_object;
                                                    new_current->total_occupied_cell=first_total_occupied_cell;
                                                    if(prev!=nullptr){
                                                        prev->next_celestial_object=new_current;
                                                        CelestialObject* temp = current;
                                                        current = new_current;
                                                        delete temp;
                                                        temp = nullptr;
                                                    }else{
                                                        new_current->next_celestial_object=current->next_celestial_object;
                                                        CelestialObject* temp = current;
                                                        current = new_current;
                                                        delete temp;
                                                        celestial_objects_list_head = current;
                                                    }
                                                }

                                            }

                                            resetSpaceGrid();
                                            placePlayerOnGrid();
                                            placeBulletsOnGrid();
                                        }
                                        else {
                                            ++it; 
                                        }
                                    }
                                }
                            }
                            
                        }    
                    }
                    if(is_collide_with_player==true){break;}
                }
                if(is_collide_with_player==true){break;}
            }
            
            if(!is_collide_with_player){
                current->starting_column_from_right++;
            }
        }
        
        if(!is_collide_with_player){
            prev = current;
            current = current->next_celestial_object;
        }
        if(is_collide_with_player){
            resetSpaceGrid();
            placePlayerOnGrid();
            
            continue;
        }
        
        
    }

    
}
vector<vector<bool>> AsteroidDash::rotate_right(const vector<vector<bool>>& shape) {
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

bool AsteroidDash::has_only_one_true(const std::vector<std::vector<bool>>& shape) {
    int trueCount = 0;
    for (const auto& row : shape) {
        for (bool cell : row) {
            if (cell) {
                trueCount++;
                if (trueCount > 1) {
                    return false;
                }
            }
        }
    }
    return trueCount == 1;
}


CelestialObject* AsteroidDash::deleteCelestialObject(CelestialObject*& head, CelestialObject*& target) {
    if (!head || !target) return nullptr; 

    if (head == target) { 
        CelestialObject* temp = head;
        head = head->next_celestial_object; 
        delete temp; 
        return head;
    }

    CelestialObject* current = head;
    CelestialObject* prev = nullptr;

    while (current) {
        if (current == target) { 
            prev->next_celestial_object = current->next_celestial_object; 
            delete current; 
            return current->next_celestial_object;
        }
        prev = current;
        current = current->next_celestial_object;
    }
    return nullptr;
}


// Corresponds to the SHOOT command.
// It should shoot if the player has enough ammo.
// It should decrease the player's ammo
void AsteroidDash::shoot() {
    // TODO: Your code here
    if(player->current_ammo > 0){
        int x = player->position_row + (player->spacecraft_shape.size() / 2);
        int y = player->position_col + player->spacecraft_shape[0].size();

        bullets.push_back(make_pair(x, y-1));
        player->current_ammo--;
    }
}


// Destructor. Remove dynamically allocated member variables here.
AsteroidDash::~AsteroidDash() {
    // TODO: Your code here
    if (player != nullptr) {
        delete player;
        player = nullptr;
    }

    CelestialObject* current = celestial_objects_list_head;
    while (current != nullptr) {
        CelestialObject* temp = current;
        current = current->next_celestial_object;
        CelestialObject::delete_rotations(temp); // Rotasyonları sil
        delete temp; // Nesneyi serbest bırak
    }
    celestial_objects_list_head = nullptr;
    
}