/**
 * @file main.cpp
 * @author DECSAI
 * @note To be implemented by students either completely or by giving them
 * key functions prototipes to guide the implementation
 */

#include <string>
#include <cassert>
#include <iostream>
#include <random>
#include <fstream>
#include <cmath>
#include "language.h"
#include "bag.h"
#include "player.h"
#include "move.h"
#include "movelist.h"
#include "tiles.h"
#include "window.h"
#include "AnsiTerminal.h"
using namespace std;


#define ERROR_ARGUMENTS 1
#define ERROR_OPEN 2
#define ERROR_DATA 3
#define GENERAL_ERROR 4
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


ostream & operator<<(ostream & os, const Game & game);
istream & operator>>(istream & is, Game &game);

/**
 * @brief Main function. 
 * @return 
 */
int main(int nargs, char * args[]) {
    Move move;
    Game game;
    int w=-1, h=-1, wait=0;
    string lang="",ifilematch="", ofilematch="", word;
    ifstream ifile; ofstream ofile;
    bool end=false;
    char c;
    
    bool continuarPartida = false, lparam = false, rparam = false, wparam = false, hparam = false, saveparam = false;
    string param = "", aux1=args[1], aux2= nargs>=4 ? args[3] : "";
    int width = 0, height = 0;
    /// Check arguments
    
    //Comprobacion de argumentos general
    if(nargs%2!=1)
        errorBreak(ERROR_ARGUMENTS, "");
    
    //Comprobacion de modo de juego
    if("-open"==aux1 || "-open"==aux2){
        continuarPartida = true;
        if(nargs > 5)
            errorBreak(ERROR_ARGUMENTS, "");
    }
    // Process arguments
    
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
        if(param=="-r"){
            rparam = true;
            game.random = atoi(args[i+1]);
        }
        if(param=="-save"){
            saveparam = true;
            ofilematch=args[i+1];
            ofile.open(ofilematch);
            if(ofile.fail())
                errorBreak(ERROR_OPEN, args[i+1]);
        }
        if(param=="-open"){
            ifilematch=args[i+1];
            ifile.open(ifilematch);
            if(ifile.fail())
                errorBreak(ERROR_OPEN, args[i+1]);
        }
    }
    
    if(continuarPartida && (lparam || wparam || hparam || rparam))
        errorBreak(ERROR_ARGUMENTS, "");
    else if((!lparam || !wparam || !hparam)&&!continuarPartida)
        errorBreak(ERROR_ARGUMENTS, "");
    /// load data from file, if asked to in arguments
    if(continuarPartida){
        ifile >> game;
        if(ifile.eof())
            errorBreak(ERROR_DATA,ifilematch);
        ifile.close();
    }
    else{
        game.language.setLanguage(lang);
        if(rparam)
            game.bag.setRandom(game.random);
        game.bag.define(game.language);
        game.player.add(game.bag.extract(MAXPLAYER));
        game.tiles.setSize(height, width);
        game.score = 0;
    }
    // Game's main loop 
    // 1) First set the size of the window according to the size (rows & columns) of
    // the new Tiles
    game.setWindowSize();
    game.doPaint();
    game.wait=wait;
    while (!end)  {
        // 2) Given the inner data members, it pretty-prints the screen
        game.doPaint();
        
        // 3) Reads the movement from cin
        cin >> move;
        word = move.getLetters();
        if (word=="_") {
            end=true;
        }
        else if(game.player.isValid(word)){
        // Checks whether the movement is valid accoring to the letters in player    
            // Finds all the crosswords produced by move and inform about the reasons of rejection
            game.crosswords = game.tiles.findCrosswords(move,game.language);
            //Checks that the crosswords are valid, that is either has a positive score
            //      or produces at least a cross with other existin letters
            if ((game.crosswords.getScore()>0 && !(game.crosswords.size()==1 && game.crosswords.get(0).getLetters()== word && game.score>0)) || (game.crosswords.getScore()>0 &&game.acceptedmovements.size()==0&&game.crosswords.size()>0)){
                // Once a valid move is detected, the user is asked to confirm
                // call doConfirmCrosswords()
                if(game.doConfirmCrosswords("Accept movement?[Y/y]")){
                    game.player.extract(word);
                    game.player.add(game.bag.extract(7-game.player.size()));
                    move.setScore(game.crosswords.getScore());
                    game.acceptedmovements.add(move);
                    game.tiles+=move;
                    game.score +=move.getScore();
                }
                //     the movement. If he/she does (by pressing Y o y) then the movement
                //     is inserted in tiles and its score is calculated,
                //     otherwise, the move is just ignored
            // If valid and accepted, computes the score and adds it
                // Show crosswords found
            // If it is a bad crosswords
 
                // call doBadCrosswords()
            } else if(game.crosswords.size()==1 && game.crosswords.get(0).getScore()==BOARD_OVERFLOW){
                game.doBadCrosswords("Board overflow");
                move.setScore(BOARD_OVERFLOW);
                game.rejectedmovements.add(move);
            }else if(game.crosswords.size()==1 && game.crosswords.get(0).getScore()==NONEXISTENT_WORD){
                game.doBadCrosswords("Non existent word");
                move.setScore(NONEXISTENT_WORD);
                game.rejectedmovements.add(move);
            } else if(game.crosswords.size()==1 && game.crosswords.get(0).getScore()==NOT_FREE){
                game.doBadCrosswords("Not free");
                move.setScore(NOT_FREE);
                game.rejectedmovements.add(move);
            } else if(game.crosswords.size()==0){
                game.doBadCrosswords("Missing crosswords");
                move.setScore(MISSING_CROSSWORDS);
                game.rejectedmovements.add(move);
            }
            
            else {
                game.doBadCrosswords("Bad crosswords found");
                game.rejectedmovements.add(move);
            }
            // If not valid w.r.t. player

                // call doBadCrosswords()
                
         }else {
            move.setScore(INFEASIBLE_WORD);
            game.crosswords.add(move);
            game.doBadCrosswords("Infeasible word");
            game.rejectedmovements.add(move);
        }
    
    }
    // End of game
    // Save file or print screen
    if(saveparam){
        ofile << PASSWORD << endl;
        ofile << game;
        if(ofile.bad())
            errorBreak(ERROR_DATA, "");
        ofile.close();
    }
    else{
        cout << endl << "%%%OUTPUT" << endl;
        cout << game;
    }
    
    return 0;
    
}

