# Some sample results for 12 bit gradient image

Resolution of image: 100 x 100, color gradient incremented by 1 on each color channel along the y axis.

`out_default.mkv` – The two LSBs are spatially dithered, bits 0b000000001100 are temporally dithered.
`out_switched.mkv` – Spatially and temorally dithered bits are switched.

Debug output that show the temporal and spatial dithering as a black / white image:

`out_debug_spatial_0011.mkv`
`out_debug_spatial_1100.mkv`
`out_debug_temporal_0011.mkv`
`out_debug_temporal_1100.mkv`
