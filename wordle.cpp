// Wordle en C++ (versión de consola)


#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>
#include <cctype>

using namespace std;

const int LONGITUD = 5;
const int MAX_INTENTOS = 6;

// Colores ANSI
const string VERDE = "\033[42;30m";
const string AMARILLO = "\033[43;30m";
const string GRIS = "\033[100;97m";
const string RESET = "\033[0m";

const vector<string> PALABRAS = {
    "perro", "gatos", "mundo", "libro", "playa", "techo", "nubes", "carro",
    "tigre", "plano", "fuego", "lunes", "vigor", "mesas", "pinos", "corte",
    "salsa", "norte", "reina", "falso", "queso", "pasta", "rosas", "cielo",
    "verde", "trigo", "campo", "dulce", "audio", "limon", "salon", "bravo",
    "civil", "ropas", "humor", "radio", "tarde", "noche", "junto", "gente"
};

string aMinusculas(string s) {
    for (char &c : s) c = tolower(static_cast<unsigned char>(c));
    return s;
}


vector<int> evaluar(const string &secreta, const string &intento) {
    vector<int> resultado(LONGITUD, 0);
    vector<int> restantes(26, 0);


    for (int i = 0; i < LONGITUD; i++) {
        if (intento[i] == secreta[i]) {
            resultado[i] = 2;
        } else {
            restantes[secreta[i] - 'a']++;
        }
    }

    for (int i = 0; i < LONGITUD; i++) {
        if (resultado[i] == 2) continue;
        int idx = intento[i] - 'a';
        if (restantes[idx] > 0) {
            resultado[i] = 1;
            restantes[idx]--;
        }
    }
    return resultado;
}

void mostrar(const string &intento, const vector<int> &res) {
    for (int i = 0; i < LONGITUD; i++) {
        string color = (res[i] == 2) ? VERDE : (res[i] == 1) ? AMARILLO : GRIS;
        cout << color << " " << static_cast<char>(toupper(intento[i])) << " " << RESET << " ";
    }
    cout << "\n";
}

bool valida(const string &s) {
    if (s.size() != LONGITUD) return false;
    return all_of(s.begin(), s.end(), [](char c) { return c >= 'a' && c <= 'z'; });
}

int main() {
    mt19937 rng(static_cast<unsigned>(time(nullptr)));
    string jugarOtra;

    do {
        string secreta = PALABRAS[rng() % PALABRAS.size()];
        vector<string> historial;
        vector<vector<int>> resultados;
        bool ganado = false;

        cout << "\n=== WORDLE ===\n";
        cout << "Adivina la palabra de " << LONGITUD << " letras. Tienes "
             << MAX_INTENTOS << " intentos.\n";
        cout << "Verde = bien colocada | Amarillo = existe pero en otro sitio | Gris = no está\n\n";

        for (int intento = 1; intento <= MAX_INTENTOS && !ganado; intento++) {
            string entrada;
            while (true) {
                cout << "Intento " << intento << "/" << MAX_INTENTOS << ": ";
                cin >> entrada;
                entrada = aMinusculas(entrada);
                if (valida(entrada)) break;
                cout << "Escribe una palabra de " << LONGITUD
                     << " letras (a-z, sin acentos ni ñ).\n";
            }

            vector<int> res = evaluar(secreta, entrada);
            historial.push_back(entrada);
            resultados.push_back(res);

            cout << "\n";
            for (size_t i = 0; i < historial.size(); i++)
                mostrar(historial[i], resultados[i]);
            cout << "\n";

            if (entrada == secreta) ganado = true;
        }

        if (ganado)
            cout << "¡Enhorabuena! Has acertado en " << historial.size() << " intento(s).\n";
        else
            cout << "Has perdido. La palabra era: " << aMinusculas(secreta) << "\n";

        cout << "¿Jugar otra vez? (s/n): ";
        cin >> jugarOtra;
    } while (!jugarOtra.empty() && tolower(jugarOtra[0]) == 's');

    cout << "¡Hasta la próxima!\n";
    return 0;
}