ostream & operator<<(ostream & os, const Game & game)  {
    os << game.score << '\n' << game.language.getLanguage() << '\n';
    game.tiles.print(os);
    os << game.player.size() << " " << toUTF(game.player.to_string()) << '\n';
    os << game.bag.size() << " " << toUTF(game.bag.to_string()) << endl;
    return os;
}

istream & operator>>(istream & is, Game &game) {
    string aux = "", lang="";
    int relleno;
    is >> aux;
    if(aux!=PASSWORD)
        errorBreak(ERROR_DATA, "");
    is >> game.score >> lang;
    if(is.eof())
        return is;
    is >> game.tiles >> relleno >> aux;
    if(is.eof())
        return is;
    game.player.add(toISO(aux));
    is >> relleno >> aux;
    if(is.eof())
        return is;
    game.bag.set(toISO(aux));
    game.language.setLanguage(lang);
    return is;
}

void errorBreak(int errorcode, const string &errordata) {
    cerr << endl << "%%%OUTPUT" << endl;
    switch(errorcode) {
        case ERROR_ARGUMENTS:
            cerr<<"Error in call. Please use either:"<<endl;
            cerr<< "-l <language> -w <width> -h <height> [-r <randomnumber> -save <matchfile>]"<<endl;
            cerr<< "-open <matchfile> [-save <matchfile>]"<<endl;            
            break;
        case ERROR_OPEN:
            cerr<<"Error opening file "<<errordata << endl;
            break;
        case ERROR_DATA:
            cerr<<"Data error in file "<<errordata << endl;
            break;
        case GENERAL_ERROR:
            cerr<<"Error: "<<errordata << endl;
            break;
    }
    std::exit(1);
}

