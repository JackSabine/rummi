#include <type_traits>

template <typename T> inline T div_and_round_up(T numerator, T denominator) {
    static_assert(std::is_integral<T>::value, "Not an integral type");
    return (numerator + (denominator - 1)) / denominator;
}
