
#include <iostream>
#include <string>
#include <vector>
#include <conio.h>
#include <chrono>
#include <algorithm>
#include <cctype>
#include "Terminal.h"
#include "Cesare.h"

using namespace std;


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

void primoEnigma(Terminal& terminal, const string& nome) {

    string soluzione = "orientamentoiltagliamento";
    string tentativo;

    terminal.write(
        "Bene, " + nome + ".\n"
        "Il primo rompicapo ti aspetta.\n"
        "Gli hacker hanno lasciato un messaggio cifrato.\n"
        "Fortunatamente, abbiamo recuperato una tabella ASCII.\n"
        "Potrebbe tornarti utile...\n"
    );

    terminal.newLine(2);

    terminal.write("========== TABELLA ASCII ==========");
    terminal.newLine(2);

    terminal.write(
        "HEX     CARATTERE\n"
        "------------------\n"
        "61      a\n"
        "62      b\n"
        "63      c\n"
        "64      d\n"
        "65      e\n"
        "66      f\n"
        "67      g\n"
        "68      h\n"
        "69      i\n"
        "6A      j\n"
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
        5,
        50
    );

    terminal.newLine(2);

    terminal.write("========== MESSAGGIO CIFRATO ==========");
    terminal.newLine(2);

    terminal.write(
        "6F 72 69 65 6E 74 61 6D 65 6E 74 6F "
        "69 6C 74 61 67 6C 69 61 6D 65 6E 74 6F"
    );

    terminal.newLine(3);

    while (true) {

        terminal.write("Inserisci il messaggio decifrato:");
        terminal.newLine();

        terminal.write("> ");
        getline(cin, tentativo);

        if (tentativo == soluzione) {

            terminal.newLine(2);

            terminal.write(
                "ACCESSO CONSENTITO.\n"
            );

            break;
        }

        else {

            terminal.newLine(2);

            terminal.write(
                "ACCESSO NEGATO\n"
            );

            terminal.newLine(2);
        }
    }
}


void secondoEnigma(Terminal& terminal, const string& nome) {

    using namespace std::chrono;

    // Timer dell'enigma
    auto inizio = steady_clock::now();

    // Soluzione
    const string soluzione = "sistemabucato";

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
        /*2 * 60,
        7 * 60,
        20 * 60*/
        10,
        15,
        20
    };

    int prossimoSuggerimento = 0;

    terminal.write(
        "========================================\n"
        "             SECONDO ENIGMA\n"
        "========================================\n\n"
        "Bene, " + nome + ".\n"
        "Abbiamo intercettato un altro messaggio.\n"
        "Questa volta sembra essere stato cifrato con un algoritmo diverso!\n"
    );

    terminal.newLine(2);

    terminal.write("MESSAGGIO INTERCETTATO:");
    terminal.newLine(2);

    terminal.write("             cscdowklemkdy");

    terminal.newLine(3);

    terminal.write("Fortunatamente, il nostro team di controhackeraggio e' riuscito a recuperare l'algoritmo di cifratura.\n"
        "Le sue caratteristiche non sono note (purtroppo e' un file binario incomprensibile...),\n"
        "ma questo ci permette di capire le regole dell'algoritmo andando a tentativi");

    terminal.newLine(2);

    terminal.write(
        "Riesci a capire cosa nasconde?\n\n"
        "Hai un numero illimitato di tentativi.\n"
        "Se ti trovi in difficolta', puoi chiedere\n"
        "un suggerimento scrivendo SUGGERIMENTO.\n"
    );

    terminal.newLine(3);

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
        if (rispostaNormalizzata == soluzione) {

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

    string nome = "pippo";

    Terminal terminal;

    terminal.fullScreen();

    terminal.clear();

    terminal.write(
        "Siamo appena stati attaccati da un gruppo hacker chiamato hack-a-ton.\n"
        "Non possiamo fare altro che utilizzare il computer in questa maniera.\n"
        "Schermo nero e caratteri squadrati..."
    );

    terminal.newLine(2);

    terminal.write("Come ti chiami?");
    terminal.newLine();

    terminal.write("> ");
    getline(cin, nome);

    terminal.newLine(2);

    
    terminal.write("Ciao " + nome +"!. \nIl gruppo hacker hack - a - ton ha lasciato due rompicapi da risolvere\n"
                "per riottenere il controllo del computer.\n"
                "ti consigliamo di avere sotto mano carta e penna, ti serviranno!");
    
    terminal.newLine(2);

    int risposta = domanda(
        terminal,
        nome,
        "Sei pronto?",
        { "Si", "No", "Ermh... cosa... devo fare?" }
    );


    primoEnigma(terminal, nome);

    secondoEnigma(terminal, nome);

    terminal.write("Complimenti " + nome + "!\n"
        "Grazie al tuo aiuto siamo riusciti a recuperare il computer!\n"
        "A presto!\n"
    );

    return 0;
}
