#ifndef RSA_H
#define RSA_H


#ifndef _RSA
#define _RSA

typedef struct key
{
    int pkey;
    int ekey;
    int dkey;
} Key;

class Rsa
{
public:
    Rsa();
    virtual ~Rsa();

public:
    Key produce_keys();
    int endecrypt(const int msg, const int key, const int pkey);

private:
    int produce_pkey(const int prime1, const int prime2);
    int produce_ekey(const int orla);
    int produce_dkey(const int ekey, const int orla);
    int produce_prime();
    int produce_orla(const int prime1, const int prime2);
    int produce_gcd(const int a, const int b);
    bool is_prime(const int digit);
};

#endif
#endif // RSA_H
