#!/bin/bash

if [ "$1" = "" ]; then
    premake5 gmake && make TowOff
fi

if [ "$1" = "debug" ]; then
    premake5 gmake && make config=debug TowOff
fi

if [ "$1" = "release" ]; then
    premake5 gmake && make config=release TowOff
fi
