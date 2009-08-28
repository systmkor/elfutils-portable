#! /bin/sh
# Copyright (C) 2009 Red Hat, Inc.
# This file is part of Red Hat elfutils.
#
# Red Hat elfutils is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by the
# Free Software Foundation; version 2 of the License.
#
# Red Hat elfutils is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with Red Hat elfutils; if not, write to the Free Software Foundation,
# Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301 USA.
#
# Red Hat elfutils is an included package of the Open Invention Network.
# An included package of the Open Invention Network is a package for which
# Open Invention Network licensees cross-license their patents.  No patent
# license is granted, either expressly or impliedly, by designation as an
# included package.  Should you wish to participate in the Open Invention
# Network licensing program, please visit www.openinventionnetwork.com
# <http://www.openinventionnetwork.com>.

. $srcdir/test-subr.sh

dwarfcmp=${dwarfcmp:-../src/dwarfcmp}
try_switches=${try_switches:- -iq -i -il}

status=0
run_one()
{
  file="$1"; shift
  testrun "$dwarfcmp" "$@" "$file" "$file" ||
  { echo "*** failure in $dwarfcmp $* on $file"; status=1; }
}

runtest()
{
  for file; do
    if [ -f $file ]; then
      for try in $try_switches; do
        run_one "$file" $try
      done
    fi
  done
}

runtest ../src/addr2line
runtest ../src/dwarfcmp
runtest ../src/dwarfcmp-test
runtest ../src/dwarflint
runtest ../src/elfcmp
runtest ../src/elflint
runtest ../src/findtextrel
runtest ../src/ld
runtest ../src/nm
runtest ../src/objdump
runtest ../src/readelf
runtest ../src/size
runtest ../src/strip
runtest ../src/unstrip
runtest ../*/*.so

exit $status
