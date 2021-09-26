#include "rsa.h"

#include "rsa.h"
#include <cstdlib>
#include <ctime>
#include <string>

using namespace std;

Rsa::Rsa()
{
}

Rsa::~Rsa()
{
}

int Rsa::endecrypt(const int msg, const int key, const int pkey)
{
    int msg_des = 1;
    int root = msg;
    int index = key;
    while (index)
    {
        if (index & 1)
            msg_des = (msg_des * root) % pkey;
        index >>= 1;
        root = (root * root) % pkey;
    }
    return msg_des;
}

/************************************
 * 生成钥匙
 * *********************************/
Key Rsa::produce_keys()
{
    int prime1 = produce_prime();
    int prime2 = produce_prime();
    while (prime2 == prime1)
        prime2 = produce_prime();

    Key key;
    int orla = produce_orla(prime1, prime2);
    key.pkey = produce_pkey(prime1, prime2);
    key.ekey = produce_ekey(orla);
    key.dkey = produce_dkey(key.ekey, orla);
    return key;
}

/************************************
 *	生成公钥
 * *********************************/
int Rsa::produce_pkey(const int prime1, const int prime2)
{
    return prime1 * prime2;
}

/************************************
 *	生成欧拉数
 * *********************************/
int Rsa::produce_orla(const int prime1, const int prime2)
{
    return (prime1 - 1) * (prime2 - 1);
}

/************************************
 *	生成加密密钥
 * *********************************/
int Rsa::produce_ekey(const int orla)
{
    int ekey;
    while (true)
    {
        ekey = rand() % orla;
        if (ekey >= 2 && produce_gcd(ekey, orla) == 1)
            break;
    }
    return ekey;
}

/************************************
 *	生成解密钥匙
 * *********************************/
int Rsa::produce_dkey(const int ekey, const int orla)
{
    int dkey = orla / ekey;
    while (true)
    {
        if (((dkey * ekey) % orla) == 1)
            break;
        else
            ++dkey;
    }
    return dkey;
}

/************************************
 *	产生随机素数，范围为[100, 200)
 * *********************************/
int Rsa::produce_prime()
{
    int prime = 0;
    srand(time(0));
    while (true)
    {
        prime = rand() % 100 + 100;
        if (is_prime(prime))
            break;
    }
    return prime;
}

/************************************
 * 产生两个数的最大公约数
 * *********************************/
int Rsa::produce_gcd(const int a, const int b)
{
    int dividend = a;
    int divisor = b;
    int residual = dividend % divisor;
    while (residual)
    {
        dividend = divisor;
        divisor = residual;
        residual = dividend % divisor;
    }
    return divisor;
}

/************************************
 * 判断是否为素数，效率较低，代改进
 * *********************************/
bool Rsa::is_prime(const int digit)
{
    int tmp = 2;
    while (tmp < digit)
        if (!(digit % tmp++))
            break;

    if (tmp == digit)
        return true;
    return false;
}
