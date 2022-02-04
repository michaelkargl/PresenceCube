screen_width=176
screen_height=264
pixel_character="x"

i=0
with open('./display.bin', 'rb') as file:
    while True:
        byte = file.read(1)
        if not byte:
            break;
        
        print(' ', end="") if byte.hex() == '00' else print(pixel_character, end="")
        if (i % screen_width) == 0:
            print();
        i+=1

print(i);
