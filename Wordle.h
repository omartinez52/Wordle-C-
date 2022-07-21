#ifndef WORDLE_H
#define WORDLE_H
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <fstream>

#define NUM_OF_WORD_AND_CHAR_GUESS 5
#define NUM_LINES_IN_WORD_DICT 5757
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define RESETCOLOR "\033[0m"

using namespace std;
typedef struct CharacterGuess{
    char character;
    bool correct_index;
    bool in_word;
}CharacterGuess;

class Wordle{
    private:
        string guess_word;
        vector<vector<CharacterGuess>> guesses;
        int num_guess;
        void printTitle(){
            string title = 
           "      _ _ _           _ _ \n"    
           "     | | | |___ ___ _| | |___ \n"
           "     | | | | . |  _| . | | -_|\n"
           "     |_____|___|_| |___|_|___|\n";
           cout <<"\033[36m"<< title <<"\033[0m"<< endl;
        }
        void printTiles(){
            cout << "[" << YELLOW <<"In Word" <<RESETCOLOR<<"] ";
            cout << "[" << GREEN <<"Correct Spot" <<RESETCOLOR<<"] ";
            cout << "[" << RED <<"Not In Word" <<RESETCOLOR<<"]\n";
            string color = "";
            for(int i = 0; i < guesses.size(); i++){
                for(int j = 0; j < NUM_OF_WORD_AND_CHAR_GUESS; j++){
                    if(guesses[i][j].correct_index)
                        color = GREEN;
                    else if(guesses[i][j].in_word)
                        color = YELLOW;
                    else
                        color = RED;
                    cout << "  [ " <<color<< guesses[i][j].character <<RESETCOLOR<<" ]";
                }
                cout<<endl;
            }
        }
        string userGuess(){
            string user_input;
            bool is_good_chars = false;
            do{
                cout << "Enter your 5 letter guess. (Must be characters [a-z] or [A-Z].): ";
                cin >> user_input;
                if(user_input.length() < NUM_OF_WORD_AND_CHAR_GUESS)
                    cout << "Guess needs to be a 5 letter word.\n";
                else{
                    if(user_input.length() > 5)
                        user_input.resize(5);
                    for(char c : user_input){
                        if((c >= 65 && c <= 90) || (c >= 97 && c <= 122))
                            is_good_chars = true;
                        else{
                            is_good_chars = false;
                            break;
                        }
                    }
                }
                if(!is_good_chars)
                    cout <<"5 letter guess must be characters [a-z] or [A-Z].\n";
            }while(!is_good_chars);
            
            return toUpperCase(user_input);
        }
        string getRandomWord(){
            ifstream myFile("sgb_5_letter_words.txt");
            srand(time(NULL));
            int random_line = rand() % NUM_LINES_IN_WORD_DICT + 1;
            int current_line = 1;
            string word_to_guess;
            if(myFile.is_open()){
                while(getline(myFile,word_to_guess)){
                    if(current_line == random_line)
                        break;
                    current_line++;
                }
            myFile.close();
            }
            else{
                cout << "\033[31m *** ERROR READING FROM DICTIONARY ***\033[0m\n";
                exit(1);
            }
            return toUpperCase(word_to_guess);
        }
        string toUpperCase(string s){
            for(int i = 0; i < s.length(); i++)
                if(s[i] >= 97)
                    s[i] -= 32;
            return s;
        }
        void compareUserGuess(string user_guess, bool &is_guessed){
            // check if user guess and word to guess are equal
            is_guessed = (user_guess.compare(guess_word) == 0)? true : false;

            // collect characters in word to guess and frequency
            map<char, int> mapped_chars;
            for(char c : guess_word)
                if(mapped_chars.count(c) > 0)
                    mapped_chars[c]++;
                else
                    mapped_chars[c] = 1;
            
            // insert character into 2d array of word guesses
            for(int i = 0; i < user_guess.length(); i++){
                guesses[num_guess][i].character = user_guess[i];
                if(mapped_chars.count(user_guess[i]) > 0){
                    guesses[num_guess][i].in_word = true;
                    // decrease frequency or remove from character map
                    if(mapped_chars[user_guess[i]] == 1)
                        mapped_chars.erase(user_guess[i]);
                    else
                        mapped_chars[user_guess[i]]--;
                    // check if in correct index
                    if(user_guess[i] == guess_word[i])
                        guesses[num_guess][i].correct_index = true;
                }
            }
        }
        void resetGame(){
            guess_word = getRandomWord();
            num_guess = 0;
            guesses.clear();
            fillGuessesArray();
            startGame();
        }
        void fillGuessesArray(){
            CharacterGuess empty={' ', 0, 0};
            for(int i = 0; i < NUM_OF_WORD_AND_CHAR_GUESS; i++){
                guesses.push_back({empty,empty,empty,empty,empty});
            }
        }
        void playAgain(){
            cout <<"Would you like to try again?\t [y/n]: ";
            string input;
            cin >> input;
            if(input.compare("y") == 0)
                resetGame();
            else{
                cout <<GREEN<<"Thanks" <<RESETCOLOR;
                cout<<RED<<" for"<<RESETCOLOR;
                cout<<YELLOW<<" playing!\n"<<RESETCOLOR;
            }
        }
    public:
        Wordle(){
            num_guess = 0;
            guess_word = getRandomWord();
            fillGuessesArray();
        }
        void startGame(){
            cout << "{+{+{+{ CREATED BY DEX }+}+}+}" << endl;
            cout << guess_word <<endl;
            bool is_guessed = false;
            do{
                system("clear");
                printTitle();
                printTiles();
                string user_guess = userGuess();
                compareUserGuess(user_guess, is_guessed);
                num_guess++;
            }while(num_guess < NUM_OF_WORD_AND_CHAR_GUESS && !is_guessed);
            system("clear");
            printTitle();
            printTiles();

            if(is_guessed)
                cout <<GREEN<<"Congratulations! You Guessed The Correct Word!"<<RESETCOLOR<<endl;
            else{
                cout <<RED<<"You almost had it! The word to guess was: "<<guess_word<<RESETCOLOR<<endl;
            }
            playAgain();
        }
};


#endif