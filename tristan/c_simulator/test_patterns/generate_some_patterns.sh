#!/usr/bin/env bash

# ./patgen solid width height maxval color_r color_g color_b
# 192, 57, 43 is a nice warm red color, multiply by four and add offset from one to 15 -> 0001 to 1111

for i in {1..15}; do
  r=`expr 16 \* 192 + $i`
  g=`expr 16 \* 57 + $i`
  b=`expr 16 \* 43 + $i`
  ./patgen.py solid 100 100 4095 $r $g $b > testimage_solid_100_100_${r}_${g}_${b}_offset_${i}.ppm
done
