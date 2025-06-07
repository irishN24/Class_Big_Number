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
    Big_Number(const Big_Number& B_v) : len(B_v.len), maxlen(B_v.maxlen) {
        coef = new Base[maxlen];
        for (int i = 0; i < maxlen; ++i) {
            coef[i] = B_v.coef[i];
        }
    }
    //деструктор
    ~Big_Number() {
        delete[] coef;
    }
    Big_Number& operator=(const Big_Number& B_v) {
        if (this != &B_v) {
            delete[] coef;
            len = B_v.len;
            maxlen = B_v.maxlen;
            coef = new Base[maxlen];
            if (coef) {
                for (int i = 0; i < maxlen; ++i) {
                    coef[i] = B_v.coef[i];
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
        else {
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
    Big_Number operator +(const Base& v) const {
        int l = len + 1;
        Big_Number res(l);
        D_Base tmp = (D_Base)coef[0] + (D_Base)v;
        res.coef[0] = (Base)tmp;
        Base k = tmp >> BASE_SIZE;
        int j = 1;
        for (; j < len; j++) {
            tmp = (D_Base)coef[j] + (D_Base)k;
            res.coef[j] = (Base)tmp;
            k = (Base)(tmp >> BASE_SIZE);
        }
        res.coef[j] = k;
        while (res.len > 1 && res.coef[res.len - 1] == 0) {
            res.len--;
        }
        return res;
    }
    Big_Number& operator+=(const Base& v);
    Big_Number& operator+=(const Big_Number& v);
    Big_Number operator -(const Big_Number& v) const {
        if (*this >= v) {
            Big_Number result(len);
            D_Base tmp;
            int j = 0;      // Индекс по коэффициентам
            D_Base k = 0;   // займ

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
    Big_Number& operator *=(const Base& a);
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
    Big_Number& operator *=(const Big_Number& v);
    Big_Number operator /(const Base& a) const {
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
    Big_Number& operator /=(const Base& a);
    Big_Number operator %(const Base& a) const {
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
    Big_Number& operator %=(const Base& a);
    Big_Number operator /(const Big_Number& v) const {
        if (v.len == 1 && v.coef[0] == 0)
        {
            throw invalid_argument("Invalid arguments.");
        }
        if (*this < v)
        {
            return Big_Number(1);;
        }
        if (*this == v) {
            Big_Number res(1);
            res.coef[0] = 1;
            return res;
        }
        if (v.len == 1)
        {
            return *this / v.coef[0];
        }
        int m = len - v.len;
        D_Base b = ((D_Base)1 << BASE_SIZE);
        D_Base d = b / (D_Base)(v.coef[v.len - 1] + (Base)1);
        //cout << "d: " << d << "\n";
        Big_Number newv = *this;
        newv *= d;
        Big_Number delv = v;
        delv *= d;
        if (newv.len == len)
        {
            Base* new_coef = new Base[newv.len + 1];
            copy(newv.coef, newv.coef + newv.len, new_coef);
            new_coef[newv.len] = 0;
            delete[] newv.coef;
            newv.coef = new_coef;
            newv.len++;
            newv.maxlen = newv.len;
        }
        //cout << "newv: " << newv.Big_Num_To_HEX() << "\n";
        Big_Number q(m + 1);
        for (int j = m; j >= 0; j--)
        {
            cout << j << " ";
            D_Base qhat = (D_Base)((D_Base)((D_Base)((D_Base)(newv.coef[j + delv.len]) * (D_Base)(b)) + (D_Base)(newv.coef[j + delv.len - 1])) / (delv.coef[v.len - 1]));

            cout << (D_Base)((D_Base)((D_Base)(newv.coef[j + delv.len]) * (D_Base)(b)) + (D_Base)(newv.coef[j + delv.len - 1])) << "\n";
            cout << (D_Base)(delv.coef[v.len - 1]) << "\n";
            cout << "qhat: " << hex << qhat << "\n";
            D_Base r = (D_Base)((D_Base)((D_Base)((D_Base)(newv.coef[j + delv.len]) * (D_Base)(b)) + (D_Base)(newv.coef[j + delv.len - 1])) % (D_Base)(delv.coef[delv.len - 1]));
            while (qhat == b || (delv.len > 1 && qhat * delv.coef[delv.len - 2] > (r << BASE_SIZE) + newv.coef[j + delv.len - 2]))
            {
                qhat--;
                r += delv.coef[delv.len - 1];
                q.coef[j] = (Base)qhat;
                q.len = m + 1;
                if (r >= b)
                {
                 //   j--;
                    break;
                }
            }
            Big_Number u(v.len + 1);
            for (int i = 0; i <= v.len; i++)
            {
                u.coef[i] = newv.coef[j + i];
            }
            u.len = v.len + 1;
            cout << "u: " << u.Big_Num_To_HEX() << "\n";
            Big_Number p = delv * (Base)(qhat);
            if (u < p)
            {
                qhat--;
                p = delv * qhat;
            }
            Big_Number tmp = u - p;

            for (int i = 0; i <= delv.len; i++)
            {
                if (i < tmp.len) {
                    newv.coef[j + i] = tmp.coef[i];
                }
                else {
                    newv.coef[j + i] = 0;
                }
            }
            q.coef[j] = (Base)qhat;
            q.len = m + 1;
            cout << "q: " << q.Big_Num_To_HEX() << "\n";
            cout << "newv: " << newv.Big_Num_To_HEX() << "\n";
        }
        while (q.len > 1 && q.coef[q.len - 1] == 0)
        {
            q.len--;
        }
        return q;
    }
    Big_Number operator %(const Big_Number& v) const {
        if (v.len == 1 && v.coef[0] == 0) {
            throw invalid_argument("Division by zero!!!");
        }
        if (*this < v) {
            return *this;
        }
        if (*this == v) {
            return Big_Number(0);
        }
        Big_Number q = *this / v;  
        Big_Number r = *this - (q * v);
        return r;
    }
    Big_Number& operator %=(const Big_Number& v);
    Big_Number& operator /=(const Big_Number& v);
    string cout_10() const {
        if (len == 1 && coef[0] == 0) {
            return "0";
        }
        Big_Number tmp = *this;
        string res = "";
        while (!(tmp.len == 1 && tmp.coef[0] == 0)) {
            Big_Number r = tmp % (Base)10;
            res = char('0' + r.coef[0]) + res;
            tmp = tmp / (Base)10;
        }
        return res.empty() ? "0" : res;
    }
    void cin_10(const string& str_10) {
        if (str_10.empty()) {
            len = 1;
            coef[0] = 0;
            return;
        }
        for (char c : str_10) {
            if (c < '0' || c > '9') {
                throw invalid_argument("INCORRECT SYMBOL!!!\n");
            }
        }
        len = 1;
        fill(coef, coef + maxlen, 0);
        for (char c : str_10) {
            int k = c - '0';
            *this = *this * (Base)10;
            Big_Number res(1);
            res.coef[0] = k;
            *this = *this + k;
        }
    }
    friend istream& operator >>(istream& in, Big_Number& bn) {
        string str_10;
        in >> str_10;
        try {
            bn.cin_10(str_10);
        }
        catch (const exception& i) {
            in.setstate(iostream::failbit);
            cout << "Incorrect vber: " << i.what() << "\n";
        }
        return in;
    }
    friend ostream& operator <<(ostream& os, const Big_Number& bn) {
        os << bn.cout_10();
        return os;
    }
    friend void test();
};
Big_Number& Big_Number::operator+=(const Big_Number& v) {
    *this = *this + v;
    return *this;
}
Big_Number& Big_Number::operator+=(const Base& v) {
    *this = *this + v;
    return *this;
}
Big_Number& Big_Number::operator-=(const Big_Number& v) {
    *this = *this - v;
    return *this;
}
Big_Number& Big_Number::operator*=(const Base& a) {
    *this = *this * a;
    return *this;
}
Big_Number& Big_Number::operator*=(const Big_Number& v) {
    *this = *this * v;
    return *this;
}
Big_Number& Big_Number::operator/=(const Base& a) {
    *this = *this / a;
    return *this;
}
Big_Number& Big_Number::operator%=(const Base& a) {
    *this = *this % a;
    return *this;
}
Big_Number& Big_Number::operator/=(const Big_Number& v) {
    *this = *this / v;
    return *this;
}
Big_Number& Big_Number::operator %=(const Big_Number& v) {
    *this = *this % v;
    return *this;
}
void test() {
        int max_len = 15;
        int N = 1000;
        Big_Number A, D, Q, R;
        int count = 0;
        do {
            int len_A = rand() % max_len + 1;
            int len_d = rand() % max_len + 1;
            Big_Number E(len_A, 1);
            Big_Number G(len_d, 1);
            while (G.len == 1 && G.coef[0] == 0) {
                G = Big_Number(len_d, 1);
            }
            A = E;
            D = G;
            try {
                Q = A / D;
                R = A % D;
            }
            catch (const invalid_argument& e) {
                cout << "Error: " << e.what() << endl;
                cout << "A = " << A.Big_Num_To_HEX() << endl;
                cout << "D = " << D.Big_Num_To_HEX() << endl;
                break;
            }
            count++;
        } while (A == Q * D + R && A - R == Q * D && R < D && --N >= 0);
        cout << count << "\n";
    }
int main() {
    srand(time(0)); string str1 = "4b3cf23c311de6eb0cc61d90";
    Big_Number v4;
    v4.HEX_TO_BN(str1);
    cout << "Num4: " << v4.Big_Num_To_HEX() << "\n";
    string str2 = "da303600c9";
    Big_Number v5;
    v5.HEX_TO_BN(str2);
    cout << "Num5: " << v5.Big_Num_To_HEX() << "\n";
    Big_Number v6 = v4 / v5;
    cout << "Num6 = " << v6.Big_Num_To_HEX() << "\n";
    Big_Number v8 = v4 % v5;
    cout << "Num8 = " << v8.Big_Num_To_HEX() << "\n";
    return 0;
} попробуй понять почему v6 считается неправильно
