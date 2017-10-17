#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

cp -r $DIR/libevhtp/evhtp /usr/local/include/
cp $DIR/libevhtp/libevhtp.a /usr/local/lib/
mkdir -p /usr/local/lib/pkgconfig
cp $DIR/libevhtp/evhtp.pc /usr/local/lib/pkgconfig/

exit 0
