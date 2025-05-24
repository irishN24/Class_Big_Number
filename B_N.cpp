#include <iostream>
#include <cstring>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <random>
#include <chrono>
#include <algorithm>

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
    bool operator ==(const Big_Number& x) const {
        if (len != x.len) return false;
        for (int i = 0; i < len; i++) {
            if (coef[i] != x.coef[i]) return false;
        }
        return true;
    }
    bool operator !=(const Big_Number& x) const {
        return !(*this == x);
    }
    bool operator >(const Big_Number& x) const {
        if (len > x.len) return true;
        if (len < x.len) return false;
        for (int i = len - 1; i >= 0; i--) {
            if (coef[i] > x.coef[i]) return true;
            if (coef[i] < x.coef[i]) return false;
        }
        return false;
    }
    bool operator <(const Big_Number& x) const {
        if (len < x.len) return true;
        if (len > x.len) return false;
        for (int i = len - 1; i > -1; i--) {
            if (coef[i] < x.coef[i]) return true;
            if (coef[i] > x.coef[i]) return false;
        }
        return false;
    }
    bool operator >=(const Big_Number& x) const {
        if (len > x.len) return true;
        if (len < x.len) return false;
        for (int i = len - 1; i > -1; i--) {
            if (coef[i] > x.coef[i]) return true;
            if (coef[i] < x.coef[i]) return false;
        }
        return true;
    }
    bool operator <=(const Big_Number& x) const {
        if (len < x.len) return true;
        if (len > x.len) return false;
        for (int i = len - 1; i > -1; i--) {
            if (coef[i] < x.coef[i]) return true;
            if (coef[i] > x.coef[i]) return false;
        }
        return true;
    }
    Big_Number operator +(const Big_Number& v) const {
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
        if (k == 0) {
            w.len = j;
        }
        else {
            w.coef[j] = k;
            w.len = j + 1;
        }
        while (w.len > 1 && w.coef[w.len - 1] == 0) {
            w.len--;
        }
        return w;
    }
   /* Big_Number operator +(const Base& v) const{
        if(v == 0){
            return *this;
        }
        int l = sizeof(v);
        Big_Number w(len + 1);
        D_Base tmp = v;
        int j = 0; //Индекс по разрядам
        int k = 0; //перенос

        for (int i = 0; i < len && tmp !=) {
            tmp = (D_Base)coef[j] + (D_Base)v + (D_Base)k;
            w.coef[j] = (Base)tmp;
            k = (Base)(tmp >> BASE_SIZE);
        }
        for (; j < len; j++) {
            tmp = (D_Base)coef[j] + (D_Base)k;
            w.coef[j] = (Base)tmp;
            k = (Base)(tmp >> BASE_SIZE);
        }
        if (k == 0) {
            w.len = j;
        }
        else {
            w.coef[j] = k;
            w.len = j + 1;
        }
        while (w.len > 1 && w.coef[w.len - 1] == 0) {
            w.len--;
        }
        return w;
    }*/
    Big_Number& operator+=(const Big_Number& v);
    Big_Number operator -(const Big_Number& v) const {
        if (*this >= v) {
            Big_Number result(len);
            D_Base tmp;
            int j = 0;      // Индекс по коэффициентам
            D_Base k = 0;   // займ

            for (; j < v.len; j++) {
                tmp = ((D_Base)1 << BASE_SIZE) | (D_Base)coef[j];
                //tmp = (tmp - v.coef[j] - k);
                tmp = (D_Base)(tmp - v.coef[j] - k);
                result.coef[j] = (Base)tmp;
                //k = (tmp >> BASE_SIZE) ? 0 : 1;
                k = (D_Base)!(tmp >> BASE_SIZE);
            }
            for (; j < len; j++) {
                tmp = ((D_Base)1 << BASE_SIZE) | (D_Base)coef[j];
                //tmp -= k;
                tmp -= (D_Base)k;
                result.coef[j] = (Base)tmp;
                //k = (tmp >> BASE_SIZE) ? 0 : 1;
                k = (D_Base)!(tmp >> BASE_SIZE);
            }
            result.len = len;
            while (result.len > 1 && result.coef[result.len - 1] == 0) {
                result.len--;
            }
            return result;
        }
        else {
            throw invalid_argument("ERROR: U < V!!!");
        }
    }
    Big_Number& operator -=(const Big_Number& v);
    Big_Number operator *(const Base& a) const {
        Big_Number w(len + 1);
        int j = 0;
        Base k = 0;
        D_Base tmp;
        for (; j < len; j++) {
            tmp = (D_Base)coef[j] * (D_Base)a + (D_Base)k;
            w.coef[j] = (Base)tmp;
            k = (Base)(tmp >> BASE_SIZE);
        }
        if (k == 0) {
            w.len = j;
        }
        else {
            w.coef[j] = k;
            w.len = j + 1;
        }
        while (w.len > 1 && w.coef[len - 1] == 0) {
            w.len--;
        }
        return w;
    }
    Big_Number& operator *=(const Base &a);
    Big_Number operator *(const Big_Number& v) const {
        Big_Number w(len + v.len);
        int j = 0;
        D_Base tmp;
        for (; j < v.len; j++) {
            if (v.coef[j] != 0) {
                Base k = 0;
                int i = 0;
                for (; i < len; i++) {
                    tmp = (D_Base)coef[i] * (D_Base)v.coef[j] + (D_Base)k + (D_Base)w.coef[i + j];                    
                    w.coef[i + j] = (Base)tmp;
                    k = (Base)(tmp >> BASE_SIZE);
                }
                w.coef[len + j] = k;
            }
        }
        w.len = len + v.len;
        while (w.len > 1 && w.coef[w.len - 1] == 0) {
            w.len--;
        }
        return w;
    }
    Big_Number& operator *=(const Big_Number &v);
    Big_Number operator /(const Base &a) const {
        if (a == 0) {
            throw invalid_argument("DIVISION BY ZERO!!!!");
        }
        Big_Number q(len);
        D_Base r = 0;
        int j = len - 1;
        D_Base tmp;
        for (; j >= 0; j--) {
            tmp = ((D_Base)r << BASE_SIZE) + (D_Base)coef[j];
            q.coef[j] = (Base)(tmp / (D_Base)a);
            r = (Base)(tmp % (D_Base)a);
        }
        q.len = len;
        while (q.len > 1 && q.coef[q.len - 1] == 0) {
            q.len--;
        }
        return q;
    }
    Big_Number& operator /=(const Base &a);
    Big_Number operator %(const Base& a) const{
        if (a == 0) {
            throw invalid_argument("Division by zero in modulo operation!");
        }
        D_Base r = 0;
        for (int i = len - 1; i >= 0; i--) {
            D_Base tmp = ((D_Base)r << BASE_SIZE) + (D_Base)coef[i];
            r = (Base)(tmp % (D_Base)a);
        }
        Big_Number res(1);
        res.coef[0] = (Base)r;
        return res;
    }
    Big_Number& operator %=(const Base &a);
    Big_Number operator /(const Big_Number &v) const{
            if (v.len == 1 && v.coef[0] == 0) {
                throw invalid_argument("Division by zero!");
            }
            if (*this < v) {
                Big_Number res0(1);
                return res0;
            }
            if (v.len == 1) {
                return *this / v.coef[0];
            }
            D_Base b = (D_Base)1 << BASE_SIZE;
            D_Base d = b / ((D_Base)v.coef[v.len - 1] + 1);
            Big_Number u1 = *this;
            u1 *= (Base)d;
            Big_Number v1 = v;
            v1 *= (Base)d;
            if (u1.len == len) {
                u1.maxlen++;
                Base* new_u = new Base[u1.maxlen];
                for (int i = 0; i < u1.len; i++) {
                    new_u[i] = u1.coef[i];
                }
                new_u[u1.len] = 0;
                delete[] u1.coef;
                u1.coef = new_u;
                u1.len++;
            }
            int m = len - v.len;  //
            Big_Number res(m + 1);
            for (int j = m; j >= 0; j--) {
                D_Base q_hat;
                D_Base r_hat;
                D_Base temp = ((D_Base)u1.coef[j + v1.len] << BASE_SIZE) + (D_Base)u1.coef[j + v1.len - 1];
                q_hat = temp / (D_Base)v1.coef[v1.len - 1];
                r_hat = temp % (D_Base)v1.coef[v1.len - 1];
                while (q_hat >= b ||
                       (v1.len >= 2 && q_hat * (D_Base)v1.coef[v1.len - 2] >
                                           (r_hat << BASE_SIZE) + (D_Base)u1.coef[j + v1.len - 2])) {
                    q_hat--;
                    r_hat += (D_Base)v1.coef[v1.len - 1];
                    if (r_hat >= b) break;
                }
                Base k = 0;
                bool borrow = false;
                for (int i = 0; i < v1.len; i++) {
                    D_Base prod = q_hat * (D_Base)v1.coef[i] + (D_Base)k;
                    Base prod_low = (Base)prod;
                    k = (Base)(prod >> BASE_SIZE);
                    if (u1.coef[j + i] >= prod_low) {
                        u1.coef[j + i] -= prod_low;
                    } else {
                        u1.coef[j + i] = u1.coef[j + i] + ((D_Base)1 << BASE_SIZE) - prod_low;
                        borrow = true;
                    }
                }
                if (u1.coef[j + v1.len] >= k) {
                    u1.coef[j + v1.len] -= k;
                } else {
                    borrow = true;
                }
                if (borrow) {
                    q_hat--;
                    Base carry = 0;
                    for (int i = 0; i < v1.len; i++) {
                        D_Base sum = (D_Base)u1.coef[j + i] + (D_Base)v1.coef[i] + (D_Base)carry;
                        u1.coef[j + i] = (Base)sum;
                        carry = (Base)(sum >> BASE_SIZE);
                    }
                    u1.coef[j + v1.len] += carry;
                }
                res.coef[j] = (Base)q_hat;
            }
            res.len = m + 1;
            while (res.len > 1 && res.coef[res.len - 1] == 0) {
                res.len--;
            }
            return res;
    }
    Big_Number operator %(const Big_Number &v) const{
        if (v.len == 1 && v.coef[0] == 0) {
            throw invalid_argument("Division by zero!");
        }
        if (*this < v) {
            return *this;
        }
        if (v.len == 1) {
            return *this % v.coef[0];
        }
        D_Base b = (D_Base)1 << BASE_SIZE;
        D_Base d = b / ((D_Base)v.coef[v.len - 1] + 1);
        Big_Number u1 = *this;
        u1 *= (Base)d;
        Big_Number v1 = v;
        v1 *= (Base)d;
        if (u1.len == len) {
            u1.maxlen++;
            Base* new_u = new Base[u1.maxlen];
            for (int i = 0; i < u1.len; i++) {
                    new_u[i] = u1.coef[i];
            }
            new_u[u1.len] = 0;
            delete[] u1.coef;
            u1.coef = new_u;
            u1.len++;
        }
        int m = u1.len - v1.len;
        for (int j = m; j >= 0; j--) {
            D_Base q_hat;
            D_Base r_hat;
            D_Base temp = ((D_Base)u1.coef[j + v1.len] << BASE_SIZE) + (D_Base)u1.coef[j + v1.len - 1];
            q_hat = temp / (D_Base)v1.coef[v1.len - 1];
            r_hat = temp % (D_Base)v1.coef[v1.len - 1];
            while (q_hat >= b ||
                        (v1.len >= 2 && q_hat * (D_Base)v1.coef[v1.len - 2] >
                            (r_hat << BASE_SIZE) + (D_Base)u1.coef[j + v1.len - 2])) {
                q_hat--;
                r_hat += (D_Base)v1.coef[v1.len - 1];
                if (r_hat >= b) break;
            }
            Base k = 0;
            bool borrow = false;
            for (int i = 0; i < v1.len; i++) {
                D_Base prod = q_hat * (D_Base)v1.coef[i] + (D_Base)k;
                Base prod_low = (Base)prod;
                k = (Base)(prod >> BASE_SIZE);
                if (u1.coef[j + i] >= prod_low) {
                        u1.coef[j + i] -= prod_low;
                } else {
                        u1.coef[j + i] = u1.coef[j + i] + ((D_Base)1 << BASE_SIZE) - prod_low;
                        borrow = true;
                }
            }
            if (u1.coef[j + v1.len] >= k) {
                u1.coef[j + v1.len] -= k;
            } else {
                borrow = true;
            }
            if (borrow) {
                Base carry = 0;
                for (int i = 0; i < v1.len; i++) {
                    D_Base sum = (D_Base)u1.coef[j + i] + (D_Base)v1.coef[i] + (D_Base)carry;
                    u1.coef[j + i] = (Base)sum;
                    carry = (Base)(sum >> BASE_SIZE);
                }
                u1.coef[j + v1.len] += carry;
            }
        }
        Big_Number r(v1.len);
        for(int i = 0; i < v1.len; i++){
            r.coef[i] = u1.coef[i];
        }
        r.len = v1.len;
        while (r.len > 1 && r.coef[r.len - 1] == 0) {
            r.len--;
        }
        if(d > 1){
            r /= (Base)d;
        }
        return r;
    }
    Big_Number& operator %=(const Big_Number &v);
    Big_Number& operator /=(const Big_Number &v);
    string cout_10() const{
        if(len == 1 && coef[0] == 0){
            return "0";
        }
        Big_Number tmp = *this;
        string res = "";
        while(!(tmp.len == 1 && tmp.coef[0] == 0)){
            Big_Number r = tmp % (Base)10;
            res = char('0' + r.coef[0]) + res;
            tmp = tmp / (Base)10;
        }
        return res.empty() ? "0" : res;
    }
    void cin_10(const string& str_10){
        if(str_10.empty()){
            len = 1;
            coef[0] = 0;
            return;
        }
        for(char c : str_10){
            if(c < '0' || c > '9'){
                throw invalid_argument("INCORRECT SYMBOL!!!\n");
            }
        }
        len = 1;
        fill(coef, coef + maxlen, 0);
        for(char c : str_10){
            int k = c - '0';
            *this = *this * (Base)10;
            Big_Number res(1);
            res.coef[0] = k;
            *this = *this + res;
        }
    }
    friend istream &operator >>(istream &in, Big_Number &bn) {
        string str_10;
        in >> str_10;
        try{
            bn.cin_10(str_10);
        }
        catch (const exception& i) {
            in.setstate(iostream::failbit);
            cout << "Incorrect number: " << i.what() << "\n";
        }
        return in;
    }
    friend ostream &operator <<(ostream &os, const Big_Number &bn) {
        os << bn.cout_10();
        return os;
    }
};
Big_Number& Big_Number::operator+=(const Big_Number& v) {
    *this = *this + v;
    return *this;
}
Big_Number& Big_Number::operator-=(const Big_Number& v) {
    *this = *this - v;
    return *this;
}
Big_Number& Big_Number::operator*=(const Base &a){
    *this = *this * a;
    return *this;
}
Big_Number& Big_Number::operator*=(const Big_Number &v){
    *this = *this * v;
    return *this;
}
Big_Number& Big_Number::operator/=(const Base &a){
    *this = *this / a;
    return *this;
}
Big_Number& Big_Number::operator%=(const Base &a){
    *this = *this % a;
    return *this;
}
Big_Number& Big_Number::operator/=(const Big_Number &v){
   *this = *this / v;
   return *this;
}
Big_Number& Big_Number::operator %=(const Big_Number &v){
   *this = *this % v;
   return *this;
}
/*void test(){
    int max_len = 1000;
    int N = 1000;
    Big_Number A, D, Q, R;
    do{
        int len_A = rand() % max_len + 1;
        int len_d = rand() % max_len + 1;
        Big_Number E(len_A, 1);
        Big_Number G(len_d, 1);
        A = E;
        D = G;
        Q = A / D;
        R = A % D;
    }while(A == Q * D + R && A - R == Q * D && R < D && --N >= 0);
}*/
int main() {
    srand(time(0));
    /*Big_Number Num1(5, 1);
    cout << "Big_Num1: " << Num1 << "\n";
    Big_Number Num2(12, 1);
    cout << "Big_Num2: " << Num2 << "\n";
    Big_Number Num3 = Num1 + (Base)6;
    cout << "Big_Num3: " << Num3 << "\n";
    Num3 -= Num2;
    cout << "Big_Num3: " << Num3 << "\n";*/

    Big_Number Num4(7, 1);
    cout << "Num 4: " << Num4.Big_Num_To_HEX() << "\n";
    Big_Number Num5(2, 1);
    cout << "Num 5: " << Num5.Big_Num_To_HEX() << "\n";
    Big_Number Num6 = Num4 * Num5;
    cout << "Num6 = " << Num6.Big_Num_To_HEX() << "\n";
    Num6 = Num6 / Num5;
    cout << "Num6 = " << Num6.Big_Num_To_HEX() << "\n";
    //test();

    return 0;
}
