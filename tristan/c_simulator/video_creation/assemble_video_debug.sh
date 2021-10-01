
FRAMERATE_DEBUG=1
FRAMES_DEBUG=100


# =============
# DEBUG section
# =============

cp ../debug_* .

for i in {0..100}; do
  cp debug_temporal_0.ppm debug_temporal_${i}_0.ppm
  cp debug_temporal_1.ppm debug_temporal_${i}_1.ppm
  cp debug_temporal_2.ppm debug_temporal_${i}_2.ppm
  cp debug_temporal_3.ppm debug_temporal_${i}_3.ppm
done

rm out_debug_temporal.mp4
ffmpeg -framerate $FRAMERATE_DEBUG -pattern_type glob -i 'debug_temporal*.ppm' -c:v libx264 -pix_fmt rgb24 out_debug_temporal.mp4 

for i in {0..100}; do
  cp debug_spatial_0.ppm debug_spatial_${i}_0.ppm
  cp debug_spatial_1.ppm debug_spatial_${i}_1.ppm
  cp debug_spatial_2.ppm debug_spatial_${i}_2.ppm
  cp debug_spatial_3.ppm debug_spatial_${i}_3.ppm
done

rm out_debug_spatial.mp4
ffmpeg -framerate $FRAMERATE_DEBUG -pattern_type glob -i 'debug_spatial*.ppm' -c:v libx264 -pix_fmt rgb24 out_debug_spatial.mp4 
