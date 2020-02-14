import serial
import numpy as np
import pandas as pd
import cv2
import os
import glob


#getting the usb serial port. This might be specific to MacOS and linux
ports = os.listdir("/dev/")
ports = [p for p in ports if p.startswith("cu.usb")]

if len(ports) == 0:
    print ("Device not found")
elif len(ports) > 1:
    print("Too many usb devices are connected. currently assuming that there is one usb device only.")

ser = serial.Serial(port=os.path.join("/dev/",ports[0]),baudrate=115200,timeout=1)

init_flag = 0
res= 1000
i = 0
while True: 

    if (ser.inWaiting() > 0):
        line = ser.readline()
        line = line.decode("utf-8")
        #print(line)
        #data = line.split(',')[1:-1]
        data = line.split(',')[(-24*32)-1:-1] # makes sure it parses correctly
        print(data)

        sensorData = np.array(data).astype(float)
        print(sensorData)
        # print(np.max(sensorData))

        sensorData = (sensorData * res)
        sensorData = sensorData - sensorData.min()
        sensorData = sensorData / sensorData.max()
        sensorData = sensorData*225
        #print(sensorData.min(),sensorData.mean(),sensorData.max())
        sensorData = sensorData.astype(np.uint8)
        sensorData = sensorData.reshape(24,32)
        sensorData = cv2.resize(sensorData, (320,240))
        sensorData = cv2.applyColorMap(sensorData, cv2.COLORMAP_JET)
        cv2.imshow("Data", sensorData)
        # saving the images
        cv2.imwrite('images/{:>05}.png'.format(i), sensorData) #'images/{:>05}.png'.format(i), makes photo files

        i += 1
    if cv2.waitKey(1) & 0xFF == ord('q'):
        # means_pd = pd.DataFrame(data = means)
        # means_pd.to_csv("means_second.csv")
        break



filename = 'video.avi'
frames_per_second = 4.0
res = '320p'

VIDEO_TYPE = {
    'avi': cv2.VideoWriter_fourcc(*'XVID'),
    #'mp4': cv2.VideoWriter_fourcc(*'H264'),
    'mp4': cv2.VideoWriter_fourcc(*'XVID'),
}

def get_video_type(filename):
    filename, ext = os.path.splitext(filename)
    if ext in VIDEO_TYPE:
      return  VIDEO_TYPE[ext]
    return VIDEO_TYPE['avi']



out = cv2.VideoWriter(filename, get_video_type(filename), frames_per_second, (320,240))


image_list = glob.glob(f"images/*.png")
sorted_images = sorted(image_list)

clear_images = True

for file in sorted_images:
    print(file)
    image_frame = cv2.imread(file, -1)
    out.write(image_frame)
for file in image_list:
    os.remove(file)


out.release()
cv2.destroyAllWindows()