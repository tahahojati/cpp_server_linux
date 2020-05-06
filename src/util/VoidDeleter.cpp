#include "VoidDeleter.hpp"
namespace framework {
    namespace util {
        namespace meta {
            template <typename  T>
            void void_deleter(void const *ptr) {
                T const *ptr2 = static_cast<T*> (ptr);
                delete ptr2;
            }
        }
    }
}