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
    row = r;
    column = c;
    if(h == 'H' || h == 'h')
        ishorizontal = true;
    else if(h == 'V' || h == 'v')
        ishorizontal = false;
    letters = l;
}

void Move::setRow(int r){
    row = r;
}

void Move::setCol(int c){
    column = c;
}

void Move::setHorizontal(char h){
    if(h == 'H')
        ishorizontal = true;
    else if(h == 'V')
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
    if(ishorizontal)
        os << "H ";
    else
        os << "V ";
    os << row << " " << column <<  " " << letters;
}

void Move::read(std::istream& is){
    char h;
    is >> h;
    setHorizontal(h);
    is >> row;
    is >> column;
    is >> letters;
    letters = normalizeWord(letters);
}

std::ostream& operator<<(std::ostream& os, const Move & m){
    m.print(os);
    return os;
}

std::istream& operator>>(std::istream& is, Move & m){
    m.read(is);
    return is;
}