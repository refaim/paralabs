#!/bin/bash
for f in `ls -rt *.msg`
do grep -q $1 $f&&cat $f
done