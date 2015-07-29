#!/bin/sh

set -xe

AUTOPOINT="${AUTOPOINT:-autopoint}"
INTLTOOLIZE="${INTLTOOLIZE:-intltoolize}"
AUTORECONF="${AUTORECONF:-autoreconf}"

if [[ "`uname`" == OpenBSD ]] ; then
    V="`ls -1 /usr/local/bin/autoreconf-* | env LC_ALL=C sort | tail -n 1`"
    V="${V##*-}"
    export AUTOCONF_VERSION="${AUTOCONF_VERSION:-$V}"

    V="`ls -1 /usr/local/bin/automake-* | env LC_ALL=C sort | tail -n 1`"
    V="${V##*-}"
    export AUTOMAKE_VERSION="${AUTOMAKE_VERSION:-$V}"
fi

#$AUTOPOINT --force || exit $?
#export AUTOPOINT="$INTLTOOLIZE --automake --copy"
$AUTORECONF --force --install --verbose

rm -rf autom4te.cache
