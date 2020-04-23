/**
 * @file move.cpp
 * @author DECSAI
 * @note To be implemented by students
 */
#include <string>
#include "player.h"
#include "move.h"
#include "language.h"
using namespace std;

Move::Move() {
    row = column = 0;
    ishorizontal = true;
    letters = "";
    score = 0;
}

void Move::set(int r, int c, char h, const string &l) {
    setRow(r);
    setCol(c);
    setHorizontal(h);
    setLetters(l);
}

void Move::setRow(int r){
    if(r >= 0)
        row = r;
}

void Move::setCol(int c){
    if(c >= 0)
        column = c;
}

void Move::setHorizontal(char h){
    if(h == 'H' || h == 'h')
        ishorizontal = true;
    else if(h == 'V' || h == 'v')
        ishorizontal = false;
}

void Move::setLetters(const string &l) {
    letters = l;
}

int Move::findScore(const Language &l) const {
    int score = 0;
    if(!l.query(letters))
        score = -1;
    else {
        for(int i = 0; i < letters.size(); i++)
            score+=l.getScore(letters[i]);
    }
    return score;
}

void Move::setScore(int s){
    score = s;
}

int Move::getScore() const{
    return score;
}

int Move::getRow() const{
    return row;
}

int Move::getCol() const{
    return column;
}

bool Move::isHorizontal() const{
    return ishorizontal;
}

string Move::getLetters() const{
    return letters;
}

void Move::print(std::ostream &os) const{
    os << "row: " << row << " column: " << column << " isHorizontal: " << ishorizontal << " letters: " << letters;
}

void Move::read(std::istream& is){
    char h;
    int c, r;
    string l;
    is >> h;
    is >> r;
    is >> c;
    is >> l;
    set(r, c, h, l);
}
