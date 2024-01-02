#include "Random.h" // learncpp
#include <iostream>
#include <vector>
#include <string_view>

namespace Panel {
    constexpr int wrongGuesses {5};
}

namespace WordList{
    std::vector<std::string_view> words {"mystery", "funny", "broccoli", "eat", "account", "read", "almost", "until", "spaghetti", "red", "opinion", "see", "beautiful", "mountain", "distance", "dance", "luggage", "dog"};

    std::string_view getRandomWord(){
        return words[Random::get<std::size_t>(0, words.size() -1)];
    }
}

class Session {
    std::string_view m_word {WordList::getRandomWord()};
    std::vector<bool> m_letterGuessed {std::vector<bool>(26)};

    std::size_t toIndex(char c) const { return static_cast<std::size_t>((c%32)-1); }

    int m_wrongGuessesLeft { Panel::wrongGuesses };

    public:

    std::string_view getWord() const {return m_word;}

    bool isLetterGuessed(char c) const { return m_letterGuessed[toIndex(c)]; }
    void setLetterGuessed(char c) { m_letterGuessed[toIndex(c)]= true; }

    int wrongGuessesLeft() const { return m_wrongGuessesLeft; }
    void removeGuess() { -- m_wrongGuessesLeft; }

    bool isLetterInWord(char c) const {
        for (auto cc: m_word)
        {
            if(cc == c){
                return true;
            }
        }

        return false;
    }

    bool won(){
        for(auto c: m_word)
        {
            if(!isLetterGuessed(c)){
                return false;
            }
        }
        return true;
    }

};

void show(const Session& s){
    std::cout << '\n';

    std::cout << "The word: ";

    for(auto c: s.getWord()){
        if(s.isLetterGuessed(c)){
            std::cout << c;
        } else {
            std::cout << '_';
        }
    }

    std::cout <<" Wrong guesses: ";
    for(size_t i=0; i < s.wrongGuessesLeft(); ++i){
        std::cout << '+';
    }

    for(char c='a'; c <= 'z'; ++c){
        if(s.isLetterGuessed(c) && !s.isLetterInWord(c)){
            std::cout << c;
        }
    }

    std::cout << '\n';
}

char getGuess(const Session& s){

    std::cout << "Enter the next letter: ";

    while(true){
        char c{};
        std::cin >> c;

        if(!std::cin){
            std::cin.clear();
            std::cout << "Invalid input! \n";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if(c<'a' || c > 'z'){
            std::cout << "Invalid!";
            continue;
        }

        if(s.isLetterGuessed(c)){
            std::cout <<"Already guessed!";
            continue;
        }

        return c;
    }
}

void inGuesses(Session &s, char c){
    s.setLetterGuessed(c);

    if(s.isLetterInWord(c)){
        std::cout << "Yes, " << c << " is in the word \n";
        return;
    }
    
    std::cout << "No, " << c << " is not in the word \n";

    s.removeGuess();

}

int main(){
    std::cout << "Welcome to Hangman C++ \n";
    std::cout << "In order to win, guess the word. Lose? Consume the pluses. \n\n";

    Session s {};

    int count { 6 };

    while(s.wrongGuessesLeft() && !s.won()){
        show(s);
        char c { getGuess(s) };
        inGuesses(s, c);
    }

    show(s);

    if (!s.wrongGuessesLeft()){
        std::cout << "Game lost! The word was: " << s.getWord() << '\n';
    } else{
        std::cout << "Game won!\n";
    }

    return 0;
}