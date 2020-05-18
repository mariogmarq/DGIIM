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
    for(int i = 0; i < r; i++)
        cell[i] = new char[c];
        
    for(int i = 0; i < r; i++)
        for(int j = 0; j < c; j++)
            cell[i][j] = EMPTY;
}

void Tiles::deallocate(){
    if(cell != nullptr){
        for(int i = 0; i < getHeight(); i++)
            delete[] cell[i];
        delete[] cell;
    }
    cell = nullptr;
}

int Tiles::getHeight() const{
    return rows;
}

int Tiles::getWidth() const{
    return columns;
}

void Tiles::setSize(int r, int c){
    if(r != rows && c != columns){
        deallocate();
        allocate(r, c);
    }
}

Tiles& Tiles::operator =(const Tiles& orig){
    if(this != &orig)
        copy(orig);

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
            //Ensures not overwriting an already exisiting letter
            while(get(m.getRow()-1,m.getCol()+i+j-1)!=EMPTY)
                j++;
            
            //Throw exception before writing out of matrix
            assert(!(m.getRow()>rows || m.getCol()+i+j > columns || m.getRow() < 1 || m.getCol() < 1));
            set(m.getRow()-1, m.getCol()+i+j-1, m.getLetters()[i]);
        }
    } 
    
    //Idem for vertical moves
    else {
        for(int i =0, j = 0; i < m.getLetters().size(); i++){
            while(get(m.getRow()-1+i+j,m.getCol()-1)!=EMPTY)
                j++;
            assert(!(m.getRow()+i+j>rows || m.getCol() > columns || m.getRow() < 1 || m.getCol() < 1));
            set(m.getRow()+i+j-1, m.getCol()-1, m.getLetters()[i]);
        }
    }
}


void Tiles::copy(const Tiles& t){
    if(getHeight() != t.getHeight() || getWidth() != t.getWidth()){
        deallocate();
        allocate(t.getHeight(), t.getWidth());
    }
    
    for(int i = 0; i < t.getHeight(); i++)
        for(int j = 0; j < t.getWidth(); j++)
            set(i, j, t.get(i, j));
}

void Tiles::print(ostream & os) const{
    os << getHeight() << " " << getWidth() << endl;
    for(int i = 0; i < getHeight();i++){
        for(int j = 0; j <getWidth();j++){
            os << toUTF(get(i, j)) << " ";
        }
        os << endl;
    }
    os << endl;
}

bool Tiles::read(istream& is){
    string s="";
    int r, c;
    
    is >> r >> c;
    if(is.eof())
        return false;
    
    setSize(r,c);
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < columns; j++){
            is >> s;
            if(is.eof())
                return false;
            set(i,j,toISO(s)[0]);   //Since toISO only support strings
        }
    }
    
    return true;
}

istream& operator>>(istream& is, Tiles &t){
    t.read(is);
    return is;
}

Tiles& Tiles::operator+=(const Move &orig){
    this->add(orig);
    return *this;
}

Move Tiles::findMaxWord(int r, int c, bool hrz) const {
    Move mov;
    string word = "";
    char ch = 0;
    int i = 0;
    
    if(hrz){
        do{
            ch = get(r, c+i);
            ++i;
            if(ch!=EMPTY)
                word.push_back(ch);
        }while(c+i < columns && ch != EMPTY);
    }
    
    else{
        do{
            ch = get(r+i, c);
            ++i;
            if(ch!=EMPTY)
                word.push_back(ch);
        }while(r+i < rows && ch != EMPTY);
    }
    

    mov.set(r+1, c+1, hrz ? 'H' : 'V', word);
    return mov;
}


