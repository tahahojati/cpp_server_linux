#ifndef CPP_SERVER_LINUX_ROUTER_HPP
#define CPP_SERVER_LINUX_ROUTER_HPP
#include <r3/r3.hpp>
#include <unique_ptr>
#include <string>
#include <unordered_set>
// create a router tree with 10 children capacity (this capacity can grow dynamically)
R3Node *n = r3_tree_create(10);

int route_data = 3;

// insert the R3Route path into the router tree
r3_tree_insert_path(n, "/bar", &route_data); // ignore the length of path

r3_tree_insert_pathl(n, "/zoo", strlen("/zoo"), &route_data );
r3_tree_insert_pathl(n, "/foo/bar", strlen("/foo/bar"), &route_data );

r3_tree_insert_pathl(n ,"/post/{id}", strlen("/post/{id}") , &route_data );

r3_tree_insert_pathl(n, "/user/{id:\\d+}", strlen("/user/{id:\\d+}"), &route_data );


// if you want to catch error, you may call the extended path function for insertion
int data = 10;
char *errstr = NULL;
R3Node *ret = r3_tree_insert_pathl_ex(n, "/foo/{name:\\d{5}", strlen("/foo/{name:\\d{5}"), NULL, &data, &errstr);
if (ret == NULL) {
// failed insertion
printf("error: %s\n", errstr);
free(errstr); // errstr is created from `asprintf`, so you have to free it manually.
}


// let's compile the tree!
char *errstr = NULL;
int err = r3_tree_compile(n, &errstr);
if (err != 0) {
// fail
printf("error: %s\n", errstr);
free(errstr); // errstr is created from `asprintf`, so you have to free it manually.
}


// dump the compiled tree
r3_tree_dump(n, 0);

// match a route
R3Node *matched_node = r3_tree_matchl(n, "/foo/bar", strlen("/foo/bar"), NULL);
if (matched_node) {
int ret = *( (int*) matched_node->data );
}

// release the tree
r3_tree_free(n);

namespace framework {
    namespace application {
        class Router {
        public:
            struct match {
                bool match = false;
                std::unordered_set<std::string, std::string> params{};
            }
            constexpr Router();
            constexpr Router(const std::initializer_list<std::pair<std::string, int>> routes);
            Router(const Router&) = delete;
            Router(Router&&) = delete;
            Router&operator=(const Router&) = delete;
            Router&operator=(Router&&) = delete;
            constexpr ~Router();

            void addRoute(string route, int code);
            /* TODO: brilliant idea: cpp templating and constexpr and macros let us do compile time stuff that leads
             * to code that is alot faster because: branching and function call and perhaps even a lot of computation
            // has been avoided (e.g. imagine calculating m! in compile time!).  They have the ability that a
            // constexpr function will be replaced by its result if its argument is also constexpr.
            // So I think we need to write that for js or python. They have eval functions that let you execute code
            // on fly. We just need to track all symbols that are known in compile time and the effect they have on
            // each other. Then we use eval to calculate all constexpr values and replace them. We can even inline
            // functions.
             */

            void insert_path(const std::string path, void* data); //TODO
            int compile(char** errstr = NULL); //TODO

            match matches(std::string route){

            }


            void dump(int level) const {
                r3_tree_dump(get(), level);
            }


            Route insert_route(int method, const char* path, void* data,
                               char** errstr = NULL) {
                return r3_tree_insert_routel_ex(get(), method, path,
                                                std::strlen(path), data, errstr);
            }

            Route insert_routel(int method, const char* path, int path_len,
                                void* data, char** errstr = NULL) {
                return r3_tree_insert_routel_ex(get(), method, path, path_len, data,
                                                errstr);
            }

            Node match(const char* path, MatchEntry* entry = NULL) const {
                return r3_tree_match(get(), path,
                                     entry != NULL ? entry->get() : NULL);
            }

            Node matchl(const char* path, int path_len, MatchEntry* entry = NULL)
            const {
                return r3_tree_matchl(get(), path, path_len,
                                      entry != NULL ? entry->get() : NULL);
            }

            Node match_entry(MatchEntry& entry) const {
                return r3_tree_match_entry(get(), entry.get());
            }

            Route match_route(MatchEntry& entry) const {
                return r3_tree_match_route(get(), entry.get());
            }

        private:
            std::unique_ptr<r3::Tree> _tree
        };
    }
}

#endif //CPP_SERVER_LINUX_ROUTER_HPP
