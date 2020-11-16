import cv2
import numpy as np

def process(img):
    filename = img
    img = cv2.imread(filename)
    img = cv2.rotate(img, cv2.cv2.ROTATE_90_CLOCKWISE) 
    img = img[40:140,20:100]
    img = cv2.resize(img, (64, 64))
    gray = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)

    _, thresh = cv2.threshold(gray, 50, 256, cv2.THRESH_BINARY)

    z32 = np.zeros((32, 32))
    o32 = np.ones((32, 32))

    templates = {
        '0000': np.zeros((64, 64), dtype=int),
        '0001': np.concatenate((np.zeros((32, 64)), np.concatenate((z32, o32), axis=1))),
        '0010': np.concatenate((np.zeros((32, 64)), np.concatenate((o32, z32), axis=1))),
        '0011': np.concatenate((np.zeros((32, 64), dtype=int), np.ones((32, 64), dtype=int))),
        '0100': np.concatenate((np.concatenate((z32, o32), axis=1), np.zeros((32, 64)))),
        '0101': np.concatenate((np.concatenate((z32, o32), axis=1), np.concatenate((z32, o32), axis=1))),
        '0110': np.concatenate((np.concatenate((z32, o32), axis=1), np.concatenate((o32, z32), axis=1))),
        '0111': np.concatenate((np.concatenate((z32, o32), axis=1), np.ones((32, 64)))),
        '1000': np.concatenate((np.concatenate((o32, z32), axis=1), np.zeros((32, 64)))),
        '1001': np.concatenate((np.concatenate((o32, z32), axis=1), np.concatenate((z32, o32), axis=1))),
        '1010': np.concatenate((np.concatenate((o32, z32), axis=1), np.concatenate((o32, z32), axis=1))),
        '1011': np.concatenate((np.concatenate((o32, z32), axis=1), np.ones((32, 64), dtype=int))),
        '1100': np.concatenate((np.ones((32, 64), dtype=int), np.zeros((32, 64), dtype=int))),
        '1101': np.concatenate((np.ones((32, 64), dtype=int),np.concatenate((z32, o32), axis=1))),
        '1110': np.concatenate((np.ones((32, 64), dtype=int),np.concatenate((o32, z32), axis=1))),
        '1111': np.ones((64, 64), dtype=int),
    }


    min_key = '0000'
    minn = 99999

    for key in templates.keys():
        if np.sum(np.logical_xor(thresh,templates[key])) < minn:
            min_key = key
            minn = np.sum(np.logical_xor(thresh,templates[key]))

    return min_key