#!/bin/bash

make distclean 2> /dev/null

./configure && make
#./configure DEFINES+=CONFIG__ENABLE_FORM_EDITOR_TEST && make -j3
