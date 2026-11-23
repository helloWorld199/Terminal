
#include <iostream>
#include <string>
#include <vector>
#include <conio.h>
#include "Terminal.h"

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


int main() {

    string nome;

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
        { "Si", "No", "Ho paura..." }
    );

    terminal.newLine(25);
    terminal.write("qua in fondoo\n\n\naaa\n\n\naaaa\n\n\n\naaaaa");

    terminal.newLine(2);

    risposta = domanda(
        terminal,
        nome,
        "Sei pronto?",
        { "Si", "No", "Ho paura..." }
    );
    return 0;
}
