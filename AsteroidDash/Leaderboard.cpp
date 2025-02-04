#include "Leaderboard.h"
#include <iostream>  
#include <fstream> 

// Read the stored leaderboard status from the given file such that the "head_leaderboard_entry" member
// variable will point to the highest all-times score, and all other scores will be reachable from it
// via the "next_leaderboard_entry" member variable pointer.
void Leaderboard::read_from_file(const string &filename) {
    // TODO: Your code here
    ifstream file(filename);
    if (!file.is_open()) {
        return;
    }

    unsigned long score;
    time_t last_played;
    string player_name;

    LeaderboardEntry *current = nullptr;

    while (file >> score >> last_played >> player_name) {
        auto *new_entry = new LeaderboardEntry(score, last_played, player_name);
        if (!head_leaderboard_entry) {
            head_leaderboard_entry = new_entry;
        } else {
            current->next = new_entry;
        }

        current = new_entry;
    }

    file.close();
}


// Write the latest leaderboard status to the given file in the format specified in the PA instructions
void Leaderboard::write_to_file(const string &filename) {
    // TODO: Your code here
    
    ofstream file(filename, ios::trunc);
    if (!file.is_open()) {
        cerr << "Unable to open file: " << filename << endl;
        return;
    }

    LeaderboardEntry *current = head_leaderboard_entry;

    while (current != nullptr) {
        file << current->score << " "
             << current->last_played << " "
             << current->player_name << "\n";
        current = current->next;
    }

    file.close();

}

// Print the current leaderboard status to the standard output in the format specified in the PA instructions
void Leaderboard::print_leaderboard() {
    // TODO: Your code here
    cout << "Leaderboard" << endl;
    cout << "----------" << endl;

    LeaderboardEntry *current = head_leaderboard_entry;
    int count = 1;

    while (current != nullptr) {
        char time_buffer[20];
        strftime(time_buffer, sizeof(time_buffer), "%H:%M:%S/%d.%m.%Y", localtime(&current->last_played));

        cout << count << ". " << current->player_name << " " << current->score << " " << time_buffer << endl;
        current = current->next;
        count++;
    }
}

//  Insert a new LeaderboardEntry instance into the leaderboard, such that the order of the high-scores
//  is maintained, and the leaderboard size does not exceed 10 entries at any given time (only the
//  top 10 all-time high-scores should be kept in descending order by the score).
void Leaderboard::insert(LeaderboardEntry *new_entry) {
    // TODO: Your code here
    if (!head_leaderboard_entry || head_leaderboard_entry->score < new_entry->score) {
        new_entry->next = head_leaderboard_entry;
        head_leaderboard_entry = new_entry;
    } else {
        LeaderboardEntry *current = head_leaderboard_entry;
        while (current->next && current->next->score >= new_entry->score) {
            current = current->next;
        }
        new_entry->next = current->next;
        current->next = new_entry;
    }

    LeaderboardEntry *current = head_leaderboard_entry;
    int count = 1;

    while (current->next != nullptr) {
        if (++count == MAX_LEADERBOARD_SIZE + 1) {
            LeaderboardEntry *to_delete = current->next;
            current->next = nullptr;
            delete to_delete;
            break;
        }
        current = current->next;
    }
}

// Free dynamically allocated memory used for storing leaderboard entries
Leaderboard::~Leaderboard() {
    // TODO: Your code here
    LeaderboardEntry *current = head_leaderboard_entry;
    while (current != nullptr) {
        LeaderboardEntry *to_delete = current;
        current = current->next;
        delete to_delete;
    }
    
    
}
