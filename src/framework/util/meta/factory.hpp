#ifndef CPP_SERVER_LINUX_FACTORY_HPP
#define CPP_SERVER_LINUX_FACTORY_HPP

#include "choose.hpp"
namespace framework{
    namespace util{
        namespace meta{
            template<size_t i, class C>
            struct _Leaf {
                using type = C;
            };
            /*
             * The function of _type_factory class is that it inherits from all correct leaf types.
             */
            template <size_t i, class first, class... rest>
            struct _type_factory: public _Leaf<i, first>, _type_factory<i+1, rest...> {};

            template <size_t i, class first>
            struct _type_factory<i, first>: public _Leaf<i, first> {};



            template<class... cls>
            class factory {
            public:
                template <size_t j, typename... ArgTypes>
                inline typename choose<j, cls...>::type* create(ArgTypes... args) {
                    using f = typename choose<j, cls...>::type;
                    return new f(args...);
                }
            };
        }
    }
}
#endif //CPP_SERVER_LINUX_FACTORY_HPP
