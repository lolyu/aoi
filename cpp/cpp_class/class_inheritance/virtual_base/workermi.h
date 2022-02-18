#ifndef _WORKERMI_H_
#define _WORKERMI_H_

#include <string>

// virtual functions: provides dynamic binding
// dynamic binding: if a virtual function is called via a reference or pointer to a class object, the method is
// selected based on the object type other than the reference type or pointer type
// static binding: for non-virtual function, the function is selected based on the reference or pointer type
//
// pure virtual function: pure virtual function can have no function definition, but the derived class must have
// the function definition
//
// abstract class: class that has at least one pure virtual function
// abstract class cannot be instantiated
// those pure virtual functions inside the abstract class must be defined in the derived classes

class Worker
{

public:
    Worker() : fullname("NONE"), id(0L) {}
    Worker(const std::string &name, long id) : fullname(name), id(id) {}
    virtual ~Worker() = 0;
    virtual void Set() = 0;
    virtual void Show() const = 0;

protected:
    virtual void Data() const;
    virtual void Get();

private:
    std::string fullname;
    long id;
};

class Waiter : virtual public Worker
{
public:
    Waiter() : Worker(), panache(0) {}
    Waiter(const std::string &name, long id, int p) : Worker(name, id), panache(p) {}
    Waiter(const Worker &wk, int p = 0) : Worker(wk), panache(p) {}
    virtual void Set();
    virtual void Show() const;

protected:
    virtual void Data() const;
    virtual void Get();

private:
    int panache;
};

class Singer : virtual public Worker
{
public:
    using VoiceType = uint8_t;

protected:
    enum : VoiceType
    {
        other,
        alto,
        contralto,
        soprano,
        bass,
        baritone,
        tenor,
        vtypes
    };

public:
    Singer() : Worker(), voice(other) {}
    Singer(const std::string &name, long id, VoiceType v = other) : Worker(name, id), voice(v) {}
    Singer(const Worker &wk, VoiceType v = other) : Worker(wk), voice(v) {}
    virtual void Set();
    virtual void Show() const;

protected:
    virtual void Data() const;
    virtual void Get();

private:
    VoiceType voice;
    static const char *pv[vtypes];
};

class SingingWaiter : public Singer, public Waiter
{
public:
    SingingWaiter() {}
    SingingWaiter(const std::string &name, long id, int p, VoiceType v = other) : Worker(name, id), Singer(name, id, v), Waiter(name, id, p) {}
    SingingWaiter(const Worker &wk, int p = 0) : Worker(wk), Singer(wk), Waiter(wk, p) {}
    SingingWaiter(const Worker &wk, VoiceType v = other) : Worker(wk), Singer(wk, v), Waiter(wk) {}

    virtual void Set();
    virtual void Show() const;

protected:
    virtual void Data() const;
    virtual void Get();

private:
};

#endif
