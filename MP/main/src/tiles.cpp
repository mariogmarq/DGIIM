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
    assert(r >= 0 && r < getHeight());
    assert(c >= 0 && c < getWidth());
    return cell[r][c];
}

void Tiles::set(int r, int c, char l){
    assert(r >= 0 && r < getHeight());
    assert(c >= 0 && c < getWidth());
    cell[r][c]=l;
}

void Tiles::add(const Move& m){
    if(m.isHorizontal()){
        for(int i =0, j = 0; i < m.getLetters().size(); i++){
            while(get(m.getRow()-1,m.getCol()+i+j-1)!=EMPTY)
                j++;
            if(m.getRow()>rows || m.getCol()+i+j > columns || m.getRow() < 1 || m.getCol() < 1)
                return;
            set(m.getRow()-1, m.getCol()+i+j-1, m.getLetters()[i]);
        }
    } else {
        for(int i =0, j = 0; i < m.getLetters().size(); i++){
            while(get(m.getRow()-1+i+j,m.getCol()-1)!=EMPTY)
                j++;
            if(m.getRow()+i+j>rows || m.getCol() > columns || m.getRow() < 1 || m.getCol() < 1)
                return;
            set(m.getRow()+i+j-1, m.getCol()-1, m.getLetters()[i]);
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
    for(int i = 0; i < t.getHeight(); i++){
        for(int j = 0; j < t.getWidth(); j++){
            temp[i][j] = t.get(i, j);
        }
    }
    deallocate();
    cell = temp;
}

void Tiles::print(std::ostream & os) const{
    os << getHeight() << " " << getWidth() << std::endl;
    for(int i = 0; i < getHeight();i++){
        for(int j = 0; j <getWidth();j++){
            os << toUTF(get(i, j)) << " ";
        }
        os << std::endl;
    }
    os << std::endl;
}

bool Tiles::read(std::istream& is) {
    int r, c;
    is >> r >> c;
    if(is.eof())
        return false;
    setSize(r,c);
    
    string s;
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < columns; j++){
            is >> s;
            if(is.eof())
                return false;
            set(i,j,toISO(s)[0]);
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

Move Tiles::findMaxWord(int r, int c, bool hrz) const {
    string word = "";
    char ch = 0;
    int i = 0;
    if(hrz){
        do{
            ch = get(r, c+i);
            ++i;
            if(ch!=EMPTY)
                word.push_back(ch);
        }while(ch!=EMPTY && c+i<columns);
    } 
    else{
        do{
            ch = get(r+i, c);
            ++i;
            if(ch!=EMPTY)
                word.push_back(ch);
        }while(ch!=EMPTY && r+i<rows);
    }
    
    Move m;
    ch = hrz ? 'H' : 'V';
    m.set(r+1, c+1, ch, word);
    return m;
}


Movelist Tiles::findCrosswords(const Move& m, const Language& l) const {
    Movelist list;
    Move aux;
    int pos=-1,desplazo=0;
    //Copiamos tiles    
    Tiles tiles(*this);
    
    //Anaidimos el movimiento
    if(m.isHorizontal()){
        for(int i =0, j = 0; i < m.getLetters().size(); i++){
            if(m.getRow()>rows || m.getCol()+i+j > columns || m.getRow() < 1 || m.getCol() < 1){
                aux = m;
                aux.setScore(BOARD_OVERFLOW);
                list.add(aux);
                return list;
            }
            if(get(m.getRow()-1,m.getCol()-1)!=EMPTY){
                aux = m;
                aux.setScore(NOT_FREE);
                list.add(aux);
                return list;
            }
            while(tiles.get(m.getRow()-1,m.getCol()+i+j-1)!=EMPTY){
                j++;
                if(m.getRow()>rows || m.getCol()+i+j > columns || m.getRow() < 1 || m.getCol() < 1){
                    aux = m;
                    aux.setScore(BOARD_OVERFLOW);
                    list.add(aux);
                    return list;
                }
            }
            
            tiles.set(m.getRow()-1, m.getCol()+i+j-1, m.getLetters()[i]);
        }
    } else {
        for(int i =0, j = 0; i < m.getLetters().size(); i++){
            if(m.getRow()+i+j>rows || m.getCol() > columns || m.getRow() < 1 || m.getCol() < 1){
                aux = m;
                aux.setScore(BOARD_OVERFLOW);
                list.add(aux);
                return list;
            }
            if(get(m.getRow()-1,m.getCol()-1)!=EMPTY){
                aux = m;
                aux.setScore(NOT_FREE);
                list.add(aux);
                return list;
            }
            while(tiles.get(m.getRow()-1+i+j,m.getCol()-1)!=EMPTY){
                j++;
                if(m.getRow()+i+j>rows || m.getCol() > columns || m.getRow() < 1 || m.getCol() < 1){
                    aux = m;
                    aux.setScore(BOARD_OVERFLOW);
                    list.add(aux);
                    return list;
                }
            }
           
            tiles.set(m.getRow()+i+j-1, m.getCol()-1, m.getLetters()[i]);
        }
    }
    
    //Pasamos a buscar las palabras
    if(m.isHorizontal()){
        //Primero comprobamos si es una extension de una palabra
        pos = -1;
        for(int i = 1; m.getCol()-i>=0 && pos==-1; i++){
            if(tiles.get(m.getRow()-1,m.getCol()-i) == EMPTY){
                pos = m.getCol()-i+1;
                break;
            }
            //Buscamos la posicion para nosotros, no para el usuario
            if(m.getCol()-i==0){
                pos = 0;
                break;
            }
            
        }
        aux = tiles.findMaxWord(m.getRow()-1, pos, true);
        aux.setScore(aux.findScore(l));
        if(aux.getScore()==-1){
            aux.setScore(NONEXISTENT_WORD);
            list.add(aux);
            return list;
        }
        list.add(aux);
        
        
        //Miramos el resto del movimiento
        desplazo = 0;
        for(int i = 0;  i < m.getLetters().size(); i++){
            while(get(m.getRow()-1, m.getCol()-1+i+desplazo)!=EMPTY)
                ++desplazo;
            pos = -1;
            //buscamos donde empiezan las letras
            for(int j = 1; pos == -1 && m.getRow()-j-1 >= 0; j++){
                if(get(m.getRow()-j-1, m.getCol()-1+i+desplazo)==EMPTY)
                    pos = m.getRow()-j;
            }
            if(pos==-1){
                pos= 0;
            }
            aux = tiles.findMaxWord(pos, m.getCol()-1+i+desplazo, false);
            if(aux.getLetters().size()>1){
                aux.setScore(aux.findScore(l));
                list.add(aux);
            }
        }
    }
    
    //Idem para vertical
    else {
        //Primero comprobamos si es una extension de una palabra
        pos = -1;
        for(int i = 1; m.getRow()-i>=0 && pos==-1; i++){
            if(tiles.get(m.getRow()-i,m.getCol()-1) == EMPTY){
                pos = m.getRow()-i+1;
                break;
            }
            if(m.getRow()-i==0){
                pos = 0;
                break;
            }
            
        }
        aux = tiles.findMaxWord(pos, m.getCol()-1, false);
        aux.setScore(aux.findScore(l));
        if(aux.getScore()==-1){
            aux.setScore(NONEXISTENT_WORD);
            list.add(aux);
            return list;
        }
        list.add(aux);
        
        //Miramos el resto del movimiento
        desplazo = 0;
        for(int i = 0;  i < m.getLetters().size(); i++){
            while(get(m.getRow()-1+i+desplazo, m.getCol()-1)!=EMPTY)
                ++desplazo;
            pos = -1;
            //buscamos donde empiezan las letras
            for(int j = 1; pos == -1 && m.getCol()-j-1 >= 0; j++){
                if(get(m.getRow()-1+i+desplazo, m.getCol()-j-1)==EMPTY)
                    pos = m.getCol()-j;
            }
            if(pos==-1)
                pos=0;
            aux = tiles.findMaxWord(m.getRow()-1+i+desplazo, pos, true);
            if(aux.getLetters().size()>1){
                aux.setScore(aux.findScore(l));
                list.add(aux);
            }
        }
    }
    
    return list;
}