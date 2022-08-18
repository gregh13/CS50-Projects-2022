while True:
    try:
        height = int(input("Height: "))
        if height > 0 and height < 9:
            break
        print("Height must be an integer from 1 to 8")
    except ValueError:
        print("Height must be an integer from 1 to 8")

for x in range(1, height + 1):
    spaces = " " * (height - (x))
    blocks = "#" * x
    print(f"{spaces}{blocks}  {blocks}")
