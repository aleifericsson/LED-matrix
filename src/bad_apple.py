import cv2

file = open("C:/dev/bad_apple.txt", "w")

cap = cv2.VideoCapture("C:\\Users\\ikram\\Downloads\\Bad Apple.mp4")
success, image = cap.read()
count = 0
while success:
    count += 1
    file.write("{\n")

    for y in range(0,10):
        file.write("0b")
        for x in range(0,10):
            v = image[y, x]
            if (v[0] >= 128):
                file.write("1")
            else:
                file.write("0")
        file.write(",\n")

    for y in range(0,10):
        file.write("0b")
        for x in range(0,10):
            v = image[y, x]
            if (v[0] >= 64 and v[0] < 128) or (v[0] >= 192):
                file.write("1")
            else:
                file.write("0")
        file.write(",\n")

    file.write("},\n")
    success, image = cap.read()
cap.release()
cv2.destroyAllWindows()