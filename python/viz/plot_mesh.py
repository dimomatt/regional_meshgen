import argparse
from netCDF4 import Dataset
import matplotlib
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

# Define parser
parser = argparse.ArgumentParser(description="Plot a mesh file in matplotlib")
parser.add_argument('filename', type=str, help='Path to the netCDF file')

args = parser.parse_args()

# load the file
dataset = Dataset(args.filename)

x_cell = dataset.variables['xCell'][:]
y_cell = dataset.variables['yCell'][:]
z_cell = dataset.variables['zCell'][:]




dataset.close()

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
ax.scatter(x_cell, y_cell, z_cell)
ax.set_xlabel('x (m)')
ax.set_ylabel('y (m)')
ax.set_zlabel('z (m)')

for i, elem in enumerate(x_cell):
  ax.text(x_cell[i], y_cell[i], z_cell[i], str(i))

plt.savefig('out.png')