Movelist Tiles::findCrosswords(const Move& m, const Language& l) const {
    Movelist list;
    Move aux;
    int pos=-1,desplazo=0;
    //Tiles copy for working more comfortable  
    Tiles tiles(*this);
    
    
    //Checks if movement is out of range
    if(m.getRow()>rows || m.getCol() > columns || m.getRow() < 1 || m.getCol() < 1){
        aux = m;
        aux.setScore(BOARD_OVERFLOW);
        list.add(aux);
        return list;
    }
    
    //If firts position is occuped
    //Checked right here since a get from an out of range position would cause the program to except
    if(get(m.getRow()-1,m.getCol()-1)!=EMPTY){
        aux = m;
        aux.setScore(NOT_FREE);
        list.add(aux);
        return list;
    }
    
    //We add the movement manually since we are looking for more especific errors
    //Another implementation would be adding this to Add() and throwing exceptions as necesary
    if(m.isHorizontal()){
        for(int i =0, j = 0; i < m.getLetters().size(); i++){
            //If movement is out of range
            //Since the next while use a getter is necesary for not excepting
            if(m.getRow()>rows || m.getCol()+i+j > columns || m.getRow() < 1 || m.getCol() < 1){
                aux = m;
                aux.setScore(BOARD_OVERFLOW);
                list.add(aux);
                return list;
            }
            
            //Looks for next available position
            while(tiles.get(m.getRow()-1,m.getCol()+i+j-1)!=EMPTY){
                j++;
                
                //Checked again if is out of range before entering to the matrix
                if(m.getRow()>rows || m.getCol()+i+j > columns || m.getRow() < 1 || m.getCol() < 1){
                    aux = m;
                    aux.setScore(BOARD_OVERFLOW);
                    list.add(aux);
                    return list;
                    }
            }
            //Adding the move
            tiles.set(m.getRow()-1, m.getCol()+i+j-1, m.getLetters()[i]);
        }
    }
    
    else {
        for(int i =0, j = 0; i < m.getLetters().size(); i++){
            
            //If movement is out of range
            //Since the next while use a getter is necesary for not excepting
            if(m.getRow()+i+j>rows || m.getCol() > columns || m.getRow() < 1 || m.getCol() < 1){
                aux = m;
                aux.setScore(BOARD_OVERFLOW);
                list.add(aux);
                return list;
            }

            //Looks for next available position
            while(tiles.get(m.getRow()-1+i+j,m.getCol()-1)!=EMPTY){
                j++;
                
                //Checked again if is out of range before entering to the matrix
                if(m.getRow()+i+j>rows || m.getCol() > columns || m.getRow() < 1 || m.getCol() < 1){
                    aux = m;
                    aux.setScore(BOARD_OVERFLOW);
                    list.add(aux);
                    return list;
                }
            }
            //Adding the move
            tiles.set(m.getRow()+i+j-1, m.getCol()-1, m.getLetters()[i]);
        }
    }
    
    //Finding crosswords
    if(m.isHorizontal()){
        //Checked if the movement is an extension to a previous one
        pos = -1;
        
        for(int i = 1; m.getCol()-i>=0 && pos==-1; i++){
            if(tiles.get(m.getRow()-1,m.getCol()-i) == EMPTY){
                pos = m.getCol()-i+1;
                break;
            }

            if(m.getCol()-i==0)
                pos = 0;
            
        }
        //Find the whole word
        aux = tiles.findMaxWord(m.getRow()-1, pos, true);
        aux.setScore(aux.findScore(l));
        
        //In case that is not in dictionary
        if(aux.getScore()==-1){
            aux.setScore(NONEXISTENT_WORD);
            list.add(aux);
            return list;
        }
        
        //Add to the list
        list.add(aux);
        
        
        //Check the rest of the movement
        desplazo = 0;   //This variable help us to check only the letters of the actual movement
                        //The use of this avoid to count twice the words
        for(int i = 0;  i < m.getLetters().size(); i++){
            while(get(m.getRow()-1, m.getCol()-1+i+desplazo)!=EMPTY)
                ++desplazo;
            
            pos = -1;
            
            //Look for the beginning of the word
            //Same algo that before
            for(int j = 1; pos == -1 && m.getRow()-j-1 >= 0; j++)
                if(get(m.getRow()-j-1, m.getCol()-1+i+desplazo)==EMPTY)
                    pos = m.getRow()-j;
            
            if(pos == -1)
                pos = 0;
            
            //Find the whole crossword
            aux = tiles.findMaxWord(pos, m.getCol()-1+i+desplazo, false);
            
            //In case that the crossword is just a letter means that is the letter of our movement
            if(aux.getLetters().size()>1){
                aux.setScore(aux.findScore(l));
                list.add(aux);
            }
        }
    }
    
    //Vertical case
    else {
        //Checked if the movement is an extension to a previous one
        pos = -1;
        
        for(int i = 1; m.getRow()-i>=0 && pos==-1; i++){
            if(tiles.get(m.getRow()-i,m.getCol()-1) == EMPTY){
                pos = m.getRow()-i+1;
                break;
            }
            
            if(m.getRow()-i==0)
                pos = 0;
            
        }
        //Find the whole word
        aux = tiles.findMaxWord(pos, m.getCol()-1, false);
        aux.setScore(aux.findScore(l));
        
        //In case that is not in dictionary
        if(aux.getScore()==-1){
            aux.setScore(NONEXISTENT_WORD);
            list.add(aux);
            return list;
        }
        
        //Add to the list
        list.add(aux);
        
        //Check the rest of the movement
        desplazo = 0;   //This variable help us to check only the letters of the actual movement
                        //The use of this avoid to count twice the words
        
        for(int i = 0;  i < m.getLetters().size(); i++){
            while(get(m.getRow()-1+i+desplazo, m.getCol()-1)!=EMPTY)
                ++desplazo;
            
            pos = -1;
            
            //Look for the beginning of the word
            //Same algo that before
            for(int j = 1; pos == -1 && m.getCol()-j-1 >= 0; j++)
                if(get(m.getRow()-1+i+desplazo, m.getCol()-j-1)==EMPTY)
                    pos = m.getCol()-j;
            
            if(pos==-1)
                pos=0;
            
            //Find the whole crossword
            aux = tiles.findMaxWord(m.getRow()-1+i+desplazo, pos, true);
            
            //In case that the crossword is just a letter means that is the letter of our movement
            if(aux.getLetters().size()>1){
                aux.setScore(aux.findScore(l));
                list.add(aux);
            }
        }
    }
    
    //Return the list
    return list;
}