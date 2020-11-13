import  cv2 
img = cv2.imread("3.setShade_pic/setShade.png",0)
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





# centerQ2 = (0,)
# centerQ3 = (int(height/2),)
# centerQ4 = (int(height/2),)

# img_Q = []
# img_Q1 =img[0:int(height/2), int(width/2):width]
# img_Q2 =img[0:int(height/2), 0:int(width/2)]
# img_Q3 =img[int(height/2):height, 0:int(width/2)]
# img_Q4 =img[int(height/2):height, int(width/2):width]

# img_Q.append(img_Q1)
# img_Q.append(img_Q2)
# img_Q.append(img_Q3)
# img_Q.append(img_Q4)
# binary_code = ""



# for i in range(0 , len(img_Q) ):
#     if img_Q[i][30][30] == 255 :
#         binary_code += "1"
#     else :
#         binary_code += "0"
# print(binary_code)



# print(img_Q1[30][30],img_Q2[30][30],img_Q3[30][30],img_Q4[30][30])
# centerQ = int(img_Q1.shape[0]/2),int(img_Q1.shape[1]/2)


# p1 = img_Q1[centerQ[0]-5][centerQ[1]+5]
# p2 = img_Q1[centerQ[0]-5][centerQ[1]-5]
# p3 = img_Q1[centerQ[0]+5][centerQ[1]-5]
# p4 = img_Q1[centerQ[0]+5][centerQ[1]+5]

# print(img_Q2[centerQ[0]-5][centerQ[1]+5])
# print(img_Q2[centerQ[0]-5][centerQ[1]-5])
# print(img_Q2[centerQ[0]+5][centerQ[1]-5])
# print(img_Q2[centerQ[0]+5][centerQ[1]+5])

# print(img_Q3[centerQ[0]-5][centerQ[1]+5])
# print(img_Q3[centerQ[0]-5][centerQ[1]-5])
# print(img_Q3[centerQ[0]+5][centerQ[1]-5])
# print(img_Q3[centerQ[0]+5][centerQ[1]+5])

# print(img_Q4[centerQ[0]-5][centerQ[1]+5])
# print(img_Q4[centerQ[0]-5][centerQ[1]-5])
# print(img_Q4[centerQ[0]+5][centerQ[1]-5])
# print(img_Q4[centerQ[0]+5][centerQ[1]+5])

