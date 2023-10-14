#include <iostream>
#include <memory>
#include <functional>
#include <string>
#include <map>
#include <mutex>
#include <stdio.h>

class Stock
{
public:
    Stock(const std::string name) : _name(name)
    {
        printf("Stock[%p] %s\n", this, _name.c_str());
    }

    ~Stock()
    {
        printf("~Stock[%p] %s\n", this, _name.c_str());
    }

    Stock(const Stock &) = delete;
    Stock &operator=(const Stock &) = delete;

    const std::string &key() const { return _name; }

private:
    std::string _name;
};

class StockFactory : public std::enable_shared_from_this<StockFactory>
{
    friend class Stock;
public:
    std::shared_ptr<Stock> get(const std::string &key)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        std::shared_ptr<Stock> stock;
        std::weak_ptr<Stock> &stock_ref = _stocks[key];
        stock = stock_ref.lock();
        if (!stock)
        {
            stock.reset(new Stock(key), std::bind(&StockFactory::remove_stock, shared_from_this(), std::placeholders::_1));
            stock_ref = stock;
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
    void remove_stock(Stock *s)
    {
        if (s)
        {
            std::lock_guard<std::mutex> lock(_mutex);
            _stocks.erase(s->key());
        }
        delete s;
    }

private:
    std::mutex _mutex;
    std::map<std::string, std::weak_ptr<Stock>> _stocks;
};

int main()
{
    std::shared_ptr<StockFactory> stock_factory = std::make_shared<StockFactory>();
    std::cout << stock_factory->stock_count() << std::endl;
    {
        std::shared_ptr<Stock> msft = stock_factory->get("MSFT");
        std::shared_ptr<Stock> apple = stock_factory->get("APPLE");

        std::cout << msft->key() << std::endl;
        std::cout << apple->key() << std::endl;

        stock_factory->show_stocks();
        std::cout << stock_factory->stock_count() << std::endl;
    }
    stock_factory->show_stocks();
    std::cout << stock_factory->stock_count() << std::endl;

    return 0;
}
