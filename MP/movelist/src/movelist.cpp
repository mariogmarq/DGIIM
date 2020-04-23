/**
 * @file movelist.cpp
 * @author DECSAI
 * @note To be implemented by students
 * @warning Complete the code
 *  */
#include <iostream>
#include <cassert>
#include "movelist.h"
#include "move.h"
#include "language.h"

using namespace std;


bool Movelist::print(std::ostream &os, bool scores) const {
    bool res=true;
    for (int i=0; i<size() && res; i++) {
        get(i).print(os);
        if (scores)
            os << " ("<<get(i).getScore()<<") - ";
        else
            os << " - ";
        if (os.bad())
            res=false;
    }
    return res;
}

void Movelist::allocate(int n){
    if(moves != nullptr)
        deallocate();
    moves = new Move[n];
}
void Movelist::deallocate(){
    if(moves != nullptr){
        delete[] moves;
    }
}
Movelist::Movelist(){
    moves = nullptr;
    nMove = 0;
}
Movelist::Movelist(int nmov){
    moves = nullptr;
    allocate(nmov);
    nMove = nmov;
}
Movelist::Movelist(const Movelist& orig) {
    moves = nullptr;
    nMove = 0;
    copy(orig);
}
Movelist::~Movelist(){
    deallocate();
}
Move Movelist::get(int p) const{
    assert(p >=0 && p < size());
    return moves[p];
}
void Movelist::set(int p, const Move& m){
    assert(p >=0 && p < size());
    moves[p]=m;
}
int Movelist::find(const Move& mov) const{
    for(int i = 0; i < size(); i++){
        if(get(i).getCol()==mov.getCol() && get(i).getRow()==mov.getRow()){
            if(get(i).getLetters()==mov.getLetters() && get(i).isHorizontal()==mov.isHorizontal()){
                return i;
            }
        }
    }
    return -1;
}
void Movelist::copy(const Movelist& ml){
    if(moves != nullptr)
        deallocate();
    nMove = ml.size();
    allocate(nMove);
    for(int i = 0; i < nMove; i++)
        set(i, ml.get(i));
}
void Movelist::add(const Move& mov){
    nMove++;
    Move *temp = new Move[nMove];
    for(int i = 0; i < size()-1;i++)
    temp[i] = get(i);
    temp[nMove-1]=mov;
    deallocate();
    moves = temp;
}
void Movelist::remove(int p){
    assert(p>=0 && p<size());
    Move *temp = new Move[--nMove];
    for(int i = p; i < size(); i++)
        moves[i] = moves[i+1];
    for(int i = 0; i < nMove; i++)
        temp[i] = moves[i];
    deallocate();
    moves = temp;
}
void Movelist::remove(const Move& mov){
    int pos=find(mov);
    if(pos!=-1)
        remove(pos);
}
void Movelist::zip(const Language& l){
    int i = 0;
    while(i < size()){
        if(get(i).getLetters().length()<2 || !(l.query(get(i).getLetters()))){
            remove(i);
        }
        else{
            i++;
        }
    }
}
int Movelist::getScore() const{
    int suma = 0;
    int score = 0;
    for(int i = 0; i < size(); i++){
        score = get(i).getScore();
        if(score == -1)
            return -1;
        else
            suma+=score;
    }
    return suma;
}
void Movelist::clear(){
    deallocate();
    moves = nullptr;
    nMove = 0;
}
Movelist Movelist::operator =(const Movelist& orig){
    if(this != &orig)
        copy(orig);
    return *this;
}

bool Movelist::read(std::istream& is){
    Move mov;
    string l="";
    char h='h';
    int col = 0, row = 0;
    do{
        if(is.eof() || !is)
            return false;
        try{
            is >> h;
            is >> row;
            is >> col;
            is >> l;
            if(l!="@"){
                mov.set(row, col, h, l);
                add(mov);
            }
        }
        catch(...){
            return false;
        }
    }while(l!="@");
    return true;
}