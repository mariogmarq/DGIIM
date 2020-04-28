/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <iostream>
#include <cassert>
#include "move.h"
#include "movelist.h"
#include "tiles.h"

using namespace std;

Tiles::Tiles(){
    cell = nullptr;
    columns = rows = 0;
}

Tiles::Tiles(int r, int c){
    cell = nullptr;
    allocate(r, c);
}

Tiles::Tiles(const Tiles& orig){
    cell = nullptr;
    copy(orig);
}

Tiles::~Tiles(){
    deallocate();
}

void Tiles::allocate(int r, int c){
    if(cell != nullptr)
        deallocate();
    rows = r;
    columns = c;
    cell = new char* [r];
    for(int i = 0; i < r; i++){
        cell[i] = new char[c];
    }
    for(int i = 0; i < r; i++)
        for(int j = 0; j < c; j++)
            cell[i][j] = EMPTY;
}

void Tiles::deallocate(){
    if(cell != nullptr){
        for(int i = 0; i < getHeight(); i++){
            delete[] cell[i];
        }
        delete[] cell;
    }
}

int Tiles::getHeight() const{
    return rows;
}

int Tiles::getWidth() const{
    return columns;
}

void Tiles::setSize(int r, int c){
    deallocate();
    allocate(r, c);
}

Tiles& Tiles::operator =(const Tiles& orig){
    if(this != &orig){
        copy(orig);
    }
    return *this;
}

char Tiles::get(int r, int c) const{
    assert(r >= 1 && r < getHeight()+1);
    assert(c >= 1 && c < getWidth()+1);
    return cell[r-1][c-1];
}

void Tiles::set(int r, int c, char l){
    assert(r >= 1 && r < getHeight()+1);
    assert(c >= 1 && c < getWidth()+1);
    cell[r-1][c-1]=l;
}

void Tiles::add(const Move& m){
    if(m.isHorizontal()){
        for(int i = 0; i<m.getLetters().size(); i++){
            if(m.getRow() >= 1 && m.getRow() < getHeight()+1)
                if(m.getCol()+i >= 1 && m.getCol()+i < getWidth()+1)
                set(m.getRow(), m.getCol()+i, m.getLetters()[i]);
        }
    }
    if(!m.isHorizontal()){
        for(int i = 0; i<m.getLetters().size(); i++){
            if(m.getRow()+i >= 1 && m.getRow()+i < getHeight()+1)
                if(m.getCol() >= 1 && m.getCol() < getWidth()+1)
                    set(m.getRow()+i, m.getCol(), m.getLetters()[i]);
        }
    }
}


void Tiles::copy(const Tiles& t){
    char **temp = nullptr;
    rows = t.getHeight();
    columns = t.getWidth();
    temp = new char* [t.getHeight()];
    for(int i = 0; i < t.getHeight(); i++){
        temp[i] = new char[t.getWidth()];
    }
    for(int i = 1; i <= t.getHeight(); i++){
        for(int j = 1; j <= t.getWidth(); j++){
            temp[i-1][j-1] = t.get(i, j);
        }
    }
    deallocate();
    cell = temp;
}

void Tiles::print(std::ostream & os) const{
    os << getHeight() << " " << getWidth() << std::endl;
    for(int i = 1; i <= getHeight();i++){
        for(int j = 1; j <= getWidth();j++){
            os << get(i, j) << " ";
        }
        os << std::endl;
    }
}

bool Tiles::read(std::istream& is) {
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < columns; j++){
            is >> cell[i][j];
            if(is.eof())
                return false;
        }
    }
    return true;
}

std::istream& operator>>(std::istream& is, Tiles &t){
    t.read(is);
    return is;
}

Tiles& Tiles::operator+=(const Move &orig){
    this->add(orig);
    return *this;
}