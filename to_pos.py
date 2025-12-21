def int_to_pos(a):
    out = ["."] * 64

    for i in range(64):
        if (1 << i) & a:
            out[63 - i] = "P"
        else:
            out[63 - i] = "."

    return "\n".join(["".join(out[8*i:8*i+8]) for i in range(8)])

# row = rank
# col = file
def pos_to_int(place):
    col = -ord('a') + ord(place[0].lower())
    assert 0 <= col <= 7 
    row = int(place[1]) - 1
    assert 0 <= row <= 7 
    w = (1 << (row * 8)) * (1 << (7-col))
    return hex(w)

while True:
    print("1 for int_to_pos")
    print("2 for pos_to_int")
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
