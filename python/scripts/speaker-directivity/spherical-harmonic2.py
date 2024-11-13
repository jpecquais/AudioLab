import numpy as np
from matplotlib import pyplot as plt

fig, ax = plt.subplots(subplot_kw={'projection': 'polar'})
theta = np.linspace(0,2*np.pi,2000)


speaker_diameter = 12 #inch
speaker_diameter *= 0.0254 #m

# print(speaker_diameter)
circumference = speaker_diameter*np.pi
# print(circumference)
f0 = 340/circumference
freq = [f0,2*f0,4*f0,8*f0] #Hz
focus = [1,2,4,8]

number_octave = np.log(20000/f0)/np.log(2)
ax.plot(theta,np.ones(len(theta)))
idx = 0
for f in freq:
    wavelen = 340/f
    ratio = circumference/wavelen
    alpha = 0.5
    r = (1-alpha)*np.abs(np.cos(ratio*theta))+alpha*np.power(np.abs(np.cos(theta)),focus[idx])
    ax.plot(theta,r,label="ratio: "+str(ratio))
    idx += 1
ax.legend()
plt.show()