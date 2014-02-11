#!/bin/bash

if [ "$1" == "" ] || [ "$2" == "" ]; then
    echo "Usage: $0 project-name projname

Creates new skeleton project in directory 'project-name' configured to have short name 'projname'.

For example: $0 competitive-market cm
" >&2
    exit 1
fi

if [ -e "$1" ]; then
    echo "Project creation aborted: $1 already exists!" >&2
    exit 2
fi

set -e

mkdir -p "$1/$2"
echo -e "cmake_minimum_required(VERSION 2.8)\nset(myproj $2)\ninclude(../cmake/project-base.cmake)" >"$1"/CMakeLists.txt
ln -s ../clang_complete "$1"/.clang_complete
