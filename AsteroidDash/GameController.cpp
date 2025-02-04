#include "GameController.h"
#include <fstream>
#include <sstream>      
#include <string>       
#include <iostream>     
#include <stdexcept>
#include <ostream>
#include <ctime>
#include <chrono>


// Simply instantiates the game
GameController::GameController(
        const string &space_grid_file_name,
        const string &celestial_objects_file_name,
        const string &leaderboard_file_name,
        const string &player_file_name,
        const string &player_name

) {
    game = new AsteroidDash(space_grid_file_name, celestial_objects_file_name, leaderboard_file_name, player_file_name,player_name);
    // TODO: Your code here, if you want to perform extra initializations
    
}

// Reads commands from the given input file, executes each command in a game tick
void GameController::play(const string &commands_file) {
    // TODO: Your code here
    std::ifstream file(commands_file);
    if (!file.is_open()) {
        std::cerr << "Could not open file." << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        if(game->game_over==false){
            
            game->update_space_grid();
            if (line == "PRINT_GRID") {
                game->print_space_grid();
            } else if (line == "MOVE_UP") {
                game->player->move_up();
            } else if (line == "MOVE_DOWN") {
                game->player->move_down(game->space_grid.size());
            }else if (line == "MOVE_RIGHT") {
                game->player->move_right(game->space_grid[0].size());
            }else if (line == "MOVE_LEFT") {
                game->player->move_left();
            }else if (line == "SHOOT") {
                game->shoot();
            }else if (line == "NOP") {
                
            } else {
                std::cerr << "Unknown command: " << line << std::endl;
            }
            

            game->game_time++;
            game->current_score++;
        }
        else{
            auto now = std::chrono::system_clock::now();

            std::time_t now_c = std::chrono::system_clock::to_time_t(now);

            std::cout<<"GAME OVER!"<<std::endl;
            auto *new_entry = new LeaderboardEntry(game->current_score, now_c, game->player->player_name);
            game->leaderboard.insert(new_entry);
            game->print_game_over(true);
            game->leaderboard.print_leaderboard();
            game->leaderboard.write_to_file(game->leaderboard_file_name);
            break;
        }
    }
    
    auto now = std::chrono::system_clock::now();

    std::time_t now_c = std::chrono::system_clock::to_time_t(now);

    std::cout<<"GAME FINISHED! No more commands!"<<std::endl;
    game->game_time--;
    game->current_score--;
    auto *new_entry = new LeaderboardEntry(game->current_score, now_c, game->player->player_name);
    game->leaderboard.insert(new_entry);
    game->print_game_over(false);
    game->leaderboard.print_leaderboard();
    game->leaderboard.write_to_file(game->leaderboard_file_name);

    file.close();
    return;
}

// Destructor to delete dynamically allocated member variables here
GameController::~GameController() {
    // TODO: Your code here
    if (game != nullptr) {
        delete game; 
        game = nullptr;
    }
    
}
