/**
 * @file main.cpp
 * @author DECSAI
 * @note To be implemented by students either completely or by giving them
 * key functions prototipes to guide the implementation
 */

#include <iostream>
#include <exception>
#include <fstream>
#include <cstdlib>
#include "language.h"
#include "bag.h"
#include "player.h"
#include "move.h"
using namespace std;


#define ERROR_ARGUMENTS 1
#define ERROR_OPEN 2
#define ERROR_DATA 3
/**
 * @brief Reports an important error and exits the program
 * @param errorcode An integer representing the error detected, which is represented
 * by several symbolic constants: ERROR_ARGUMENTS, ERROR_OPEN y ERROR_DATA
 * @param errorinfo Additional information regarding the error: "" (empty string) for 
 * errors parsing the arguments to main() and, for the case of errors opening or 
 * reading/writing data, the name of the file thas has failed.
 */
void errorBreak(int errorcode, const string & errorinfo);

/**
 * @brief Shows final data
 * @param l Language
 * @param random Random seed
 * @param b Final bag
 * @param p Final player
 * @param nwords Number of words found
 * @param score Number of points scored
 * @param result Aggregation of all valid words found
 */
void HallOfFame(const Language &l, int random, const Bag &b, const Player &p, 
        int nwords, int score, const string &result);



/**
 * @brief Main function. 
 * @return 
 */
