#ifndef CPP_SERVER_LINUX_VOIDDELETER_HPP
#define CPP_SERVER_LINUX_VOIDDELETER_HPP


namespace framework {
    namespace util {
        namespace meta {
            template <typename  T>
            void void_deleter(const * void ptr);
        }
    }
}


#endif //CPP_SERVER_LINUX_VOIDDELETER_HPP
