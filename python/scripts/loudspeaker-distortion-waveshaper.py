import numpy as np
import matplotlib.pyplot as plt

x = np.linspace(-1,1,1000)
print(x)
# y = 2*np.power(x,4)-1
a = 1
b = 1
c = 0
d = -1
e = 0.1
y = a*np.power(x,1)+b*np.power(x,2)+c*np.power(x,3)+d*np.power(x,4)+e*np.power(x,5)

fig, ax = plt.subplots()
ax.plot(x,y)
plt.show()
