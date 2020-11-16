import cv2
import numpy as np

img = cv2.imread('ss.bmp')
img = cv2.rotate(img, cv2.cv2.ROTATE_90_CLOCKWISE) 
img = img[40:140,20:100]

gray = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)

frame = ''

# img = cv2.resize(img, (64, 64))
# Q2
cv2.circle(img,(10, 20), 1, (0,0,255), -1)
cv2.circle(img,(20, 20), 1, (0,0,255), -1)
cv2.circle(img,(10, 30), 1, (0,0,255), -1)
cv2.circle(img,(20, 30), 1, (0,0,255), -1)

frame_Q2 = f'(10,20):{gray[20][10]},(20,20):{gray[20][20]},(10,30):{gray[30][10]},(20,30):{gray[30][20]}'

# Q1
cv2.circle(img,(60, 20), 1, (0,0,255), -1)
cv2.circle(img,(70, 20), 1, (0,0,255), -1)
cv2.circle(img,(60, 30), 1, (0,0,255), -1)
cv2.circle(img,(70, 30), 1, (0,0,255), -1)

frame_Q1 = f'(60, 20):{gray[20][60]},(70, 20):{gray[20][70]},(60, 30):{gray[30][60]},(70, 30):{gray[30][70]}'

# Q4
cv2.circle(img,(60, 60), 1, (0,0,255), -1)
cv2.circle(img,(70, 60), 1, (0,0,255), -1)
cv2.circle(img,(60, 70), 1, (0,0,255), -1)
cv2.circle(img,(70, 70), 1, (0,0,255), -1)

frame_Q4 = f'(60, 60):{gray[60][60]},(70, 60):{gray[60][70]},(60, 70):{gray[70][60]},(70, 70):{gray[70][70]}'

# Q3
cv2.circle(img,(20, 60), 1, (0,0,255), -1)
cv2.circle(img,(30, 60), 1, (0,0,255), -1)
cv2.circle(img,(20, 70), 1, (0,0,255), -1)
cv2.circle(img,(30, 70), 1, (0,0,255), -1)

frame_Q3 = f'(20, 60):{gray[60][20]},(30, 60):{gray[60][30]},(20, 70):{gray[70][20]},(30, 70):{gray[70][30]}'

frame = f'{frame_Q1}|{frame_Q2}|{frame_Q3}|{frame_Q4}'

print(frame)

img = cv2.resize(img, (512, 512))

cv2.imshow('image',img)
cv2.waitKey(0)
cv2.destroyAllWindows()