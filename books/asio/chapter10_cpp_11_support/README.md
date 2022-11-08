#### movable I/O object
```cpp
class connection : public enable_shared_from_this<connection>
{
private:
  tcp::socket socket_;
  ...
public:
  connection(tcp::socket&& s) : socket_(std::move(s)) {}
  ...
};

...

class server
{
private:
  tcp::acceptor acceptor_;
  ...
  void handle_accept(error_code ec, tcp::socket socket)
  {
    if (!ec)
      std::make_shared<connection>(std::move(socket))->go();
    acceptor_.async_accept(...);
  }
  ...
};
```

