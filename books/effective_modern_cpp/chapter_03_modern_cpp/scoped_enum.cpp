#include <iostream>
#include <string>
#include <tuple>

using UserInfo = std::tuple<std::string, std::string, std::size_t>;
enum UserInfoFieldsUnscoped : uint32_t {uiuName, uiuEmail, uiuReputation};
enum UserInfoFieldScoped :uint64_t {uisName, uisEmail, uisReputation};

void print0(const UserInfo &uinfo)
{
    std::cout << std::endl;
    std::cout << "Name: " << std::get<uiuName>(uinfo) << std::endl;
    std::cout << "Email: " << std::get<uiuEmail>(uinfo) << std::endl;
    std::cout << "Reputation: " << std::get<uiuReputation>(uinfo) << std::endl;
    std::cout << std::endl;
}

template <typename E>
constexpr auto toUnderlyingType(E enumerator) noexcept
{
    return static_cast<std::underlying_type_t<E>>(enumerator);
}

void print1(const UserInfo &uinfo)
{
    std::cout << std::endl;
    std::cout << "Name: " << std::get<toUnderlyingType(uisName)>(uinfo) << std::endl;
    std::cout << "Email: " << std::get<toUnderlyingType(uisEmail)>(uinfo) << std::endl;
    std::cout << "Reputation: " << std::get<toUnderlyingType(uisReputation)>(uinfo) << std::endl;
    std::cout << std::endl;
}

int main()
{
    UserInfo uinfo{"Alice", "aliceg@gmail.com", 128};
    print0(uinfo);
    print1(uinfo);
    return 0;
}
