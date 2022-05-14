import os 
import json
import csv
import numpy as np
from scipy.fftpack import fft
import matplotlib.pyplot as plt
from scipy import signal

file = open('record.txt')

csvreader = csv.reader(file)
rows = []
for row in csvreader:
  
  accx = float(row[0])
  accy = float(row[1])
  accz = float(row[2])
  acc = accx*accx+ accy*accy+accz*accz
  acc= accx
  rows.append(acc)


#rows = np.array(rows)
x = np.abs((rows))
x = x- np.average(x)

f, t, Zxx = signal.stft(x, fs=200, nperseg=1024,noverlap= 512)
Zxx = np.abs(Zxx)

Zxx = Zxx[5:100,]
f = f[5:100]

print((Zxx[:,1].shape))
print(f.shape)
print(t.shape)

plt.pcolormesh(t, f, (Zxx))
plt.show()


file.close()


