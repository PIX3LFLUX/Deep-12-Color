#!/usr/bin/env python3

# Command structure:
# patgen = pattern generator
# ./patgen solid width height maxval color_r color_g color_b
# ./patgen gradient width height maxval color_r color_g color_b gradient_step

import sys;
def main():
    if sys.argv[1] == "solid":
        gradient_step = 0;
    elif sys.argv[1] != "gradient":
        print("Subcommand not implemented.")
        sys.exit(1)
    else: # gradient command
        gradient_step = int(sys.argv[8])


    width = int(sys.argv[2])
    height = int(sys.argv[3])
    maxval = int(sys.argv[4])
    color_r = int(sys.argv[5])
    color_g = int(sys.argv[6])
    color_b = int(sys.argv[7])
        
    sys.stdout.write("P3 " + str(width) + " " + str(height) + " " + str(maxval) + "\n")
    for y in range(100):
        for x in range(100):
            sys.stdout.write(str(color_r + y*gradient_step) + " ")
            sys.stdout.write(str(color_g + y*gradient_step) + " ")
            sys.stdout.write(str(color_b + y*gradient_step) + " ")
        sys.stdout.write("\n")


if __name__ == "__main__":
    main()
