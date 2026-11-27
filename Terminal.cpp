
#include <iostream>
#include <string>
#include <vector>
#include <conio.h>
#include <chrono>
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include "Terminal.h"
#include "Cesare.h"
#include "Matrix.h"

using namespace std;

#define ENIGMA_1_SOL "scuoleaperte2627"
#define ENIGMA_2_SOL "chiavedecifrata"

int domanda(
    Terminal& terminal,
    const string& nome,
    const string& testo,
    const vector<string>& risposte
) {
    int scelta = 0;
    int scelta_prec = 0;
    int tasto;

    // Print question
    terminal.write(testo);
    terminal.newLine(2);

    // Remember where the answers start
    int startY = terminal.cursor.y;

    // Print all answers
    for (int i = 0; i < risposte.size(); i++) {

        if (i == 0)
            terminal.write(nome + "> " + risposte[i]);
        else
            terminal.write(string(nome.length(), ' ') + "  " + risposte[i]);

        terminal.newLine();
    }

    while (true) {

        tasto = _getch();

        if (tasto == 224 || tasto == 0) {

            tasto = _getch();

            if (tasto == 72) { // UP
                scelta--;

                if (scelta < 0)
                    scelta = risposte.size() - 1;
            }
            else if (tasto == 80) { // DOWN
                scelta++;

                if (scelta >= risposte.size())
                    scelta = 0;
            }
            else {
                continue;
            }

            // Remove cursor from previous answer
            terminal.moveTo(nome.length(), startY + scelta_prec);
            terminal.write(" ");

            // Draw cursor on new answer
            terminal.moveTo(nome.length(), startY + scelta);
            terminal.write(">");

            scelta_prec = scelta;
        }

        else if (tasto == 13) { // ENTER
            terminal.moveTo(0, startY + risposte.size() + 1);
            return scelta;
        }
    }
}

void testoPrimoEnigma(Terminal& terminal, const string& nome) {
    terminal.write(
        "Bene, " + nome + ".\n"
        "Il primo rompicapo ti aspetta.\n"
        "Gli hack-a-ton ci costringono a decodificare una stringa esadecimale.\n"
        "Fortunatamente, abbiamo recuperato una tabella ASCII.\n"
        "Potrebbe tornarti utile...\n"
    );

    terminal.newLine(2);

    terminal.write("========== TABELLA ASCII ==========");
    terminal.newLine(2);

    terminal.write(
        "HEX     CARATTERE       HEX     CARATTERE\n"
        "-----------------------------------------\n"
        "61      a               30      0\n"
        "62      b               31      1\n"
        "63      c               32      2\n"
        "64      d               33      3\n"
        "65      e               34      4\n"
        "66      f               35      5\n"
        "67      g               36      6\n"
        "68      h               37      7\n"
        "69      i               38      8\n"
        "6A      j               39      9\n"
        "6B      k\n"
        "6C      l\n"
        "6D      m\n"
        "6E      n\n"
        "6F      o\n"
        "70      p\n"
        "71      q\n"
        "72      r\n"
        "73      s\n"
        "74      t\n"
        "75      u\n"
        "76      v\n"
        "77      w\n"
        "78      x\n"
        "79      y\n"
        "7A      z\n",
        2,
        15
    );

    terminal.newLine(2);

    terminal.write("========== MESSAGGIO CIFRATO ==========");
    terminal.newLine(2);

    terminal.write(
        "73 63 75 6F 6C 65 61 70 65 72 74 65 "
        "32 36 32 37"
    );

    terminal.newLine(3);
}

void primoEnigma(Terminal& terminal, const string& nome) {

    string tentativo;

    testoPrimoEnigma(terminal, nome);

    while (true) {

        terminal.write("Inserisci il messaggio decifrato:");
        terminal.newLine();

        terminal.write("> ");
        getline(cin, tentativo);

        if (tentativo == "ripeti")
            testoPrimoEnigma(terminal, nome);

        if (tentativo == ENIGMA_1_SOL) {

            terminal.newLine(2);

            terminal.write(
                "SUCCESS\n"
            );

            break;
        }

        else {

            terminal.newLine(2);

            terminal.write(
                "STRINGA ERRATA.\n"
            );

        }
    }
}

