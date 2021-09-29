#!/bin/bash

FRAMERATE=60
FRAMES=100

cp ../output_* .

for i in {0..$FRAMES}; do
  cp output_0.ppm output_${i}_0.ppm
  cp output_1.ppm output_${i}_1.ppm
  cp output_2.ppm output_${i}_2.ppm
  cp output_3.ppm output_${i}_3.ppm
done

ffmpeg -framerate $FRAMERATE -pattern_type glob -i 'output*.ppm' -c:v libx264 -pix_fmt rgb24 out.mp4 
