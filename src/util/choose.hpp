#ifndef CPP_SERVER_LINUX_CHOOSE_HPP
#define CPP_SERVER_LINUX_CHOOSE_HPP
namespace framework{
    namespace util {
        namespace meta {
            template <size_t i, class first, class... rest>
            struct choose: public choose<i - 1, rest...> {};

            template <class first, class... rest>
            struct choose<0, first, rest...> {
                using type = first;
            };
        }
    }
}
#endif //CPP_SERVER_LINUX_CHOOSE_HPP
