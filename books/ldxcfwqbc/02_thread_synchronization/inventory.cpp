#include <mutex>
#include <cassert>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <set>
#include <memory>
#include <stdio.h>

class Request;

typedef std::set<std::weak_ptr<Request>, std::owner_less<std::weak_ptr<Request>>> RequestWeakSet;

class Inventory
{
public:
  Inventory() : requests_(std::make_shared<RequestWeakSet>()) {}

  void add(const std::weak_ptr<Request> &req)
  {
    std::lock_guard<std::mutex> lock(mutex_);
    if (!requests_.unique())
    {
      requests_.reset(new RequestWeakSet(*requests_));
    }
    requests_->insert(req);
  }

  void remove(const std::weak_ptr<Request> &req) __attribute__((noinline))
  {
    std::lock_guard<std::mutex> lock(mutex_);
    if (!requests_.unique())
    {
      requests_.reset(new RequestWeakSet(*requests_));
    }
    requests_->erase(req);
  }

  void printAll() const;

private:
  mutable std::mutex mutex_;
  std::shared_ptr<RequestWeakSet> requests_;
};

Inventory g_inventory;

class Request : std::enable_shared_from_this<Request>
{
public:
  void process() // __attribute__ ((noinline))
  {
    std::lock_guard<std::mutex> lock(mutex_);
    g_inventory.add(weak_from_this());
    // ...
  }

  ~Request() __attribute__((noinline))
  {
    std::lock_guard<std::mutex> lock(mutex_);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    g_inventory.remove(weak_from_this());
  }

  void print() const __attribute__((noinline))
  {
    std::lock_guard<std::mutex> lock(mutex_);
    // ...
  }

private:
  mutable std::mutex mutex_;
};

void Inventory::printAll() const
{
  std::shared_ptr<RequestWeakSet> requests_view;
  {
    std::lock_guard<std::mutex> lock(mutex_);
    requests_view = requests_;
  }

  std::this_thread::sleep_for(std::chrono::seconds(1));
  for (auto it = requests_view->cbegin(); it != requests_view->cend(); ++it)
  {
    auto request = it->lock();
    if (request)
    {
      request->print();
    }
  }
  printf("Inventory::printAll() unlocked\n");
}

void threadFunc()
{
  Request *req = new Request;
  req->process();
  delete req;
}

int main()
{
  std::thread(threadFunc).detach();
  std::this_thread::sleep_for(std::chrono::microseconds(500));
  g_inventory.printAll();

  return 0;
}
