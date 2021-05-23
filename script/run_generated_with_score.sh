#!/bin/bash

set -eu
cd `dirname $0`/..

TOTAL_SCORE=0
mkdir -p out/results

for FILE in `ls out/cases`; do
  FILE_STDIN="out/cases/$FILE"
  FILE_STDOUT="out/results/$FILE"

  SCORE_LINE=`./bin/run.sh $FILE_STDIN 2>&1 > $FILE_STDOUT | tail -n1`
  [[ $SCORE_LINE =~ [0-9]+$ ]] && SCORE=${BASH_REMATCH[0]}
  FILE_VIS_TMP=`./bin/visualize.sh $FILE_STDIN $FILE_STDOUT`
  mv $FILE_VIS_TMP "out/results/$FILE.svg"
  echo "case '$FILE': score=$SCORE"
  TOTAL_SCORE=$(($TOTAL_SCORE+$SCORE))
done
echo "total score:"
echo $TOTAL_SCORE