void testoSecondoEnigma(Terminal& terminal, const string& nome) {
    terminal.write(
        "Questa volta dobbiamo capire cosa si cela dietro questa sequenza di testo:\n"
    );
    terminal.newLine(2);

    terminal.write("             " + cypher(ENIGMA_2_SOL));

    terminal.newLine(3);

    terminal.write("Sappiamo solo che la sequenza originale e' stata modificata secondo una regola precisa.\nSta a te scoprire"
    " come andando a tentativi inserendo stringhe nel cifratore.");

    terminal.newLine(2);

    terminal.write(
        "Se ti trovi in difficolta', puoi chiedere\n"
        "un suggerimento scrivendo SUGGERIMENTO.\n"
    );

    terminal.newLine(3);
}

void secondoEnigma(Terminal& terminal, const string& nome) {

    using namespace std::chrono;

    // Timer dell'enigma
    auto inizio = steady_clock::now();

    string tentativo;

    // Suggerimenti
    const vector<string> suggerimenti = {

        "Prova a concentrarti anche su una singola lettera. "
        "Osserva cosa succede quando provi a decifrarla piu' volte.",

        "Il segreto dell'algoritmo e' spostare ogni lettera "
        "di un determinato numero di posizioni nell'alfabeto.",

        "Facciamo un esempio: CASA diventa MKCK.\n"
        "Attenzione: quando superi la Z, devi ricominciare "
        "dalla A. L'alfabeto e' circolare."
    };

    const vector<int> tempiSuggerimenti = {
        2 * 60,
        7 * 60,
        20 * 60
    };

    int prossimoSuggerimento = 0;

    testoSecondoEnigma(terminal, nome);
    
    while (true) {

        terminal.write("Inserisci la soluzione:");
        terminal.newLine();

        terminal.write("> ");

        getline(cin, tentativo);

        // Trasformiamo il tentativo in minuscolo
        // e rimuoviamo gli spazi.
        string rispostaNormalizzata;

        for (unsigned char c : tentativo) {

            if (!isspace(c)) {
                rispostaNormalizzata +=
                    static_cast<char>(tolower(c));
            }
        }

        if (rispostaNormalizzata == "ripeti") {
            testoSecondoEnigma(terminal, nome);
        }

        // Richiesta di un suggerimento
        if (rispostaNormalizzata == "suggerimento") {

            auto adesso = steady_clock::now();

            int secondiTrascorsi =
                duration_cast<seconds>(adesso - inizio).count();

            terminal.newLine(2);

            if (prossimoSuggerimento >= suggerimenti.size()) {

                terminal.write(
                    "Hai gia' ricevuto tutti i suggerimenti disponibili!\n"
                    "Volessi rivederli, digita SUGGERIMENTITUTTI"
                );

            }
            else if (secondiTrascorsi <
                tempiSuggerimenti[prossimoSuggerimento]) {

                int mancanti =
                    tempiSuggerimenti[prossimoSuggerimento]
                    - secondiTrascorsi;

                int minuti = mancanti / 60;
                int secondi = mancanti % 60;

                terminal.write(
                    "Questo suggerimento non e' ancora disponibile.\n"
                );

                terminal.write(
                    "Dovrai aspettare ancora "
                    + to_string(minuti) + " minuti e "
                    + to_string(secondi) + " secondi."
                );

            }
            else {

                terminal.write(
                    "========== SUGGERIMENTO "
                    + to_string(prossimoSuggerimento + 1)
                    + " ==========\n\n"
                );

                terminal.write(
                    suggerimenti[prossimoSuggerimento]
                );

                prossimoSuggerimento++;
            }

            terminal.newLine(3);

            continue;
        }
        else
        {
            if (rispostaNormalizzata == "suggerimenti" and duration_cast<seconds>(steady_clock::now() - inizio).count() > tempiSuggerimenti[2]) {
                terminal.newLine(3);
                for (int i = 0; i < 3; i++) {
                    terminal.newLine(3);
                    terminal.write(
                        "==========\nSUGGERIMENTO " + std::to_string(i+1) + "\n\n"
                        + suggerimenti[i]
                        + "\n==========\n\n",
                        2,
                        20
                    );

                }

                terminal.newLine(3);

                continue;
            }

        }
        

        // Controllo della soluzione
        if (rispostaNormalizzata == ENIGMA_2_SOL) {

            terminal.newLine(2);

            terminal.write(
                "ACCESSO CONSENTITO."
            );

            break;
        }

        // Risposta errata
        else {

            terminal.newLine(2);

            terminal.write(
                "ACCESSO NEGATO."
            );

            terminal.newLine(2);

            terminal.write("MESSAGGIO CIFRATO:");
            terminal.newLine(2);

            terminal.write(cypher(rispostaNormalizzata));

            terminal.newLine(2);
        }
    }
}


