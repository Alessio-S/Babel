cmake_minimum_required(VERSION 3.20)

# Project name
project(Babel-client)

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "-Wall -Werror -O3 -g")
endif()

# Find SFML & Boost components (graphics, audio, thread, regex, etc.)
# find_package(SFML 2.6 COMPONENTS graphics audio system windows REQUIRED)
# find_package(Boost REQUIRED COMPONENTS system date_time regex serialization filesystem program_options thread)

# Ajouter tous les fichiers source dans le dossier
file(GLOB CLIENT_SOURCES "*.cpp")

# Ajouter tous les fichiers header dans le dossier
# include_directories(${SFML_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})

# Ajouter l'exécutable avec tous les fichiers source
add_executable(babel-client ${CLIENT_SOURCES})

# Link SFML libraries to your executable
target_link_libraries(babel-client portaudio)
# target_link_libraries(r-type_client ${Boost_LIBRARIES})