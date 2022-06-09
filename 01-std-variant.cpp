#include <type_traits>
#include <cstdint>
#include <cassert>
#include <iostream>

// =======================================================================
template<typename T, int32_t idx, typename Head, typename... Tail>
struct get_index
{
    static constexpr int32_t value = std::is_same_v<T, Head> ? idx : get_index<T, idx + 1, Tail...>::value;
};

template<typename T, int32_t idx, typename Head>
struct get_index<T, idx, Head>
{
    static constexpr int32_t value = std::is_same_v<T, Head> ? idx : -1;
};

// =======================================================================
template<typename... Ts>
struct max_size;

template<typename Head, typename... Tail>
struct max_size<Head, Tail...>
{
    static constexpr size_t value = sizeof(Head) > max_size<Tail...>::value ? sizeof(Head) : max_size<Tail...>::value;
};

template<typename Head>
struct max_size<Head>
{
    static constexpr size_t value = sizeof(Head);
};

// =======================================================================

struct bad_variant_access: public std::exception {};

// =======================================================================
template<typename... Ts>
class variant
{
public:
    template<typename T>
    variant(T&& t)
    {
        idx = get_index<T, 0, Ts...>::value;
        if (idx != -1)
        {
            new (m_buffer) T(std::forward<T>(t));
        }
        else
        {
            // raise exception
            return;
        }


    }

    template<typename T>
    T& get()
    {
        int32_t in_idx = get_index<T, 0, Ts...>::value;
        if (in_idx == idx)
        {
            return *reinterpret_cast<T*>(m_buffer);
        }
        else
        {
            // raise exception
            throw bad_variant_access();
        }
    }

private:
    char m_buffer[max_size<Ts...>::value];
    int32_t idx;
};


template<typename T, typename... Ts>
T& get(variant<Ts...>& v)
{
    return v.template get<T>();
}

int main()
{
    variant<int, double, char> v = 2;
    assert(get<int>(v) == 2);
    return 0;
}
