#include <type_traits>

// ==================================================================

template<int... I>
struct Vector {};

template<typename>
struct remove_head;

template<int H, int... T>
struct remove_head<Vector<H, T...>>
{
    static constexpr int val = H;
    using type = Vector<T...>;
};

template<int, typename>
struct prepend;

template<int H, int... T>
struct prepend<H, Vector<T...>>
{
    using type = Vector<H, T...>;
};

// ==================================================================


template<typename, typename>
struct merge;

template<int... I1, int... I2>
struct merge<Vector<I1...>, Vector<I2...>>
{
    static constexpr int mul_val = remove_head<Vector<I1...>>::val * remove_head<Vector<I2...>>::val;
    using type_1 = typename remove_head<Vector<I1...>>::type;
    using type_2 = typename remove_head<Vector<I2...>>::type;

    using type = typename prepend<mul_val, typename merge<type_1, type_2>::type>::type;
};

template<int I1, int I2>
struct merge<Vector<I1>, Vector<I2>>
{
    using type = Vector<I1 * I2>;
};
// ==================================================================

template<typename...>
struct zip;

template<typename V1, typename... Vs>
struct zip<V1, Vs...>
{
    using type = typename merge<V1, typename zip<Vs...>::type>::type;
};

template<int... I>
struct zip<Vector<I...>>
{
    using type = Vector<I...>;
};

// ==================================================================

int main()
{
    static_assert(std::is_same_v<zip<Vector<1, 2, 3>, Vector<2, 3, 4>, Vector<3, 4, 5>>::type, Vector<6, 24, 60>>);
}
