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

<<<<<<< HEAD
=======
void LiberarMemoria(string* &arr1, string* &arr2) {
    if(arr1!=nullptr)
        delete [] arra1;
     if(arr!=nullptr)
        delete [] arr2;
}

>>>>>>> 468b4113dde6abbfa413a408b056132c81b9ebb9
void Errorbreak(string error) {
    cerr << error;
    exit(1);
}

<<<<<<< HEAD
void Leer(string ifilename, string* &array1, int &tam1, string* &array, int &tam2){
=======
void Leer(string ifilename, string* &array1, int &tam1, string* &array2, int &tam2){
>>>>>>> 468b4113dde6abbfa413a408b056132c81b9ebb9
    ifstream file;
    file.open(ifilename);
    if(!file)
        Errorbreak("Error al abrir "+ ifilename);
    file >> tam1;
    if((file.eof() && tam1 != 0)||tam1 < 0)
        Errorbreak("Error al leer");
    array1 = new string[tam1];
    for(int i = 0; i < tam1; i++){
<<<<<<< HEAD
        if(!file)
            Errorbreak("Error al leer");
=======
        if(!file){
            LiberarMemoria(array1, array2);
            Errorbreak("Error al leer");
        }
>>>>>>> 468b4113dde6abbfa413a408b056132c81b9ebb9
        else{
            file >> array1[i];
        }
    }
    file >> tam2;
    if((file.eof() && tam2 != 0)||tam2 < 0)
        Errorbreak("Error al leer");
    array2 = new string[tam2];
    for(int i = 0; i < tam2; i++){
<<<<<<< HEAD
        if(!file)
            Errorbreak("Error al leer");
=======
        if(!file){
            LiberarMemoria(array1, array2);
            Errorbreak("Error al leer");
        }
>>>>>>> 468b4113dde6abbfa413a408b056132c81b9ebb9
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
<<<<<<< HEAD
        if(!contenidoen(array1[i], array2, tam2))
=======
        if(contenidoen(array1[i], array2, tam2))
>>>>>>> 468b4113dde6abbfa413a408b056132c81b9ebb9
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
