#include <iostream>
#include <string>
#include <cstring>
#include <iomanip>
#include <cmath>

using namespace std;

string floatToBits(float f) {// Convertir float a string binaria de 32 bits
    unsigned char bytes[4];

    memcpy(bytes, &f, sizeof(float));
    
    string bin = "";
    for (int i = 3; i >= 0; --i) {
        for (int j = 7; j >= 0; --j) {

            unsigned char byte = bytes[i];//Tomado un byte en la posición i
            unsigned char shifted = byte >> j;//Corrimiento derecha j posiciones
            unsigned char bit = shifted & 1;//Se aplica un AND con 1 para quedarnos solo con el último bit
            
            char c; //Se convierte ese bit a carácter 1 o 0
            if (bit == 1) {
                c = '1';
            } else {
                c = '0';
            }
            bin += c;//Se une al string
        }
    }
    return bin;
}

void descomponer(const string& bits, int &signo, int &exponente, unsigned int &mantisa) {
    signo = (bits[0] == '1');
    exponente = stoi(bits.substr(1, 8), nullptr, 2);
    mantisa = stoi(bits.substr(9), nullptr, 2);
}

int main() {
    cout.precision(8);
    float a, b;
    cout<<"Ingrese primer float: ";
    cin>>a;
    cout<<"Ingrese segundo float: ";
    cin>>b;

    string bitsA = floatToBits(a);
    string bitsB = floatToBits(b);

    int signoA, signoB, expA, expB;
    unsigned int mantA, mantB;
    descomponer(bitsA, signoA, expA, mantA);
    descomponer(bitsB, signoB, expB, mantB);

    cout<<endl<<"Numero A = "<<a<<endl<<"Bits: "<<bitsA<<endl;
    cout<<"Signo: "<<signoA<<" Exponente: "<<expA<<" Mantisa: "<<mantA<<endl;

    cout<<endl<<"Numero B = "<<b<<endl<<"Bits: "<<bitsB<<endl;
    cout<<"Signo: "<<signoB<<" Exponente: "<<expB<<" Mantisa: "<<mantB<<endl;

    if (a == 0.0f || b == 0.0f) { //Verificar si alguno es 0
        
        cout<<"Resultado desde c++: "<<(a*b)<<endl;
        cout<<"Bits reales desde c++: "<<floatToBits(a*b)<<endl;

        cout<<endl<<"Resultado final: 00000000000000000000000000000000 (cero)"<<endl;
        return 0;
    }

    int signoRes = signoA ^ signoB;//Signo del resultado
    int expRes = (expA - 127) + (expB - 127) + 127;//exponentes con bias = 127

    if (expRes >= 255) {    //overflow/underflow en el exponente
        cout<<endl<<"Overflow de exponente -> Resultado = Infinito"<<endl;
        string res = (signoRes ? "1":"0") + string("11111111") + string(23, '0');
        cout<<"Resultado final: " <<res<<endl;
        return 0;
    }
    if (expRes <= 0) {
        cout << "\n*** Underflow de exponente -> Resultado = 0 ***" << endl;
        string res = (signoRes ? "1" : "0") + string(31, '0');
        cout<<"Resultado final: "<<res<<endl;
        return 0;
    }

    //multiplicación de significandos con un 1 implícito
    double mA = 1.0 + (double)mantA / (1 << 23);
    double mB = 1.0 + (double)mantB / (1 << 23);
    double mRes = mA * mB;

    //Normalizar
    if (mRes >= 2.0) {
        mRes /= 2.0;
        expRes++;
    }

    //Redondear mantisa 23 bits
    unsigned int mantRes = (unsigned int)round((mRes - 1.0) * (1 << 23));

    //Concatenar los resultados
    string res = "";
    res += (signoRes ? "1" : "0");

    string expBin = "";
    for (int i = 7; i >= 0; --i){
        expBin += ((expRes >> i) & 1) ? '1' : '0';
    }
    res += expBin;

    string mantBin = "";
    for (int i = 22; i >= 0; --i){
        mantBin += ((mantRes >> i) & 1) ? '1' : '0';
    }
    res += mantBin;

    float real = a * b;
    cout<<endl<<"Resultado lo que se espera desde el propio C++: "<<real<<endl;
    cout<<"Bits reales del float desde el propio C++: "<<floatToBits(real)<<endl;

    cout<<endl<<"Resultado final, en binario: "<<res<<endl;



    return 0;
}
