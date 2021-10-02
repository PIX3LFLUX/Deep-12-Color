#!/bin/bash

FRAMERATE=60

cp ../output_* .

for i in {0..1000}; do
  cp output_0.ppm output_${i}_0.ppm
  cp output_1.ppm output_${i}_1.ppm
  cp output_2.ppm output_${i}_2.ppm
  cp output_3.ppm output_${i}_3.ppm
done

# -crf 0 is very important, for losslessness
ffmpeg -framerate $FRAMERATE -pattern_type glob -i "output_*.ppm" -c:v libx264 -pix_fmt rgb24 -crf 0 out.mkv

rm *ppm
