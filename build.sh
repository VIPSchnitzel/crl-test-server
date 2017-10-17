#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

DEBUG=0
ASAN=0

OPT="-O3 -march=broadwell"
PROJECT_NAME="corelatorTest"
COMP_WARN="-Wall -Wtype-limits -Wsign-compare"
CC_COMMAND="gcc-7"
CFLAGS="-std=gnu11"
COMMON_LIB="-pthread -lm -l:libevhtp.a -levent_openssl -levent -lcrypto -lssl" 
#-pthread -lucy -lception -lspectaculator -ljemalloc -lm -ltabulator -l:libevhtp.a -levent_openssl -levent 
while [[ $# -gt 0 ]]; do

    case $1 in
        -dbg)
            DEBUG=1
            OPT="-g"
            shift
            ;;
        -asan)
            DEBUG=1
            ASAN=1
            OPT="-g -fsanitize=address"
            shift
            ;;
        *)
            shift
            ;;
    esac

done

#Get current git revision
GIT_BRANCH="$( git rev-parse --abbrev-ref HEAD )"
GIT_BRANCH_REV="$( git rev-list --count --branches $GIT_BRANCH )"
GIT_TAG="$( git describe --tags )"

if [[ $DEBUG -ne 1 ]]; then
    echo "Building $PROJECT_NAME $GIT_BRANCH release $GIT_BRANCH_REV, version $GIT_TAG"
else
    echo "Building $PROJECT_NAME with debugging symbols"
fi

#cp ../../crl_info.h ../../crl_info.h.bak.tmp
#Fill in Product Info
#sed -i "s/CRL_PRODUCT_REV.*/& $GIT_BRANCH_REV/" ../../crl_info.h
#sed -i "s/CRL_PRODUCT_TAG.*/& \"$GIT_TAG\"/" ../../crl_info.h
#sed -i "s/CRL_PRODUCT_BRANCH.*/& \"$GIT_BRANCH\"/" ../../crl_info.h

rm $PROJECT_NAME
$CC_COMMAND $OPT -o $PROJECT_NAME $DIR/src/*.c $COMP_WARN $COMMON_LIB $CFLAGS

#revert crl_info.h to pre-sed state
#mv -f crl_info.h.bak.tmp crl_info.h

if [ -e $PROJECT_NAME ]; then
   echo "Successfully compiled $PROJECT_NAME"
else
   echo "Failed to compile $PROJECT_NAME. Check output!"
   exit 1;
fi

exit 0;
