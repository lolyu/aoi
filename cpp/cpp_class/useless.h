#ifndef _USELESS_H_
#define _USELESS_H_

class Useless
{
public:
    Useless();

    explicit Useless(int k);

    Useless(int k, char ch);

    Useless(const Useless &f);

    Useless(Useless &&f);

    ~Useless();

    Useless operator+(const Useless &f) const;

    Useless &operator=(const Useless &f);

    Useless &operator=(Useless &&f);

    void showData() const;

private:
    int n;

    char *pc;

    static int ct;

    void showObject() const;
};

#endif
