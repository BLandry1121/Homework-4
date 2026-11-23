
#include <iostream>
#include <cstdlib>
#include <string>
#include <climits>
#include "bowling_frame.h"

using namespace std;

int current_game_total(Bowling_Frame* first_frame)
{
    Bowling_Frame* temp = first_frame;
    int game_total = 0;

    while (temp != nullptr)
    {
        game_total += temp->frame_score();
        temp = temp->next_frame();
    }

    return game_total;
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        cout << "Usage: " << argv[0]
             << " <num_players>" << endl;
        return 1;
    }

    string argument = argv[1];

    if (argument.empty())
    {
        cout << "Error: Enter a valid number of players."
             << endl;
        return 1;
    }

    for (char c : argument)
    {
        if (c < '0' || c > '9')
        {
            cout << "Error: Number of players must be "
                 << "a positive integer." << endl;
            return 1;
        }
    }

    int num_players = 0;

    try
    {
        size_t used = 0;
        unsigned long count = stoul(argument, &used);

        if (used != argument.size() ||
            count == 0 || count > INT_MAX)
        {
            cout << "Error: Invalid number of players."
                 << endl;
            return 1;
        }

        num_players = static_cast<int>(count);
    }
    catch (...)
    {
        cout << "Error: Invalid number of players."
             << endl;
        return 1;
    }

    cout << "Player count is: "
         << num_players << endl;

    // Create an array of pointers for each player.
    Bowling_Frame** players =
        new Bowling_Frame*[num_players];

    // Create 10 linked frames for each player.
    for (int i = 0; i < num_players; i++)
    {
        players[i] = new Bowling_Frame(1);

        Bowling_Frame* previous = players[i];

        for (int j = 2; j <= 10; j++)
        {
            Bowling_Frame* current =
                new Bowling_Frame(j);

            previous->next_frame(current);
            current->prev_frame(previous);

            previous = current;
        }
    }

    // Generate scores for every player.
    for (int i = 0; i < num_players; i++)
    {
        Bowling_Frame* current = players[i];

        while (current != nullptr)
        {
            current->update_scores();

            if (!current->strike())
            {
                current->update_scores();
            }

            if (current->frame_num() == 10)
            {
                if (current->strike())
                {
                    current->update_scores();
                    current->update_scores();
                }
                else if (current->spare())
                {
                    current->update_scores();
                }
            }

            current = current->next_frame();
        }
    }

    // Print every frame and each player's total.
    for (int i = 0; i < num_players; i++)
    {
        cout << endl;

        Bowling_Frame* traverse = players[i];

        while (traverse != nullptr)
        {
            cout << "Frame "
                 << traverse->frame_num()
                 << ":" << endl;

            cout << " Score 1: "
                 << traverse->score1() << endl;

            cout << " Score 2: "
                 << traverse->score2() << endl;

            cout << " Frame Total: "
                 << traverse->frame_score() << endl;

            traverse = traverse->next_frame();
        }

        cout << endl;

        cout << "Total Game Score for player "
             << i + 1 << ": "
             << current_game_total(players[i])
             << endl;
    }

    // Delete all frames and free memory.
    for (int i = 0; i < num_players; i++)
    {
        Bowling_Frame* current = players[i];

        while (current != nullptr)
        {
            Bowling_Frame* next =
                current->next_frame();

            delete current;
            current = next;
        }
    }

    delete[] players;

    return 0;
}
