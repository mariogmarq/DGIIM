/**
 * @file main.cpp
 * @author DECSAI
 * @note To be implemented by students either completely or by giving them
 * key functions prototipes to guide the implementation
 */

#include "tiles.h"
#include "bag.h"
#include "player.h"
#include "movelist.h"
#include "move.h"
#include "language.h"
#include <cstdlib>
#include <fstream>
#include <iostream>


using namespace std;


#define ERROR_ARGUMENTS 1
#define ERROR_OPEN 2
#define ERROR_DATA 3
#define PASSWORD "MPALABRADOS-V1"
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
 * @brief Main function. 
 * @return 
 */
int main(int nargs, char * args[]) {
    //Declaraciones
    Tiles tiles;
    Movelist original, legal, accepted, rejected;
    Player player;
    Bag bag;
    Move aux;
    ostream *os = &cout;
    bool continuarPartida = false, lparam = false, rparam = false, wparam = false, hparam = false, pparam = false, saveparam = false;
    ifstream pfile, inMatchfile;
    ofstream outMatchfile;
    string lang = "", param = "", pfilename = "",aux1=args[1], aux2= nargs>=4 ? args[3] : "";
    int score = 0, relleno=0;
    Language language;
    int width = 0, height = 0, id = 0;
    
    //Comprobacion de argumentos general
    if(nargs%2!=1)
        errorBreak(ERROR_ARGUMENTS, "");
    
    //Comprobacion de modo de juego
    if("-open"==aux1 || "-open"==aux2){
        continuarPartida = true;
        if(nargs > 7)
            errorBreak(ERROR_ARGUMENTS, "");
    }
    
    //Se leen todos los parametros
    for(int i = 1; i < nargs; i+=2){
        param = args[i];
        if(param=="-l"){
            lparam = true;
            lang = args[i+1];
        }
        if(param=="-w"){
            wparam = true;
            width = atoi(args[i+1]);
        }
        if(param=="-h"){
            hparam = true;
            height = atoi(args[i+1]);
        }
        if(param=="-p"){
            pparam = true;
            pfile.open(args[i+1]);
            pfilename = args[i+1];
            if(pfile.fail())
                errorBreak(ERROR_OPEN, args[i+1]);
        }
        if(param=="-r"){
            rparam = true;
            id = atoi(args[i+1]);
        }
        if(param=="-save"){
            saveparam = true;
            outMatchfile.open(args[i+1]);
            if(outMatchfile.fail())
                errorBreak(ERROR_OPEN, args[i+1]);
            os = &outMatchfile;
        }
        if(param=="-open"){
            inMatchfile.open(args[i+1]);
            if(inMatchfile.fail())
                errorBreak(ERROR_OPEN, args[i+1]);
        }
    }
    
    //Procesamiento error parametros
    if(!pparam)
        errorBreak(ERROR_ARGUMENTS, "");
    if(continuarPartida && (lparam || wparam || hparam || rparam))
        errorBreak(ERROR_ARGUMENTS, "");
    else if((!lparam || !wparam || !hparam)&&!continuarPartida)
        errorBreak(ERROR_ARGUMENTS, "");
    
    
    //if(!continuarPartida)
    if(!continuarPartida){
        language.setLanguage(lang);
        if(rparam)
            bag.setRandom(id);
        bag.define(language);
        player.add(bag.extract(MAXPLAYER));
        tiles.setSize(height, width);
    }
    else {
        inMatchfile >> aux1;
        if(aux1!=PASSWORD)
            errorBreak(ERROR_DATA, "");
        inMatchfile >> score;
        inMatchfile >> lang;
        if(inMatchfile.eof())
            errorBreak(ERROR_DATA,"");
        inMatchfile >> tiles;
        inMatchfile >> relleno >> aux1;
        if(inMatchfile.eof())
            errorBreak(ERROR_DATA,"");
        player.add(toISO(aux1));
        inMatchfile >> relleno >> aux2;
        if(inMatchfile.eof())
            errorBreak(ERROR_DATA,"");
        bag.set(toISO(aux2));
        language.setLanguage(lang);
    }
    
    pfile >> original;
    if(pfile.eof())
        errorBreak(ERROR_DATA, pfilename);
    legal = original;
    legal.zip(language);
    
    for(int i = 0; i < legal.size(); i++){
        aux = legal.get(i);
        if(player.isValid(aux.getLetters())){
            player.extract(aux.getLetters());
            player.add(bag.extract(MAXPLAYER-player.size()));
            aux.setScore(aux.findScore(language));
            accepted.add(aux);
            tiles += aux;
        }
        else
            rejected.add(aux);
    }
    score = continuarPartida ? accepted.getScore() + score : accepted.getScore();
    if(os==&cout)
        *os << "%%%OUTPUT" << endl;
    else
        *os << PASSWORD << endl;
    *os << score << endl << lang << endl;
    tiles.print(*os);
    *os << player.size() << " " << toUTF(player.to_string()) << endl;
    *os << bag.size() << " " << toUTF(bag.to_string()) << endl;
    
    
/*

 * 1) El main() recibe múltiples parámetros de entrada, distinguiendo
entre dos modos de funcionamiento.

    * a) Empezar una partida nueva. Para ello los parámetros de
llamada serán
-l <lang> -w <int> -h <int> -p <pfile> [-r <int> -save <matchfile>]
especificando el diccionario, el ancho y alto del tablero de
juego, el fichero de movimientos registrados y, opcional-
mente, el número aleatorio y la posibilidad de salvar la la
jugada en un fichero con extensión .match con la opción
-save . En caso de que no se indique esta última opción,
entonces deberá mostrar el estado final de la partida con el
mismo formato (ver Sección ??).

     * b) Continuar una partida existente. Para ello los parámetros
de llamada serán
5Metodologı́a de la Programación
MPALABRADOS (tiles-1)
-open <matchfile> -p <playfile> [-save <matchfile>]
indicando la apertura de un fichero .match desde el que
se restaura el estado anterior de la partida, y un fichero
de movimientos. Opcionalmente, se podrá grabar la partida
final si se indica el parámetro -save comentado antes.

 * 2. Crear una instancia de la clase Language con el ID indicado.

 * 3. Crear una instancia de la clase Bag, si es una partida nueva,
inicializar la bolsa, en otro caso, cargarla directamente desde el
fichero .match

 * 4. Crear una instancia de la clase Player y inicializarla por com-
pleto con caracteres de la bolsa o bien leerla del fichero .match.

 * 5. Crear una instancia de la clase Tiles y dimensionarla según
los parámetros leı́dos, bien desde la lı́nea de comandos, bien
desde el fichero .match.

 * 6. Crear una instancia de la clase bf Movelist llamada original
y leer todos los movimientos desde el fichero indicado en el
parámetro -p usando operador sobrecargado >>

 * 7. Crear una instancia de Movelist llamada legal que contenga
sólo los movimientos de original que están en el diccionario
del lenguaje elegido. Usar, para ello, el método zip(...)

 * 8. Crear dos instancias adicionales de Movelist y llamarlas accepted
y rejected

 * 9. Recorrer toda la lista de movimientos leı́da y, por cada uno de
ellos.

     * a) Si el movimiento está en el diccionario, añadir la palabra a
la lista accepted , calcular su puntuación, según el idioma
y acumularla. A continuación, se deberá colocar cada mo-
vimiento en su posición correspondiente en la instancia de
Tiles creada anteriormente, ignorando aquellos caracteres
que caigan fuera de las dimensiones permitidas y sobrees-
cribiendo los caracteres que se crucen.

     * b) En otro caso añadirla a la lista rejected .

 * 10. Terminar mostrando el estado de la partida en pantalla o guardándo-
lo en disco según la presencia o no de -save .
11. Si en cualquier momento se presenta un error en los argumen-
tos, en la apertura de ficheros o en la lectura de datos del fiche-
ro, se debe usar la función errorBreak(...) para notificar el error
y parar el programa 
 */
    pfile.close();
    if(saveparam)
        outMatchfile.close();
    if(continuarPartida)
        inMatchfile.close();
    return 0;
}


void errorBreak(int errorcode, const string &errordata) {
    cerr << endl << "%%%OUTPUT" << endl;
    switch(errorcode) {
        case ERROR_ARGUMENTS:
            cerr<<"Error in call. Please use either:"<<endl;
            cerr<< "-l <language> -w <width> -h <height> -p <playfile> [-r <randomnumber> -save <matchfile>]"<<endl;
            cerr<< "-open <matchfile> -p <playfile> [-save <matchfile>]"<<endl;            
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