int main(int nargs, char * args[]) {
    Bag bag;
    Player player;
    Language language;
    Move move;
    string word, lang="", goodmoves="", badmoves="", ifilename="", ofilename="";
    int random=-1, nwords=0, score=0;
    ifstream fs; ofstream ofs;
    istream *input = &cin; ostream *output;
    /// @warning: Declare more vars when needed
    bool idparam=false, randomparam = false, fileparam = false;
    string param;
    /// @warning: Check arguments
    if(nargs < 2)
        errorBreak(ERROR_ARGUMENTS, "sin suficientes parametros");
    if(nargs > 7)
        errorBreak(ERROR_ARGUMENTS, "demasiados parametros");
    if(nargs%2==0)
        errorBreak(ERROR_ARGUMENTS, "parametros impares");
    for(int i = 1; i < nargs; i+=2)
        if(!(args[i] != "-l" || args[i] != "-i" || args[i] != "-r")){
            cout << endl << args[i] << "-l";
            errorBreak(ERROR_ARGUMENTS, "parametros no identificados");
        }
    for(int i = 1; i < nargs; i+=2){
        param = args[i];
        if(param == "-l")
            idparam = true;
        else if(param == "-r")
            randomparam = true;
        else if (param == "-i")
            fileparam = true;
    }
    if(!idparam)
        errorBreak(ERROR_ARGUMENTS, "falta -l <ID>");
    /// @warning: Process arguments
    for(int i = 1; i < nargs; i++){
        param = args[i];
        if(param == "-l")
            lang = args[i+1];
        if(randomparam && param == "-r"){
            random = atoi(args[i+1]);
        }
        if(fileparam && param =="-i"){
            ifilename = args[i+1];
            fs.open(ifilename);
            if(fs.fail())
                errorBreak(ERROR_OPEN, ifilename);
            input=&fs;
        }
    }
    
    language.setLanguage(lang);
    cout << "Caracteres permitidos: " << language.getLetterSet() << endl;
    bag.setRandom(random);
    bag.define(language);
    do{
        if(!input->eof()){
            player.add(bag.extract(MAXPLAYER - player.size()));
            cout << "Letras para mover: " << toUTF(player.to_string());
            cout <<endl << "Introduce si es horizontal, fila, columna y la palabra: ";
            move.read(*input);
            if(move.getLetters() != "@"){
                word = normalizeWord(move.getLetters());
                if(player.isValid(word) and word.size() > 1){
                    player.extract(word);
                    if(language.query(word)){
                        cout << "Palabra encontrada" << endl;
                        score+=move.findScore(language);
                        nwords++;
                        goodmoves += word + " - ";
                    }
                    else{
                        cout << "Error, palabra no encontrada" << endl;
                    }
                }
            }
            else
                word = "@";
        }
        else{
            errorBreak(ERROR_DATA, ifilename);
        }
    }while(word != "@");
	/*
	1. El main() recibe como parámetro obligatorio "-l <ID>" y co-
	mo parámetros opcionales "-i <file>" y "-r <random>" ,
	en cualquier orden entre los tres. Si se especifica "-i" se leen
	los datos desde ese fichero, si no, se leen desde el teclado. Si
	se especifica "-r" se define el aleatorio con el número indica-
	do, si no, no se define aleatorio.
	2. Crear una instancia de la clase Language con el anterior ID y
	mostrar el conjunto de caracteres permitido para ese lenguaje.
	3. Crear una instancia de la clase Bag, inicializar el generador de
	números aleatorios con el número aleatorio anterior, si es que
	se ha indicado, y definir su contenido en base al lenguaje que
	se ha declarado anteriormente.
	4. Crear una instancia de la clase Player y llenarla por comple-
	to con caracteres de la bolsa. Este objeto player deberá estar
	siempre ordenado de la A a la Z.
	5. Repetir la siguiente secuencia hasta que se lea un movimiento
	con la palabra "@"
	a) Usar el método read(...) para leer un movimiento (desde
	teclado o desde el fichero de entrada, según el parámetro
	"-i" ). Los valores para isHorizontal, row y column se leen
	pero se van a ignorar en el resto del programa, pues sólo se
	5Metodologı́a de la Programación
	MPALABRADOS (move)
	usará letters. En las anteriores prácticas se han usado pa-
	labras con letras controladas, pues, al fin y al cabo, todas
	provenı́an de bag, las cuales provienen del diccionario, y
	todas siguen el mismo patrón del juego scrabble: las letras
	son mayúsculas, no contienen tildes ni diéresis, aunque al-
	gunos caracteres internacionales están soportados, como
	la Ñ. Aunque no se ha usado hasta ahora, al incluir langua-
	ge.h en cualquier proyecto, también se incluye la definición
	de esta constante:
	static const std::string ALPHABET=toISO(” ABCDEFGHIJKLMNOPQRSTUVWXYZÑ”);
	Esto quiere decir que cualquier palabra que se consulte en
	el diccionario debe estar representada con caracteres de
	ese ALPHABET Esta es la primera vez que exponemos el
	programa a leer datos externos no controlados, por lo que
	habrı́a que transformar cada palabra de juego que se lee,
	a una palabra expresada en ese alfabeto. Y eso es lo que
	hace la función normalizeWord(...)
	b) Si la palabra leı́da es válida para el valor de Player (al me-
	nos ha de tener dos caracteres) entonces se anota la pala-
	bra, se calcula la puntuación de la palabra según el diccio-
	nario y se anota, se eliminan las letras de player, se sacan
	nuevas letras de bag para rellenar player, y se sigue ju-
	gando. Ya nos estamos acercando al ciclo de juego de la
	práctica final.
	c) Si la palabra leı́da no es compatible con el valor de player,
	se desecha y se lee el siguiente movimiento.
	6. Terminar con la llamada a HallOfFame para visualizar los re-
	sultados.
	7. Si en cualquier momento se presenta un error en los argumen-
	tos, en la apertura de ficheros o en la lectura de datos del fiche-
	ro, se debe usar la función errorBreak(...) para notificar el error
	y parar el programa.
	*/

    /// @warning: final report
    HallOfFame(language, random, bag, player, nwords, score, goodmoves);
    return 0;
}

void HallOfFame(const Language &l, int random, const Bag &b, const Player &p, 
        int nwords, int score, const string &result) {
    cout << endl << "%%%OUTPUT" << endl << "LANGUAGE: "<<l.getLanguage()<< " ID: " << random << endl;
    cout << "BAG ("<<b.size()<<"): " << toUTF(b.to_string()) << endl;
    cout << "PLAYER (" <<p.size() << "): " << toUTF(p.to_string())<<endl;
    cout << nwords << " words and " << score << " points "<<endl << 
            toUTF(result) << endl;    
}

void errorBreak(int errorcode, const string &errordata) {
    cerr << endl << "%%%OUTPUT" << endl;
    switch(errorcode) {
        case ERROR_ARGUMENTS:
            cerr<<"Error in call. Please use:\n -l <language>  [-r <randomnumber> -i <inputfile>]" << endl;
            break;
        case ERROR_OPEN:
            cerr<<"Error opening file "<<errordata << endl;
            break;
        case ERROR_DATA:
            cerr<<"Data error in file "<<errordata << endl;
            break;
    }
    std::exit(1);
}

