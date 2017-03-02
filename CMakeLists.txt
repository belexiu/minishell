cmake_minimum_required(VERSION 3.6)
project(minishell)

set(CMAKE_C_STANDARD 90)

set(SOURCE_FILES main.c ft_strsplit.c ft_strsplit.h ft_gnl.c ft_gnl.h ft_gnl_help.c built_in.c minishell.h)
add_executable(minishell ${SOURCE_FILES})