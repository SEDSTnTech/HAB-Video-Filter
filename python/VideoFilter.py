import cv2
import numpy as np
import sys
import time
import itertools
import threading

start = time.perf_counter()

# Retain: frames or clips that were 'kept in' from the original video, i.e. added to the output file
# Note that audio is not retained in output video file, if audio is desired it will need to be readded manually
# Audio could be lined up by using the timestamps of clips used from the original video, printed at the end of the program

fileName = sys.argv[1]          # when running script first argument is the filename and the second is the output filename and third is the blue threshold
outputFileName = sys.argv[2]    # output filename
threshold = float(sys.argv[3])  # what fraction of frame that must be blue in order to be cut (ex: more than 40% -> 0.4, more than 90% -> 0.9)

# Because cutting videos may take a while, the loading animation tells the user that the process has not frozen
done = False
def animate():                  # Loading animation that displays while video is being cut
    for c in itertools.cycle(['|', '/', '-', '\\']):
        if done:
            break
        sys.stdout.write('\rCutting Video ' + c)
        sys.stdout.flush()
        time.sleep(0.1)

def printTime(seconds):
    return f"{int(seconds//3600):02d}:{int(seconds//60):02d}:{round(seconds % 60, 2):05.2f}"

capture = cv2.VideoCapture(fileName)
if (capture.isOpened() == False):
    print("Could not open file", fileName)

width = int(capture.get(3))     # gets information about video using the cv2 'get' function
height = int(capture.get(4))
fps = capture.get(5)
frameCount = int(capture.get(7))
TotalNumPixels = height * width

fourcc = cv2.VideoWriter_fourcc(*'mp4v')    # If output is desired in .avi format, change *'mp4v' to *'xvid' (output filename must also be .avi)
out = cv2.VideoWriter(outputFileName, fourcc, fps, (width,height))
duration = printTime(frameCount/fps)

print("\n******************** Video Info ********************\n")
print(f"Video Dimensions: {width} x {height}")
print(f"Video Framerate: {fps}")
print(f"Video Duration: {duration}")
print("\n****************************************************\n")

t = threading.Thread(target=animate) # Begins loading animation
t.start()

frameCount = 0
cutCount = 0
lastRet = False
timeStamps = []

while True:
    ret, frame = capture.read()

    if ret == False:            # ret returns false when there are no frames left in the input video
        break                   # exits program

    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

    lowerBlue = np.array([100, 50, 50])
    upperBlue = np.array([130, 255, 255])

    mask = cv2.inRange(hsv, lowerBlue, upperBlue)
    numWhitePixels = np.sum(mask == 255)
    numBlackPixels = np.sum(mask == 0)

    # cv2.imshow('frame', frame) # will display original video in real time
    # cv2.imshow('mask', frame)   # will display masked video in real time - Note: these slow down program

    if ((numWhitePixels / TotalNumPixels ) > threshold):    # Cuts frame if more than [threshold] of frame is blue
        #print(f"Frame {frameCount}: REMOVED")
        cutCount += 1
        if (lastRet == True):                               # If last clip was not cut out and this one is, record that frame
            timeStamps.append(frameCount)
        lastRet = False
    else:
        #print(f"Frame {frameCount}: RETAINED")
        out.write(frame)
        if (lastRet == False):                              # If last clip was cut out and this clip is not, record that frame
            timeStamps.append(frameCount)                   # These record where the 'clips' retained start and end, used to find the timestamps
        lastRet = True                                              # at the end so that the clips used can be found in the original video

    frameCount += 1

if (lastRet == True):
    timeStamps.append(frameCount)

end = time.perf_counter()
done = True

print("\nVideo Cutting Complete")
print("\n******************** Edit Info *********************\n")
print(f"Blue Frame Threshold: {threshold*100}%")
print(f"{cutCount} of {frameCount} total frames ({round((cutCount/frameCount)*100,2)}%) were cut")
print(f"Time Elapsed: {printTime(end-start)}")
print("\n****************************************************\n")

capture.release()
out.release()
cv2.destroyAllWindows()

print("\n******************** Timestamps *********************\n")
print("Timestamps of Clips Retained from Original Video:")
for i, stamp in enumerate(timeStamps):
    if ((i % 2) == 0):
        print()
    elif ((i % 2) == 1):
        print(" - ", end='')
    print(printTime(stamp/fps), end='')
print("\n")
