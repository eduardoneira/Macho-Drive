cmake_minimum_required (VERSION 2.8)

project ("Mongoose lib")

add_definitions(-DNS_ENABLE_THREADS)
add_library(mongoose STATIC mongoose.c)
