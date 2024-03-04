#include <iostream>
#include <memory>
#include <mutex>
#include <map>
#include <functional>
#include <string>
#include <cstdio>
#include <cstdlib>

using namespace std;

class Stock
{
public:
    Stock(const string name) : _name(name)
    {
        printf("Stock[%p] %s\n", this, _name.c_str());
    }

    ~Stock()
    {
        printf("~Stock[%p] %s\n", this, _name.c_str());
    }

    Stock(const Stock &) = delete;
    Stock &operator=(const Stock &) = delete;

    const string &key() const { return _name; }

private:
    string _name;
};

class StockFactory : public enable_shared_from_this<StockFactory>
{
    friend class Stock;

public:
    StockFactory()
    {
        printf("StockFactory[%p]\n", this);
    }
    StockFactory(const StockFactory &sf) = delete;
    StockFactory &operator=(const StockFactory &sf) = delete;
    ~StockFactory()
    {
        printf("~StockFactory[%p]\n", this);
    }

    shared_ptr<Stock> get(const string &key)
    {
        lock_guard<mutex> lock(_mutex);
        weak_ptr<Stock> &stock_ref = _stocks[key];
        shared_ptr<Stock> stock = stock_ref.lock();
        if (!stock)
        {
            stock.reset(new Stock(key), bind(&StockFactory::remove_stock_weak, weak_from_this(), placeholders::_1));
        }
        return stock;
    }

    size_t stock_count() const
    {
        return _stocks.size();
    }

    void show_stocks() const
    {
        for (auto it = _stocks.cbegin(); it != _stocks.cend(); ++it)
        {
            if (it->second.lock())
            {
                std::cout << "Stock: " << it->first << std::endl;
            }
        }
    }

private:
    void remove_stock(const Stock *sp)
    {
        if (sp)
        {
            lock_guard<mutex> lock(_mutex);
            _stocks.erase(sp->key());
        }
        delete sp;
    }

    static void remove_stock_weak(const weak_ptr<StockFactory> &wsfp, const Stock *sp)
    {
        shared_ptr<StockFactory> sfp = wsfp.lock();
        if (sfp)
        {
            sfp->remove_stock(sp);
        }
    }

    mutex _mutex;
    map<string, weak_ptr<Stock>> _stocks;
};

int main()
{
    shared_ptr<Stock> stock;
    {
        shared_ptr<StockFactory> stock_factory = make_shared<StockFactory>();
        stock = stock_factory->get("MSFT");
        shared_ptr<Stock> msft_stock = stock_factory->get("MSFT");
        // stock_factory->show_stocks();
        cout << stock.use_count() << endl;
        cout << msft_stock.use_count() << endl;
    }
    auto stock2 = stock;
    cout << stock.use_count() << endl;
    return 0;
}
