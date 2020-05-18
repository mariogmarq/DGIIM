/**
 * @file player.cpp
 * @author DECSAI
 * @note To be implemented by students
 */

#include <cstring>
#include <string>
#include "player.h"
///@warning complete missing #includes
/// See http://www.cplusplus.com/reference/cstring/ for details about cstrings
using namespace std;

/**
 * @brief Removes a position from a cstring
 * @param cstr The cstring
 * @param pos The position
 * @return The cstring is modified
 * @warning To be fully implemented
 */
void removeCString(char *cstr, int pos);

/**
 * @brief Sort a cstring from A to Z
 * @param cstr The cstring
 * @return The cstring is modified
 * @warning To be fully implemented
 */
void sortCString(char *s);


//Auxiliar methods

void removeCString(char *cstr, int pos){
    for(int i = pos; i <= MAXPLAYER; i++)
        cstr[i] = cstr[i+1];
}


void sortCString(char *s){
    for(int i = 0; s[i] != '\0'; i++){
        for(int j = 0; s[j]!='\0';j++){
            if(s[j] > s[j+1] && s[j+1]!='\0'){
                char temp=s[j];
                s[j] = s[j+1];
                s[j+1]=temp;
            }
        }
    }
}

//Player methods

Player::Player(){
    for(int i = 0; i <= MAXPLAYER; i++)
        letters[i] = '\0';
}


int Player::size() const{
    return strlen(letters);   
}

string Player::to_string() const{
    string str(letters);
    return str;
}

void Player::clear() {
    for(int i = 0; i <= MAXPLAYER; i++)
        letters[i]='\0';
}


bool Player::isValid(const string s) const {    
    char check[MAXPLAYER+1];
    strcpy(check, letters);
    for(int i = 0; i < s.size();i++){
        bool buscando = true;
        for(int j = 0; check[j]!='\0' && buscando;j++){
            if(check[j] == s[i]){
                buscando = false;
                removeCString(check, j);
            }    
        }
        if(buscando)
            return false;
    }
    return true;
}

bool Player::extract(const string s) {
    char check[MAXPLAYER+1];
    strcpy(check, letters);
    
    
    //No llamo a isValid ya que sigue el mismo algoritmo
    for(int i = 0; i < s.size();i++){
        bool buscando = true;
        for(int j = 0; check[j]!='\0' && buscando;j++){
            if(check[j] == s[i]){
                buscando = false;
                removeCString(check, j);
            }    
        }
        if(buscando)
            return false;
    }
    
    strcpy(letters, check);
    return true;
}


void Player::add(string frombag){
    if((strlen(letters) + frombag.size()) <= MAXPLAYER){
        strcat(letters,frombag.c_str());
        sortCString(letters);
    }
}