int main() {

    string nome = "davide";

    Terminal terminal;

    terminal.fullScreen();

    terminal.clear();
    
    terminal.write("Premi INVIO per iniziare, o skippa ai crediti con E");

    bool play = true;
    char tasto;
    do {
        tasto = _getch();
    } while (tasto != 13 && tasto != 'e'); // 13 invio, 27 esc

    if (tasto == 'e')
        play = false;

    terminal.clear();

    if (play) {
        
        terminal.write(
            "Siamo appena stati attaccati da un gruppo hacker chiamato hack-a-ton!\n"
            "Hanno infettato i nostri sistemi con un trojan che ci costringe ad utilizzare il computer in questa maniera.\n"
            "Schermo nero e caratteri squadrati..."
        );

        terminal.newLine(2);

        terminal.write("Come ti chiami?");
        terminal.newLine(2);

        terminal.write("> ");
        getline(cin, nome);

        terminal.newLine(2);


        terminal.write("Ciao " + nome + "! \nIl gruppo hacker hack - a - ton ha richiesto che vengano risolti due rompicapi da risolvere per riottenere il controllo \"normale\" del computer.\n"
            "Ti consigliamo di avere sotto mano carta e penna, ti serviranno!");

        terminal.newLine(2);

        int risposta = domanda(
            terminal,
            nome,
            "Sei pronta/o?",
            { "Si", "No", "Ermh... cosa... devo fare?" }
        );
        /*
        if(risposta==1){}

        if (risposta == 2) {
            terminal.newLine();
            terminal.write("Solo provare a rispondere ad un paio di domande");
        }*/
        

        primoEnigma(terminal, nome);

        terminal.write("Complimenti " + nome + "!\n"
            "Hai risolto il primo enigma, passiamo al secondo!"
        );

        terminal.newLine(2);

        
        secondoEnigma(terminal, nome);


        terminal.write("Grande " + nome + "\nHai risolto anche il secondo enigma!\n"
            "Grazie al tuo aiuto siamo riusciti a recuperare il computer!\n"
            "Ora rilassati e goditi questa animazione in stile Matrix, ciao!\n"
        );

        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
    
    // Credits
    
    float clear_probability = 0.8f;
    float spawn_probability = 0.2f;
    int space_interval = 1;

    int HEIGHT = 40;
    int WIDTH = 140;
    Matrix m(WIDTH, HEIGHT);

    while (true) {

        // Stampa "IIS Il Tagliamento"

        int x = 30, y = 10;
        std::string iis = "IIS Il Tagliamento";
        for (char c : iis) {
            m.set(x++, y, c, FOREGROUND_RED);
        }

        x = 50; y = 20;
        std::string dev_by = "Sviluppato da Davide Rizzotti";
        for (int i = 0; i < dev_by.length(); i++) {
            if (i%3==0)
                m.set(x++, y++, dev_by[i], FOREGROUND_RED);
            else
                m.set(x++, y, dev_by[i], FOREGROUND_RED);
        }

        // Check keyboard input without blocking
        if (_kbhit()) {
            int key = _getch();

            if (key == 224) {
                key = _getch();

                if (key == 72) { // UP
                    clear_probability += 0.03f;
                }
                else if (key == 80) { // DOWN
                    clear_probability -= 0.03f;
                }
                else if (key == 77) { // RIGHT
                    spawn_probability += 0.03f;
                }
                else if (key == 75) { // LEFT
                    spawn_probability -= 0.03f;
                }
                else if (key == 'x') {
                    space_interval += 1;
                }
                else if (key == 'z') {
                    space_interval = space_interval >= 0 ? space_interval - 1 : 0;
                }

                clear_probability = std::clamp(clear_probability, 0.0f, 1.0f);
                spawn_probability = std::clamp(spawn_probability, 0.0f, 1.0f);
            }
        }

        float dice = (rand() % 1001) / 1000.0f;
        int h = rand() % HEIGHT;
        int w = rand() % WIDTH;

        if (dice < clear_probability) {
            m.set(w, h, ' ');
            m.set(w, h + space_interval, ' ');
            m.set(w, h - space_interval, ' ');
            m.set(w + space_interval, h, ' ');
            m.set(w - space_interval, h, ' ');
        }

        if (dice < spawn_probability) {
            char c = rand() % 255;
            m.set(w, h, c);
        }

    }

    return 0;
}
