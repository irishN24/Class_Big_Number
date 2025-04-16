#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <random>
#define BASE_SIZE (sizeof(Base) * 8) // размер Base в битах
typedef unsigned char Base;
typedef unsigned short int D_Base;
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
        coef = new Base[max];
        for (int i = 0; i < maxlen; ++i) {
            coef[i] = 0;
        }
        if (randomz != 0) {
            mt19937 mt;
            mt.seed(time(nullptr));
            for (int i = 0; i < maxlen; ++i) {
                coef[i] = mt();
                if (BASE_SIZE > 12) {
                    for (int j = 1; j < ceil(BASE_SIZE / 12); j++) {
                        coef[i] = coef[i] << (j * 12) | mt();
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
    //operator =
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
            }
        }
        else if (len <= 1) {
            result = '0';
        }
        return result;

    }
    void HEX_TO_BN(string& str_16) {
        int length = str_16.length();
        int n = 0;
        for (int i = 0; i < length && str_16[i] != 0; i++) {            
             n++;           
        }
        for (int i = 0; i < length - n; ++i) {
            str_16[i] = str_16[i + n];
        }
        length -= n; 
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
            else if (coef[i] < x.coef[i]){
                return false;
            }
        }
        return false;
    }
    bool operator <(const Big_Number& x) {
        if (len < x.len) {
            return true;
        }
        else if(len > x.len){
            return false;
        }
        for (int i = len - 1; i > -1; i--) {
            if (coef[i] < x.coef[i]) {
                return true;
            }
            else if (coef[i] > x.coef[i]){
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
            else if(coef[i] < x.coef[i]){
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
            else if(coef[i] > x.coef[i]) {
                return false;
            }
        }
        return true;
    }
};

int main() {
    Big_Number Num1;
    Big_Number Num2(12);
    Big_Number Num3(12, 1);
    cout << "Big_Num1: " << Num1.Big_Num_To_HEX() << "\n";
    cout << "Big_Num2: " << Num2.Big_Num_To_HEX() << "\n";
    cout << "Big_Num3: " << Num3.Big_Num_To_HEX() << "\n";
    string hexStr = "000000000000001a2b3c4d0000000000000000";
    Num1.HEX_TO_BN(hexStr);
    cout << "Hex representation: " << Num1.Big_Num_To_HEX() << "\n";
    Big_Number Num4(5, 1);
    Big_Number Num5(12, 1);
    if (Num4 == Num5) {
        cout << "Num 4: " << Num4.Big_Num_To_HEX() << " and Num5: " << Num5.Big_Num_To_HEX() << " are equal\n";
    }
    else if (Num4 > Num5) {
        cout << "Num 4: " << Num4.Big_Num_To_HEX() << " > Num5: " << Num5.Big_Num_To_HEX() << "\n";
    }
    else if (Num4 < Num5) {
        cout << "Num 4: " << Num4.Big_Num_To_HEX() << " < Num5: " << Num5.Big_Num_To_HEX() << "\n";
    }
    Num4 = Num5;
    if (Num4 >= Num5) {
        cout << "Num 4: " << Num4.Big_Num_To_HEX() << " >= Num5: " << Num5.Big_Num_To_HEX() << "\n";
    }
    else {
        cout << "Num 4: " << Num4.Big_Num_To_HEX() << " != Num5: " << Num5.Big_Num_To_HEX() << "\n";
    }
    return 0;
}
