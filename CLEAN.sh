#!/bin/bash

make distclean 2> /dev/null
DIR="src"
rm -r "$DIR/app/sfd" 2> /dev/null
find $DIR -name "Makefile" | xargs rm 2> /dev/null
find $DIR -name "*.o" | xargs rm 2> /dev/null
find $DIR -name "*.moc" | xargs rm 2> /dev/null
find $DIR -name "moc_*.?pp" | xargs rm 2> /dev/null
find $DIR -name "ui_*.?pp" | xargs rm 2> /dev/null
find $DIR -name "lib*.so*" | xargs rm 2> /dev/null
