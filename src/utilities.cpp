#include "utilities.hpp"
vector<int> separate_by_delimeter(string original_command, int col , int row)
{
    int counter_col = 0;
    vector<int> storing_data;
    string token = original_command.substr(0, original_command.find(INTRO_DELIMETER));
    original_command.erase(0, token.size() + 2);
    while (original_command != "")
    {
        counter_col++;
        string token;
        token = original_command.substr(0, original_command.find(TYPE_DELIMETER));
        original_command.erase(0, token.size() + 2);
        if (counter_col == col)
        {
            if(row == 1 || row == 2)
                token.erase(0, token.find(DATA_DELIMETER) + 1);
            while (token != "")
            {
                string temp_token = token.substr(0, token.find(DATA_DELIMETER));
                storing_data.push_back(stoi(temp_token));
                token.erase(0, temp_token.size() + 1);
            }
            return storing_data;
        }
    }
    return storing_data;
}

vector<int> read_from_file(int row, int col)
{
    ifstream game_setting_file(PATH_GAME_SETTING);
    string info_per_line;
    vector<int> storing_data;
    if (game_setting_file.is_open())
    {
        for (int i = 1; i < row; i++)
        {
            getline(game_setting_file, info_per_line);
        }
        getline(game_setting_file, info_per_line);
        storing_data = separate_by_delimeter(info_per_line, col , row);
    }
    else
    {
        cerr << ERROR_MESSAGE << endl;
        exit(1);
    }
    game_setting_file.close();
    return storing_data;
}

int get_random_number_between_a_limit(int limit)
{
    int random_num = 0;
    unsigned seed = static_cast<unsigned>(chrono::system_clock::now().time_since_epoch().count());
    mt19937 generator(seed);
    uniform_int_distribution<int> distribution(1,limit-1);
    random_num = distribution(generator);
    return random_num;
}