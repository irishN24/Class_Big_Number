#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <random>
#include <chrono>
#define BASE_SIZE (sizeof(Base) * 8) // размер Base в битах
typedef unsigned char Base;
typedef unsigned short D_Base;
using namespace std;

class Big_Number {
    Base* coef;
    int len;
    int maxlen;
public:
    //конструктор 1 по умолчанию (создает число 0; maxLen = 1)
    //конструктор 2 с параметром (maxLen передаем через параметр и все цифры числа заполняем нулями) и 3
    //конструктор 3 с параметрами (maxLen передаем через параметр и цифры заполняем случайными числами)
    Big_Number(int max = 1, int randomz = 0) : len(max), maxlen(max) {
        if (max <= 0) {
            len = maxlen = 1;
            coef = new Base[1]{ 0 };
            return;
        }
        coef = new Base[max];
        for (int i = 0; i < maxlen; ++i) {
            coef[i] = 0;
        }
        if (randomz != 0) {
            for (int i = 0; i < maxlen; ++i) {
                coef[i] = rand();
                if (sizeof(Base) > 2) {
                    for (int j = 0; j < sizeof(Base) / 2; ++j) {
                        coef[i] = (coef[i] << 16) | rand();
                    }
                }
            }
        }
        while (len > 1 && coef[len - 1] == 0) {
            len--;
        }
    }
    //конструктор копирования
    Big_Number(const Big_Number& B_Num) : len(B_Num.len), maxlen(B_Num.maxlen) {
        coef = new Base[maxlen];
        for (int i = 0; i < maxlen; ++i) {
            coef[i] = B_Num.coef[i];
        }
    }
    //деструктор
    ~Big_Number() {
        delete[] coef;
    }
    Big_Number& operator=(const Big_Number& B_Num) {
        if (this != &B_Num) {
            delete[] coef;
            len = B_Num.len;
            maxlen = B_Num.maxlen;
            coef = new Base[maxlen];
            if (coef) {
                for (int i = 0; i < maxlen; ++i) {
                    coef[i] = B_Num.coef[i];
                }
            }
        }
        return *this;
    }
    string Big_Num_To_HEX() {
        string result;
        if (len > 1) {
            for (int j = len - 1; j >= 0; j--) {
                int k = BASE_SIZE - 4;
                while (k >= 0) {
                    int tmp = (coef[j] >> k) & 0xF;
                    if (tmp >= 0 && tmp <= 9) {
                        result += (char)(tmp + '0');
                    }
                    else if (tmp >= 10 && tmp <= 15) {
                        result += (char)(tmp - 10 + 'a');
                    }
                    k -= 4;
                }
                while (len > 1 && coef[len - 1] == 0) {
                    len--;
                }
            }
        }
        if (len < 0 || coef == nullptr) {
            result = '0';
        }
        size_t k = result.find_first_not_of('0');
        if (k != string::npos) {
            result = result.substr(k);
        }
        else{
            result = '0';
        }
        return result;

    }
    void HEX_TO_BN(string& str_16) {
        int length = str_16.length();
        len = (length - 1) / (BASE_SIZE / 4) + 1;
        maxlen = len;
        delete[] coef;
        coef = new Base[maxlen];
        fill(coef, coef + maxlen, 0);
        if (coef) {
            int j = 0;
            int k = 0;
            for (int i = length - 1; i >= 0; i--) {
                char c = str_16[i];
                int tmp = 0;
                if (c >= '0' && c <= '9') {
                    tmp = c - '0';
                }
                else if (c >= 'a' && c <= 'f') {
                    tmp = c - 'a' + 10;
                }
                else if (c >= 'A' && c <= 'F') {
                    tmp = c - 'A' + 10;
                }
                else {
                    cout << "INCORRECT SYMBOL!!!\n";
                    len = 1;
                    coef[0] = 0;
                    return;
                }
                coef[j] |= tmp << k;
                k += 4;
                if (k >= BASE_SIZE) {
                    k = 0;
                    j++;
                }
            }
            while (len > 1 && coef[len - 1] == 0) {
                len--;
            }
        }
    }
    bool operator ==(const Big_Number& x) {
        if (len != x.len) {
            return false;
        }
        for (int i = 0; i < len; i++) {
            if (coef[i] != x.coef[i]) {
                return false;
            }
        }
        return true;
    }
    bool operator !=(const Big_Number& x) {
        if (len != x.len) {
            return true;
        }
        for (int i = 0; i < len; i++) {
            if (coef[i] != x.coef[i]) {
                return true;
            }
        }
        return false;
    }
    bool operator >(const Big_Number& x) {
        if (len > x.len) {
            return true;
        }
        else if (len < x.len) {
            return false;
        }
        for (int i = len - 1; i > -1; i--) {
            if (coef[i] > x.coef[i]) {
                return true;
            }
            else if (coef[i] < x.coef[i]) {
                return false;
            }
        }
        return false;
    }
    bool operator <(const Big_Number& x) {
        if (len < x.len) {
            return true;
        }
        else if (len > x.len) {
            return false;
        }
        for (int i = len - 1; i > -1; i--) {
            if (coef[i] < x.coef[i]) {
                return true;
            }
            else if (coef[i] > x.coef[i]) {
                return false;
            }
        }
        return false;
    }
    bool operator >=(const Big_Number& x) {
        if (len > x.len) {
            return true;
        }
        else if (len < x.len) {
            return false;
        }
        for (int i = len - 1; i > -1; i--) {
            if (coef[i] > x.coef[i]) {
                return true;
            }
            else if (coef[i] < x.coef[i]) {
                return false;
            }
        }
        return true;
    }
    bool operator <=(const Big_Number& x) {
        if (len < x.len) {
            return true;
        }
        else if (len > x.len) {
            return false;
        }
        for (int i = len - 1; i > -1; i--) {
            if (coef[i] < x.coef[i]) {
                return true;
            }
            else if (coef[i] > x.coef[i]) {
                return false;
            }
        }
        return true;
    }
    Big_Number operator +(const Big_Number& v) {
        int l = min(len, v.len);
        Big_Number w(max(len, v.len) + 1);
        D_Base tmp;
        int j = 0; //Индекс по разрядам
        int k = 0; //перенос

        for (; j < l; j++) {
            tmp = (D_Base)coef[j] + (D_Base)v.coef[j] + (D_Base)k;
            w.coef[j] = (Base)tmp;
            k = (Base)(tmp >> BASE_SIZE);
        }
        for (; j < len; j++) {
            tmp = (D_Base)coef[j] + (D_Base)k;
            w.coef[j] = (Base)tmp;
            k = (Base)(tmp >> BASE_SIZE);
        }
        for (; j < v.len; j++) {
            tmp = (D_Base)v.coef[j] + (D_Base)k;
            w.coef[j] = (Base)tmp;
            k = (Base)(tmp >> BASE_SIZE);
        }
        w.coef[j] = k;
        if (k == 0) {
            w.len = j;
        }
        else {
            w.len = j + 1;
        }
        return w;
    }
    Big_Number& operator+=(const Big_Number& v);
    Big_Number operator -(const Big_Number& v) {
        if (*this >= v) {
            D_Base tmp;
            int j = 0;      // Индекс по коэффициентам
            D_Base k = 0;   // займ
            Big_Number result(len);

            for (; j < v.len; j++) {
                tmp = ((D_Base)1 << BASE_SIZE) | (D_Base)coef[j];
                tmp = (D_Base)(tmp - v.coef[j] - k);
                result.coef[j] = (Base)tmp;
                k = (D_Base)!(tmp >> BASE_SIZE);
            }
            for (; j < len; j++) {
                tmp = ((D_Base)1 << BASE_SIZE) | (D_Base)coef[j];
                tmp -= (D_Base)k;
                result.coef[j] = (Base)tmp;
                k = (D_Base)!(tmp >> BASE_SIZE);
            }
            result.len = len;
            while (result.len > 1 && result.coef[result.len - 1] == 0) {
                result.len--;
            }
            return result;
        }
        else {
            cout << "ERROR: U < V!!!";
            return -1;
        }
    }
    Big_Number& operator -=(const Big_Number& v);
    Big_Number operator *(const Base &a) {
        Big_Number w(len + 1);
        int j = 0;
        Base k = 0;
        D_Base tmp;
        for (; j < len; j++) {
            tmp = (D_Base)coef[j] * (D_Base)a + (D_Base)k;
            w.coef[j] = (Base)tmp;
            k = (Base)(tmp >> BASE_SIZE);
        }
        w.coef[j] = k;
        w.len = len + 1;
        while (w.len > 1 && w.coef[len - 1] == 0) {
            w.len--;
        }
        return w;
    }
    Big_Number& operator *=(const Base &a);
    Big_Number operator *(const Big_Number& v) {
        Big_Number w(len + v.len);
        int j = 0;
        D_Base tmp;
        for (; j < v.len; j++) {
            if (v.coef[j] != 0) {
                Base k = 0;
                int i = 0;
                for (; i < len; i++) {
                    tmp = (D_Base)coef[i] * v.coef[j] + (D_Base)k + (D_Base)w.coef[i + j];
                    w.coef[i + j] = (Base)tmp;
                    k = (Base)(tmp >> BASE_SIZE);
                }
                w.coef[len + j] = k;
            }
        }
        w.len = len + v.len;
        while (w.len > 1 && w.coef[len - 1] == 0) {
            w.len--;
        }
        return w;
    }
    Big_Number& operator *=(const Big_Number &v);
    Big_Number operator /(const Base &a) {
        if (a == 0) {
            throw invalid_argument("DIVISION BY ZEROOOO!!!!");
        }
        Big_Number q(len);
        D_Base r = 0;
        int j = len - 1;
        D_Base tmp;
        for (; j >= 0; j--) {
            tmp = (D_Base)(r << BASE_SIZE) + (D_Base)coef[j];
            q.coef[j] = (Base)(tmp / (D_Base)a);
            r = (Base)(tmp % (D_Base)a);
        }
        q.len = len;
        while (q.len > 1 && q.coef[len - 1] == 0) {
            q.len--;
        }
        return q;
    }
    Big_Number& operator /=(const Base &a);
    Big_Number operator %(const Base& a) {
        if (a == 0) {
            throw invalid_argument("DIVISION BY ZEROOOO!!!!");
        }
        D_Base r = 0;
        for(int i = len - 1; i >= 0; i--){
            r = (D_Base)((r << BASE_SIZE) | coef[i] % a);
        }
        Big_Number res(1);
        res.coef[0] = r;
        return res;
    }
    Big_Number& operator %=(const Base &a);
    friend ostream operator <<(ostream &, const Big_Number &) {


    }
};
Big_Number& Big_Number::operator+=(const Big_Number& v) {
    int l = min(len, v.len);
    Base* tmpSumm = new Base[max(len, v.len) + 1];
    D_Base tmp;
    int j = 0; //Индекс по разрядам
    int k = 0; //перенос

    for (; j < l; j++) {
        tmp = (D_Base)coef[j] + (D_Base)v.coef[j] + (D_Base)k;
        tmpSumm[j] = (Base)tmp;
        k = (Base)(tmp >> BASE_SIZE);
    }
    for (; j < len; j++) {
        tmp = (D_Base)coef[j] + (D_Base)k;
        tmpSumm[j] = (Base)tmp;
        k = (Base)(tmp >> BASE_SIZE);
    }
    for (; j < v.len; j++) {
        tmp = (D_Base)v.coef[j] + (D_Base)k;
        tmpSumm[j] = (Base)tmp;
        k = (Base)(tmp >> BASE_SIZE);
    }
    tmpSumm[j] = k;
    delete[] coef;
    coef = tmpSumm;
    if (k == 0) {
        len = j;
    }
    else {
        len = j + 1;
    }
    return *this;
}
Big_Number& Big_Number::operator -=(const Big_Number& v) {
    if (*this >= v) {
        D_Base tmp;
        int j = 0; // Индекс по коэффициентам
        D_Base k = 0; // займ
        Base* tmpres = new Base[len];

        for (; j < v.len; j++) {
            tmp = ((D_Base)1 << BASE_SIZE) | (D_Base)coef[j];
            tmp = tmp - v.coef[j] - k;
            tmpres[j] = (Base)tmp;
            k = !(tmp >> BASE_SIZE);
        }
        for (; j < len; j++) {
            tmp = (1 << BASE_SIZE) | coef[j];
            tmp -= k;
            tmpres[j] = (Base)tmp;
            k = !(tmp >> BASE_SIZE);
        }
        delete[] coef;
        coef = tmpres;
        while (len > 1 && coef[len - 1] == 0) {
            len--;
        }
        return *this;
    }
    else {
        cout << "ERROR: U < V!!!";
    }
}
Big_Number& Big_Number::operator*=(const Base &a){
    int j = 0;
    Base k = 0;
    D_Base tmp;
    Base *tmpMul = new Base[len + 1];
    for (; j < len; j++) {
        tmp = (D_Base)coef[j] * (D_Base)a + (D_Base)k;
        tmpMul[j] = (Base)tmp;
        k = (Base)(tmp >> BASE_SIZE);
    }
    tmpMul[j] = k;
    int lenght = len + 1;
    while (lenght > 1 && tmpMul[len - 1] == 0) {
        lenght--;
    }
    delete[] coef;
    coef = tmpMul;
    len = lenght;
    return *this;
}
Big_Number& Big_Number::operator*=(const Big_Number &v){
    Base *tmpMul = new Base[len + v.len];
    int j = 0;
    D_Base tmp;
    for (; j < v.len; j++) {
        if (v.coef[j] != 0) {
            Base k = 0;
            int i = 0;
            for (; i < len; i++) {
                tmp = (D_Base)coef[i] * v.coef[j] + (D_Base)k + (D_Base)tmpMul[i + j];
                tmpMul[i + j] = (Base)tmp;
                k = (Base)(tmp >> BASE_SIZE);
            }
            tmpMul[len + j] = k;
        }
    }
    delete[] coef;
    len = len + v.len;
    coef = tmpMul;
    while (len > 1 && coef[len - 1] == 0) {
        len--;
    }
    return *this;
}
Big_Number& Big_Number::operator/=(const Base &a){
    if (a == 0) {
        throw invalid_argument("DIVISION BY ZEROOOO!!!!");
    }
    D_Base r = 0;
    int j = len - 1;
    D_Base tmp;
    Base *tmpDiv = new Base[len];
    for (; j >= 0; j--) {
        tmp = (D_Base)(r << BASE_SIZE) + (D_Base)coef[j];
        tmpDiv[j] = (Base)(tmp / (D_Base)a);
        r = (Base)(tmp % (D_Base)a);
    }
    delete[] coef;
    int lenght = len;
    while (lenght > 1 && tmpDiv[len - 1] == 0) {
        lenght--;
    }
    coef = tmpDiv;
    len = lenght;
    return *this;
}
Big_Number& Big_Number::operator%=(const Base &a){
    if (a == 0) {
        throw invalid_argument("DIVISION BY ZEROOOO!!!!");
    }
    D_Base r = 0;
    for(int i = len - 1; i >= 0; i--){
        r = (D_Base)((r << BASE_SIZE) | coef[i] % a);
    }
    delete[] coef;
    coef = new Base[1];
    len = 1;
    coef[0] = r;
    return *this;
}
int main() {
    srand(time(0));
    /*Big_Number Num1;
    Big_Number Num2(12);
    Big_Number Num3(12, 1);
    cout << "Big_Num1: " << Num1.Big_Num_To_HEX() << "\n";
    cout << "Big_Num2: " << Num2.Big_Num_To_HEX() << "\n";
    cout << "Big_Num3: " << Num3.Big_Num_To_HEX() << "\n";
    string hexStr = "000000000000001a2b3c4d0000000000000000";
    Num1.HEX_TO_BN(hexStr);
    cout << "Hex representation: " << Num1.Big_Num_To_HEX() << "\n";*/

    Big_Number Num4(12, 1);
    cout << "Num 4: " << Num4.Big_Num_To_HEX() << "\n";
    Big_Number Num5(10, 1);
    cout << "Num 5: " << Num5.Big_Num_To_HEX() << "\n";
    /*if (Num4 == Num5) {
        cout << "Num 4 and Num5 are equal\n";
    }
    else if (Num4 > Num5) {
        cout << "Num 4 > Num5\n";
    }
    else if (Num4 < Num5) {
        cout << "Num 4 < Num5\n";
    }*/

    Big_Number Num6 = Num4 + Num5;
    cout << "Num6 = Num4 + Num5 = " << Num6.Big_Num_To_HEX() << "\n";
    /*Num4 += Num6;
    cout <<"Num4 += Num6: " << Num4.Big_Num_To_HEX() << "\n";*/
    Num6 -=Num4;
    cout << "Num6 = " << Num6.Big_Num_To_HEX() << "\n";
    Big_Number Num7(7, 1);
    cout << "Num 7: " << Num7.Big_Num_To_HEX() << "\n";
    Big_Number Num8(5, 1);
    cout << "Num 8: " << Num8.Big_Num_To_HEX() << "\n";
    Big_Number Num9 = Num7 - Num8;
    cout << "Num 9 = Num7 - Num8 = " << Num9.Big_Num_To_HEX() << "\n";
    Num7 -= Num8;
    cout <<"Num7 -= Num8 = " << Num7.Big_Num_To_HEX() << "\n";

    Big_Number Num10 = Num7 % 2;
    Num10 %= 0;
    cout << "Num 10: " << Num10.Big_Num_To_HEX() << "\n";

    return 0;
}
