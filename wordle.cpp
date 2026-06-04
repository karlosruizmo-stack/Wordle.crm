#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>

#ifdef _WIN32
  #define CLR_GREEN  ""
  #define CLR_YELLOW ""
  #define CLR_GRAY   ""
  #define CLR_RESET  ""
#else
  #define CLR_GREEN  "\033[42;30m"
  #define CLR_YELLOW "\033[43;30m"
  #define CLR_GRAY   "\033[100;37m"
  #define CLR_RESET  "\033[0m"
#endif

const std::vector<std::string> WORDS = {
    "piano", "carta", "playa", "monte", "cielo",
    "brisa", "flota", "gruta", "llama", "prado",
    "barco", "campo", "feria", "globo", "huevo",
    "jaula", "limon", "mango", "noche", "oveja",
    "perro", "queso", "radio", "sabor", "tierra",
    "uvula", "vapor", "xilof", "yerno", "zorro",
    "arena", "bosco", "calor", "dulce", "espada",
    "fuego", "grano", "hielo", "indio", "juego"
};

std::string toUpper(std::string s) {
    for (char& c : s) c = toupper(c);
    return s;
}

void printBoard(const std::vector<std::string>& guesses,
                const std::vector<std::string>& feedbacks) {
    std::cout << "\n";
    for (int i = 0; i < 6; i++) {
        std::cout << "  ";
        if (i < (int)guesses.size()) {
            std::string g = toUpper(guesses[i]);
            std::string f = feedbacks[i];
            for (int j = 0; j < 5; j++) {
                if (f[j] == 'G') std::cout << CLR_GREEN  << " " << g[j] << " " << CLR_RESET;
                else if (f[j] == 'Y') std::cout << CLR_YELLOW << " " << g[j] << " " << CLR_RESET;
                else                  std::cout << CLR_GRAY   << " " << g[j] << " " << CLR_RESET;
            }
        } else {
            for (int j = 0; j < 5; j++) std::cout << CLR_GRAY << "   " << CLR_RESET;
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

std::string getFeedback(const std::string& guess, const std::string& secret) {
    std::string feedback(5, 'X');
    int used[5] = {0};

    
    for (int i = 0; i < 5; i++) {
        if (guess[i] == secret[i]) {
            feedback[i] = 'G';
            used[i] = 1;
        }
    }

    for (int i = 0; i < 5; i++) {
        if (feedback[i] == 'G') continue;
        for (int j = 0; j < 5; j++) {
            if (!used[j] && guess[i] == secret[j]) {
                feedback[i] = 'Y';
                used[j] = 1;
                break;
            }
        }
    }
    return feedback;
}

void printLegend() {
    std::cout << CLR_GREEN  << " G " << CLR_RESET << " Lletra correcta i posicio correcta\n";
    std::cout << CLR_YELLOW << " Y " << CLR_RESET << " Lletra present, posicio incorrecta\n";
    std::cout << CLR_GRAY   << " X " << CLR_RESET << " Lletra absent\n\n";
}

int main() {
    srand((unsigned)time(nullptr));
    bool playAgain = true;

    std::cout << "\n=============================\n";
    std::cout << "       W O R D L E  C++      \n";
    std::cout << "=============================\n";
    printLegend();

    while (playAgain) {
        std::string secret = WORDS[rand() % WORDS.size()];
        std::vector<std::string> guesses, feedbacks;
        bool won = false;

        for (int attempt = 1; attempt <= 6 && !won; attempt++) {
            printBoard(guesses, feedbacks);
            std::cout << "Intent " << attempt << "/6 — Escriu una paraula de 5 lletres: ";

            std::string input;
            std::cin >> input;

            
            for (char& c : input) c = tolower(c);

            if (input.length() != 5) {
                std::cout << "  !! Ha de tenir exactament 5 lletres.\n";
                attempt--;
                continue;
            }
            if (!std::all_of(input.begin(), input.end(), ::isalpha)) {
                std::cout << "  !! Nomes lletres, sense accents ni numeros.\n";
                attempt--;
                continue;
            }

            std::string fb = getFeedback(input, secret);
            guesses.push_back(input);
            feedbacks.push_back(fb);

            if (fb == "GGGGG") won = true;
        }

        printBoard(guesses, feedbacks);

        if (won) {
            std::cout << "  ENHORABONA! Has endevinat la paraula!\n\n";
        } else {
            std::cout << "  Has perdut! La paraula era: " << toUpper(secret) << "\n\n";
        }

        std::cout << "Vols jugar de nou? (s/n): ";
        char opt; std::cin >> opt;
        playAgain = (tolower(opt) == 's');
    }

    std::cout << "\nGracies per jugar! Fins aviat.\n\n";
    return 0;
}