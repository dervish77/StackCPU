#!/bin/bash
#
# coverage -- calculates coverage percentage using GCOV report
#

if [ $# -lt "1" ]
then
	echo "Usage: coverage <report>"
	echo "i.e.:  coverage report.gcov"
	exit
fi

echo " "
echo "COVERAGE"

filename=$1

nottested=`cat $filename | grep "#####" | wc -l`
notexec=`cat $filename | grep " -: " | wc -l`
total=`cat $filename | wc -l`

#echo " "
#echo "not tested   = $nottested"
#echo "not executed = $notexec"
#echo "total        = $total"
#echo " "

let executed=$total-$notexec
let tested=$executed-$nottested
echo "tested       = $tested"
echo "executed     = $executed"

echo " "
let tested100=$tested*100
let coverage=($tested100/$executed)
echo "coverage     = $coverage% of $executed"
echo " "

exit

