# Almost all CMake files should start with this
# You should always specify a range with the newest
# and oldest tested versions of CMake. This will ensure
# you pick up the best policies.
cmake_minimum_required(VERSION 3.15...3.16)

# This is your project statement. You should always list languages;
# Listing the version is nice here since it sets lots of useful variables
project(cpp_server_linux
        VERSION 1.0
        DESCRIPTION "A web application written in cpp"
        LANGUAGES CXX C)

# If you set any CMAKE_ variables, that can go here.
# (But usually don't do this, except maybe for C++ standard)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_CXX_STANDARD 17)

# Find packages go here.

# You should usually split this into folders, but this is a simple example

add_subdirectory(src)
add_subdirectory(Library)

# Link each target with other targets or add options, etc.

# Adding something we can run - Output name matches target name

# Make sure you link your targets with this command. It can also link libraries and
# even flags, so linking a target that does not exist will not give a configure-time error.
#target_link_libraries(MyExample PRIVATE MyLibExample)