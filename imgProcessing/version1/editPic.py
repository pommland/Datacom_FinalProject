import cv2
import numpy as np 

def cropPic(path_IN,path_OUT):        # function ครอปตรงกลางรูป เริ่มที่ ตำแหน่ง x= 60 y=80 ขนาด 160*160 พิกเซล
    x=40
    y=80
    h=160
    w=160
    img = cv2.imread(path_IN, 0)   # read path รูปที่จะครอป 
    crop = img[y:y+h, x:x+w]    # ทำการ ครอป
    cv2.imwrite(path_OUT, crop) # save path ไปที่ ตำแหน่งที่ต้องการ save


def setShade(path_IN,path_OUT): #แก้เฉดสีให้เป็นขาว-ดำสุดไปเลย
    img = cv2.imread(path_IN,cv2.IMREAD_GRAYSCALE)
    img_binary = cv2.threshold(img, 127, 255, cv2.THRESH_BINARY)[1] # trans เฉด https://opencv-python-tutroals.readthedocs.io/en/latest/py_tutorials/py_imgproc/py_thresholding/py_thresholding.html
    cv2.imwrite(path_OUT,img_binary)


def resizePic(path_IN,path_OUT): # Function ลดขนาดรูป
    img = cv2.imread(path_IN,0) 
    W = 60  
    height, width = img.shape  
    imgScale = W/width    
    new_x,new_y = width*imgScale, height*imgScale  # newX,newY เก็บค่า x y 
    newImg = cv2.resize(img,(int(new_x),int(new_y)))  # ทำการลดขนาดภาพ
    cv2.imwrite(path_OUT, newImg)  # save path ไปที่ ตำแหน่งที่ต้องการ save


def rePicture(pathOriginal,pathCrop,pathSetShade,pathResize): # function รวมการปรับแต่งรูป
    cropPic(pathOriginal,pathCrop)   # function ครอปตรงกลางรูป 
    setShade(pathCrop,pathSetShade)    # Function ทำให้รูปสีเข้มขึ้น
    resizePic(pathSetShade,pathResize)       # Function ลดขนาดรูป
    getValue(pathSetShade)


