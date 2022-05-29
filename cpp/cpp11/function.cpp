#include <functional>
#include <iostream>

template <typename T, typename F>
T use_f(T v, F f)
{
    static int count = 0;
    ++count;
    std::cout << "use_f count = " << count << ", &count = " << &count << std::endl;
    return f(v);
}

class Fp
{
private:
    double d;

public:
    Fp(double d = 1.0)
        : d(d) {}

    double operator()(double p) { return d * p; }
};

class Fq
{
private:
    double d;

public:
    Fq(double d = 1.0)
        : d(d) {}

    double operator()(double p) { return d + p; }
};

double dub(double x)
{
    return 2.0 * x;
}

double square(double x)
{
    return x * x;
}

int main()
{
    double x = 10.0;
    use_f(x, Fp());
    use_f(x, Fq());
    use_f(x, dub);
    use_f(x, square);

    // using function object, template function could only instantiate only once for the function object type once
    // and all callable objects(lambda, function, functor) that share same signature could use.
    std::function<double(double)> fobj;
    use_f(x, fobj = Fp());
    use_f(x, fobj = Fq());
    use_f(x, fobj = dub);
    use_f(x, fobj = square);
    return 0;
}
