#! /bin/sh
# t-option.sh:  Testing for the -t option.   Make sure we get the 
#               correct SIDs with and without the "-t" option.

# Import common functions & definitions.
. ../common/test-common


# Get a test file...
g=testfile
s=s.$g
x=x.$g
p=p.$g
z=z.$g
remove $s $g $p $z


get_expect () {
label=$1         ; shift
r_option=$1      ; shift
sid_expected=$1  ; shift
docommand $label "${get} ${toption} -g ${r_option} $s" \
    0 "$sid_expected\n" IGNORE
}

## Create the file (empty).
docommand prep1 "${admin} -n $s" 0 "" ""

## make some deltas.
docommand prep2 "${get} -e $s"    0  "1.1\nnew delta 1.2\n0 lines\n" ""
docommand prep3 "${delta} -y $s"  \
    0  "1.2\n0 inserted\n0 deleted\n0 unchanged\n" IGNORE
docommand prep4 "${get} -e -r2 $s"   0  "1.2\nnew delta 2.1\n0 lines\n" ""
docommand prep5 "${delta} -y $s"  \
    0  "2.1\n0 inserted\n0 deleted\n0 unchanged\n" IGNORE



# Do various forms of get on the file and make sure we get the right SID.
toption=""
get_expect x1 -r1.1 1.1
get_expect x2 -r1.2 1.2
get_expect x3 -r2.1 2.1
get_expect x4 -r2   2.1
get_expect x5 ""    2.1
get_expect x6 -r1   1.2

toption="-t"
get_expect tx1 -r1.1 1.1
get_expect tx2 -r1.2 1.2
get_expect tx3 -r2.1 2.1
get_expect tx4 -r2   2.1
get_expect tx5 ""    2.1
get_expect tx6 -r1   1.2


## Now make a branch at the top of the trunk.
docommand prep6 "${admin} -fb $s" 0 "" ""
docommand prep7 "${get} -e -b $s" 0 "2.1\nnew delta 2.1.1.1\n0 lines\n" IGNORE
docommand prep8 "${delta} -y $s" 0 \
	"2.1.1.1\n0 inserted\n0 deleted\n0 unchanged\n" IGNORE
docommand prep9 "${get} -e -r2.1.1 $s" 0 \
	"2.1.1.1\nnew delta 2.1.1.2\n0 lines\n" IGNORE
docommand prep10 "${delta} -y $s" 0 \
	"2.1.1.2\n0 inserted\n0 deleted\n0 unchanged\n" IGNORE

## Also make a branch on release 1.
docommand prep11 "${get} -e -r1.2 $s" 0 \
	"1.2\nnew delta 1.2.1.1\n0 lines\n" IGNORE
docommand prep12 "${delta} -y $s" 0 \
	"1.2.1.1\n0 inserted\n0 deleted\n0 unchanged\n" IGNORE


toption=""
get_expect y1 -r1.1 1.1
get_expect y2 -r1.2 1.2
get_expect y3 -r2.1 2.1
get_expect y4 -r2   2.1
get_expect y5 ""    2.1
get_expect y6 -r1   1.2

toption="-t"
get_expect ty1 -r1.1 1.1
get_expect ty2 -r1.2 1.2.1.1
get_expect ty3 -r2.1 2.1.1.2
get_expect ty4 -r2   2.1.1.2
get_expect ty5 ""    2.1.1.2
get_expect ty6 -r1   1.2.1.1


## Set the default delta flag and test the -t option's effect
## (the -t flag should not over-ride it).
docommand prep13 "${admin} -fd1.1 $s" 0 "" ""

toption=""
get_expect d1 ""     1.1
toption="-t"
get_expect td1 ""    1.1


## Set the release ceiling...
docommand prep14 "${admin} -dd $s" 0 "" ""  # remove default delta flag
docommand prep15 "${admin} -fc1 $s" 0 "" "" # set release ceiling to 1.

# Testing on "real" SCCS shows that the release ceiling apparently has
# no effect.  This is surely wrong, given the documentation. [jay@gnu.org]
toption=""
get_expect c1 ""     2.1			# This is surely wrong!
toption="-t"
get_expect tc1 ""    2.1.1.2			# This is surely wrong!

remove $s
success