def getValue(pathSetShade):
    img = cv2.imread(pathSetShade,0)
    height,width = img.shape

    center_img = (height//2,width//2)
    centerQ1 = (center_img[0]//2,center_img[1]//2+center_img[1])
    centerQ2 = (center_img[0]//2,center_img[1]//2)
    centerQ3 = (center_img[0]//2+center_img[0],center_img[1]//2)
    centerQ4 = (center_img[0]//2+center_img[0],center_img[1]//2+center_img[1])

    Q1_YP1P2 = centerQ1[0]-5
    Q1_XP2P3 = centerQ1[1]-5
    Q1_YP3P4 = centerQ1[0]+5
    Q1_XP1P4 = centerQ1[1]+5
    Q1P1 = int(img[Q1_YP1P2][Q1_XP1P4])
    Q1P2 = int(img[Q1_YP1P2][Q1_XP2P3])
    Q1P3 = int(img[Q1_YP3P4][Q1_XP2P3])
    Q1P4 = int(img[Q1_YP3P4][Q1_XP1P4])
    avrQ1 = (Q1P1+Q1P2+Q1P3+Q1P4)//4
    print('--------------',avrQ1)
    image2 = cv2.rectangle(img,(Q1_YP1P2,Q1_XP2P3), (Q1_YP3P4,Q1_XP1P4),(255,40,40), 1) 
    cv2.imshow('Contours', image2) 
    cv2.waitKey(0) 
    cv2.destroyAllWindows() 
        

    Q2_YP1P2 = centerQ2[0]-5
    Q2_XP2P3 = centerQ2[1]-5
    Q2_YP3P4 = centerQ2[0]+5
    Q2_XP1P4 = centerQ2[1]+5
    Q2P1 = int(img[Q2_YP1P2][Q2_XP1P4])
    Q2P2 = int(img[Q2_YP1P2][Q2_XP2P3])
    Q2P3 = int(img[Q2_YP3P4][Q2_XP2P3])
    Q2P4 = int(img[Q2_YP3P4][Q2_XP1P4])
    avrQ2 = (Q2P1+Q2P2+Q2P3+Q2P4)//4




    Q3_YP1P2 = centerQ3[0]-5
    Q3_XP2P3 = centerQ3[1]-5
    Q3_YP3P4 = centerQ3[0]+5
    Q3_XP1P4 = centerQ3[1]+5
    Q3P1 = int(img[Q3_YP1P2][Q3_XP1P4])
    Q3P2 = int(img[Q3_YP1P2][Q3_XP2P3])
    Q3P3 = int(img[Q3_YP3P4][Q3_XP2P3])
    Q3P4 = int(img[Q3_YP3P4][Q3_XP1P4])
    avrQ3 = (Q3P1+Q3P2+Q3P3+Q3P4)//4





    Q4_YP1P2 = centerQ4[0]-5
    Q4_XP2P3 = centerQ4[1]-5
    Q4_YP3P4 = centerQ4[0]+5
    Q4_XP1P4 = centerQ4[1]+5
    Q4P1 = int(img[Q4_YP1P2][Q4_XP1P4])
    Q4P2 = int(img[Q4_YP1P2][Q4_XP2P3])
    Q4P3 = int(img[Q4_YP3P4][Q4_XP2P3])
    Q4P4 = int(img[Q4_YP3P4][Q4_XP1P4])
    avrQ4 = (Q4P1+Q4P2+Q4P3+Q4P4)//4







    print("________")
    print(centerQ1)
    print(centerQ2)
    print(centerQ3)
    print(centerQ4)
    print("________")

    print(Q1_YP1P2)
    print(Q1_XP2P3)
    print(Q1_YP3P4)
    print(Q1_XP1P4)


    binary_code = ""
    for i in centerQ1,centerQ2,centerQ3,centerQ4:
        if (img[i[0]][i[1]]) == 255:
            binary_code  +="1"
        else :
            binary_code += "0"
    print(binary_code )

    print("________")
    print(Q1P1)
    print(Q1P2)
    print(Q1P3)
    print(Q1P4)
    print(avrQ1)
    print("________")
    print(Q2P1)
    print(Q2P2)
    print(Q2P3)
    print(Q2P4)
    print(avrQ2)
    print("________")
    print(Q3P1)
    print(Q3P2)
    print(Q3P3)
    print(Q3P4)
    print(avrQ3)
    print("________")
    print(Q4P1)
    print(Q4P2)
    print(Q4P3)
    print(Q4P4)
    print(avrQ4)


# rePicture("1.original_pic/original.png","2.crop_pic/crop.png","3.setShade_pic/setShade.png","4.resize_pic/resize.png")
# rePicture("1.original_pic/g1.bmp","2.crop_pic/g2.bmp","3.setShade_pic/g3.bmp","4.resize_pic/g4.bmp")

# img = cv2.imread()

# cropPic("1.original_pic/g1.bmp","1.original_pic/g2.bmp")   # function ครอปตรงกลางรูป 
    # setShade(pathCrop,pathSetShade)    # Function ทำให้รูปสีเข้มขึ้น
    # resizePic(pathSetShade,pathResize)       # Function ลดขนาดรูป
    # getValue(pathSetShade)

x=20
y=20
h=120
w=120
img = cv2.imread("1.original_pic/g1.bmp", 0)   # read path รูปที่จะครอป 
crop = img[80:240, 40:200]    # ทำการ ครอป
cv2.imwrite("2.crop_pic/g2.bmp", crop) # save path ไปที่ ตำแหน่งที่ต้องการ save
getValue("2.crop_pic/g2.bmp")



# # Let's load a simple image with 3 black squares 
# image = cv2.imread("2.crop_pic/g2.bmp") 
# cv2.waitKey(0) 
  
# # Grayscale 
# gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY) 
  
# # Find Canny edges 
# edged = cv2.Canny(gray, 30, 200) 
# cv2.waitKey(0) 
  
# # Finding Contours 
# # Use a copy of the image e.g. edged.copy() 
# # since findContours alters the image 
# contours, hierarchy = cv2.findContours(edged,  
#     cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE) 
  
# cv2.imshow('Canny Edges After Contouring', edged) 
# cv2.waitKey(0) 
  
# print("Number of Contours found = " + str(len(contours))) 
  
# # Draw all contours 
# # -1 signifies drawing all contours 
# cv2.drawContours(image, contours, -1, (0, 255, 0), 3) 
  
# cv2.imshow('Contours', image) 
# cv2.waitKey(0) 
# cv2.destroyAllWindows() 