import numpy as np

number = 550
Y_z_down = ([0, 0, 1])
Y_z_up = ([0, 0, -1])
Y_y_down = ([0, 1, 0])
Y_y_up = ([0, -1, 0])
Y_x_down = ([1, 0, 0])
Y_x_up = ([-1, 0, 0])

xDown = open('data/X_down_filtered.txt', 'r')
xDownMatrix = [map(str, line.strip(' \n').split(' ')) for line in xDown]
xDownMatrix = np.asarray(xDownMatrix, float)

xUp = open('data/X_up_filtered.txt', 'r')
xUpMatrix = [map(str, line.strip(' \n').split(' ')) for line in xUp]
xUpMatrix = np.asarray(xUpMatrix, float)

yDown = open('data/Y_down_filtered.txt', 'r')
yDownMatrix = [map(str, line.strip(' \n').split(' ')) for line in yDown]
yDownMatrix = np.asarray(yDownMatrix, float)

yUp = open('data/Y_up_filtered.txt', 'r')
yUpMatrix = [map(str, line.strip(' \n').split(' ')) for line in yUp]
yUpMatrix = np.asarray(yUpMatrix, float)

zDown = open('data/Z_down_filtered.txt', 'r')
zDownMatrix = [map(str, line.strip(' \n').split(' ')) for line in zDown]
zDownMatrix = np.asarray(zDownMatrix, float)

zUp = open('data/Z_up_filtered.txt', 'r')
zUpMatrix = [map(str, line.strip(' \n').split(' ')) for line in zUp]
zUpMatrix = np.asarray(zUpMatrix, float)

YXD= [Y_x_down,]*number
YXU= [Y_x_up,]*number

YYU= [Y_y_up,]*number
YYD= [Y_y_down,]*number

YZU= [Y_z_up,]*number
YZD= [Y_z_down,]*number

p = np.append( YXD, YXU, 0)
p = np.append( p, YYU, 0)
p = np.append( p, YYD, 0)
p = np.append( p, YZU, 0)
p = np.append( p, YZD, 0)

Y = np.array( p )

w = np.append(xDownMatrix,xUpMatrix, 0)
w = np.append(w,yUpMatrix, 0)
w = np.append(w,yDownMatrix, 0)
w = np.append(w,zUpMatrix, 0)
w = np.append(w,zDownMatrix, 0)

W = np.array(w)
ones = np.vstack(np.repeat(1,3300))
new_W = np.append(W, ones, 1)

W_T = new_W.transpose()
I = np.dot(W_T, new_W)
O = np.linalg.inv(I)
M = np.dot(O, W_T)
G = np.dot(M, Y)
print G
