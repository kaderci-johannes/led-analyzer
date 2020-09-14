#!/bin/bash
i=1
N=`wc -l $1`
while read p; do
	echo "($i/$N)"
	echo "$p"
#	cmsRun /afs/cern.ch/work/o/okoseyan/public/CMSSW_8_0_16/src/LED/HFanalyzer/python/HFLEDP5.py $p 0
	i=$((i+1))
done < $1
root -q hfled_colz.C
