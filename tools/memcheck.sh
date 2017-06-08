#!/bin/bash

valgrind --leak-check=full --leak-resolution=high --show-leak-kinds=all --errors-for-leak-kinds=all --track-origins=yes --expensive-definedness-checks=yes "$@"
