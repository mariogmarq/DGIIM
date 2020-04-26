#include <iostream>
#include <fstream>
using namespace std;


bool contenidoen(string s, string* c, int tam){
    for(int i = 0; i < tam; i ++){
        if(s==c[i])
            return true;
    }
    return false;
}

void Errorbreak(string error) {
    cerr << error;
    exit(1);
}

void Leer(string ifilename, string* &array1, int &tam1, string* &array, int &tam2){
    ifstream file;
    file.open(ifilename);
    if(!file)
        Errorbreak("Error al abrir "+ ifilename);
    file >> tam1;
    if((file.eof() && tam1 != 0)||tam1 < 0)
        Errorbreak("Error al leer");
    array1 = new string[tam1];
    for(int i = 0; i < tam1; i++){
        if(!file)
            Errorbreak("Error al leer");
        else{
            file >> array1[i];
        }
    }
    file >> tam2;
    if((file.eof() && tam2 != 0)||tam2 < 0)
        Errorbreak("Error al leer");
    array2 = new string[tam2];
    for(int i = 0; i < tam2; i++){
        if(!file)
            Errorbreak("Error al leer");
        else{
            file >> array2[i];
        }
    }
    file.close();
}

void aumentar(string* &array, int &tam, const string &palabra) {
    string* newarray = new string[tam+1];
    for(int i = 0; i < tam; i++){
        newarray[i] = array[i];
    }
    newarray[tam] = palabra;
    delete[]array;
    array = newarray;
    tam++;
}

int main() {
    int tam1, tam2, tam3 = 0;
    string *array1=nullptr, *array2=nullptr, *array3=nullptr;
    Leer("fichero", array1, tam1, array2, tam2);

    for(int i = 0; i < tam1; i++){
        if(contenidoen(array1[i], array2, tam2))
            aumentar(array3, tam3, array1[i]);
    }
    for(int i = 0; i < tam3; i++){
        cout << array3[i] << " ";
    }
    delete[] array1;
    delete[] array2;
    delete[] array3;
    return 0;
}
