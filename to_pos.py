def int_to_pos(a):
    out = ""

    for row in range(7, -1, -1):
        for col in range(8):
            if (1 << (row * 8)) * (1 << col) & a:
                out += "P"
            else:
                out += "."
    out = list(out)

    return "\n".join(["".join(out[8*i:8*i+8]) for i in range(8)])

def pad_int(w):
    return "0x" + hex(w)[2:].rjust(16, "0")

# row = rank
# col = file
def pos_to_int(place):
    col = -ord('a') + ord(place[0].lower())
    assert 0 <= col <= 7 
    row = int(place[1]) - 1
    assert 0 <= row <= 7 
    w = (1 << (row * 8)) * (1 << col)
    return pad_int(w)

def multi_pos_to_int(arr):
    tot = 0
    for elem in arr:
        pc = eval(pos_to_int(elem))
        tot |= pc
    return pad_int(tot)

while True:
    print("1 for int_to_pos")
    print("2 for pos_to_int")
    print("3 for multi_pos_to_int")
    print("4 for pos_to_square")
    print("5 for int_to_square (not implemented?)")
    x = int(input("> "))

    if x == 1:
        print("input number")
        w = eval(input("> "))
        assert type(w) == int
        print(int_to_pos(w))
    if x == 2:
        print("input location (e.g. e4)")
        loc = input("> ")
        print(pos_to_int(loc))
    if x == 3:
        print("input multiple locations, formatted like [\"e2e4\", \"b1b7\"]")
        loc = eval(input("> "))
        print(multi_pos_to_int(loc))
    if x == 4:
        print("input square, (e.g. e4")
        loc = input("> ")
        print(square_to_int())
