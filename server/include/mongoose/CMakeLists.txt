cmake_minimum_required (VERSION 2.8)

project ("Mongoose lib")

add_definitions(-DMG_ENABLE_THREADS)
add_library(mongoose STATIC mongoose.c)
