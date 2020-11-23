cmake_minimum_required(VERSION 3.10)
set (CMAKE_CXX_STANDARD 11)
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")

project(voice_to_instrument)

include_directories(gaborator)

find_package(Boost COMPONENTS system filesystem thread REQUIRED)

add_executable(voice_to_instrument main.cpp kiss_fftr.cpp kissfft.cpp FreqCalculator.cpp GetNoteData.cpp FreqToMIDI.cpp WavReader.cpp)
target_link_libraries( voice_to_instrument ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} Boost::thread -lpthread -lsndfile)
