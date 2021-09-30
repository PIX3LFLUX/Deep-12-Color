import sys;
def main():
    sys.stdout.write("P3 100 100 4095\n")
    startvalue = 255;
    for x in range(100):
        for i in range(100):
            for j in range(3):
                sys.stdout.write(str(startvalue + 4*x) + " ")
        sys.stdout.write("\n")


if __name__ == "__main__":
    main()
