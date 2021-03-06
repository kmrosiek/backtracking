#include "crossword.hpp"
#include "board.hpp"
#include "dictionary.hpp"
#include "word_footprint.hpp"
#include "constraint.hpp"
#include "utils.hpp"

#include <string>
#include <vector>
#include <stdint.h>

bool Crossword::backtracking()
{

    for(uint32_t i = 0; i < NO_OF_DIFF_POSITION_SELECTION; i++)//for(number_of_different_position_selection)
    {
        // Create random footprint - position, direction and word_length for current iteration.
        Word_footprint random_footprint(10, 10);
        // Create constraints for footprint. Needs footprint and board. Returns Constraint class that takes
        // pointer to dictionary lookup function and word to be checked.
        Constraints constraints(random_footprint, board.get_printable(), board.get_width());
        // Create domain - set of words with a given length and that satisfy given constraints.
        std::vector<std::string> ordered_domain = dictionary.create_domain(
                random_footprint.get_word_length(), constraints);
        std::queue<std::string> shuffled_domain = utils::shuffle_domain(ordered_domain);

        while(!shuffled_domain.empty())
        {
            std::string word = shuffled_domain.front();
            board.insert_word(word, random_footprint);
            dictionary.remove_word(word); // if removing option enabled.

            //Update_GUI(SDL_Graphics);
            utils::visualise_board(board.get_printable());
            if(true == backtracking())
                return true; // finished

            board.remove_word(random_footprint);
            dictionary.insert_word(word); // if removing option enabled.
            shuffled_domain.pop();
        }
    }

    return false;
}

void Crossword::prepare_dictionary(const std::string& path)
{
    dictionary.load_words_from_file(path);
}
