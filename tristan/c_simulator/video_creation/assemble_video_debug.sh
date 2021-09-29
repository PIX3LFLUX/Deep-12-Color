
FRAMERATE_DEBUG=1
FRAMES_DEBUG=100


# =============
# DEBUG section
# =============

cp ../debug_* .

for i in {0..$FRAMES_DEBUG}; do
  cp debug_0.ppm debug_${i}_0.ppm
  cp debug_1.ppm debug_${i}_1.ppm
  cp debug_2.ppm debug_${i}_2.ppm
  cp debug_3.ppm debug_${i}_3.ppm
done

ffmpeg -framerate $FRAMERATE_DEBUG -pattern_type glob -i 'debug*.ppm' -c:v libx264 -pix_fmt rgb24 out_debug.mp4 
