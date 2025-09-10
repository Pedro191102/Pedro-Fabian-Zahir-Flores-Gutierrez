#include <iostream>
#include <vector>
#include <string>

using namespace std;

//Convierte un string a vector de bits
vector<int> binarioALista(const string& binStr, int n) {
    vector<int> binario(n, 0);
    int len = binStr.size();
    for(int i = 0; i < len; ++i) {
        if (binStr[i] == '1') {
            binario[n - len + i] = 1;
        } else {
            binario[n - len + i] = 0;
        }
    }
    return binario;
}
//Complemento a 2 de un vector binario
vector<int> complemento2(const vector<int>& binario) {
    vector<int> resultado = binario;
    for(int &b : resultado) b = 1 - b; //invierte los bits

    int carry = 1;
    for(int i = resultado.size()-1; i >= 0; i--) {
        int s = resultado[i] + carry;
        resultado[i] = s % 2;
        carry = s / 2;
    }
    return resultado;
}
// Suma binaria de dos vectores
vector<int> sumaBinaria(const vector<int>& a, const vector<int>& b) {
    int n = a.size();
    vector<int> sum(n, 0);
    int carry = 0;
    for(int i = n-1; i >= 0; i--) {
        int s = a[i] + b[i] + carry;
        sum[i] = s % 2;
        carry = s / 2;
    }
    return sum;
}
//Corrimiento derecho de A y Q, y actualizando Q-1
void Corrimiento(vector<int>& A, vector<int>& Q, int &Q_1) {
    int n = A.size();
    int tempA0 = A[0];
    Q_1 = Q.back();
    for(int i = n-1; i > 0; i--) Q[i] = Q[i-1];
    Q[0] = A.back();
    for(int i = n-1; i > 0; i--) A[i] = A[i-1];
    A[0] = tempA0;
}
//Algoritmo de Booth
vector<int> Algoritmobooth(const vector<int>& M, const vector<int>& Q) {
    int n = M.size();
    vector<int> A(n,0);
    vector<int> multiplicador = Q;
    vector<int> M_neg = complemento2(M);
    int Q_1 = 0;

    cout << "Iteracion: A Q Q-1\n";
    for(int i = 0; i < n; i++) {
        if(multiplicador.back() == 1 && Q_1 == 0)
            A = sumaBinaria(A, M_neg);
        else if(multiplicador.back() == 0 && Q_1 == 1)
            A = sumaBinaria(A, M);

        Corrimiento(A, multiplicador, Q_1);

        cout<<i+1<<": ";
        for(int bit : A){
            cout<<bit;
        }
        cout<<" ";
        for(int bit : multiplicador){
            cout << bit;
        }
        cout<<" "<<Q_1 << "\n";
    }

    vector<int> resultado(2*n);
    for(int i = 0; i < n; i++) resultado[i] = A[i];
    for(int i = 0; i < n; i++) resultado[n + i] = multiplicador[i];

    return resultado;
}

int main() {
    int n;
    string M_string, Q_string;
    cout<<"Ingrese cantidad de bits n: ";
    cin >> n;
    cout<<"Multiplicando (binario de " <<n<< " bits): ";
    cin>>M_string;
    cout<<"Multiplicador (binario de " <<n<< " bits): ";
    cin>>Q_string;

    vector<int> M = binarioALista(M_string, n);
    vector<int> Q = binarioALista(Q_string, n);

    vector<int> resultado = Algoritmobooth(M, Q);

    cout<<"Resultado final (" <<2*n<< " bits): ";
    for(int bit : resultado){
        cout << bit;
    }
    cout << "\n";
    return 0;
}
