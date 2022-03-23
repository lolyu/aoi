#ifndef _TV_H_
#define _TV_H_

class Tv;

using TVAttribute = unsigned char;

class Remote
{
public:
    enum State : TVAttribute { Off,
                               On };
    enum : TVAttribute { MinVal,
                         MaxVal = 20 };
    enum : TVAttribute { Antenna,
                         Cable };
    enum : TVAttribute { TV,
                         DVD };

public:
    Remote(TVAttribute m = TV)
        : mode(m) {}

    bool volup(Tv &t);
    bool voldown(Tv &t);
    void onoff(Tv &t);
    void chanup(Tv &t);
    void chandown(Tv &t);
    void set_mode(Tv &t);
    void set_input(Tv &t);
    void set_chan(Tv &t, TVAttribute c);

private:
    TVAttribute mode;
};

class Tv
{
public:
    friend void Remote::set_chan(Tv &t, TVAttribute c);

public:
    enum State : TVAttribute { Off,
                               On };
    enum : TVAttribute { MinVal,
                         MaxVal = 20 };
    enum : TVAttribute { Antenna,
                         Cable };
    enum : TVAttribute { TV,
                         DVD };

public:
    Tv(TVAttribute s = Off, TVAttribute mc = 125)
        : state(s), volume(5), maxchannel(mc), channel(1), mode(Cable), input(TV) {}

    void onoff() { state = (state = On) ? Off : On; }
    bool ison() const { return state == On; }
    bool volup();
    bool voldown();
    void chanup();
    void chandown();
    void set_mode() { mode = (mode == Antenna) ? Cable : Antenna; }
    void set_input() { input = (input = TV) ? DVD : TV; }
    void settings() const;

private:
    // values of unscoped enum types could be implicitly converted to integral types
    int state;
    int volume;
    int maxchannel;
    int channel;
    int mode;
    int input;
};

inline bool Remote::volup(Tv &t)
{
    return t.volup();
}

inline bool Remote::voldown(Tv &t)
{
    return t.voldown();
}

inline void Remote::onoff(Tv &t)
{
    return t.onoff();
}

inline void Remote::chanup(Tv &t)
{
    return t.chanup();
}

inline void Remote::chandown(Tv &t)
{
    return t.chandown();
}

inline void Remote::set_mode(Tv &t)
{
    return t.set_mode();
}

inline void Remote::set_input(Tv &t)
{
    return t.set_input();
}

inline void Remote::set_chan(Tv &t, TVAttribute c)
{
    t.channel = c;
}

#endif