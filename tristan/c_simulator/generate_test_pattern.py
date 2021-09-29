def main():
    f = open("test_pattern.ppm", "w")
    f.write("P3 100 100 4095\n")
    startvalue = 255;
    for x in range(100):
        for i in range(100):
            for j in range(3):
                f.write(str(startvalue + x) + " ")
        f.write("\n")
    f.close()


if __name__ == "__main__":
    main()
