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

x_vertex = dataset.variables['xVertex'][:]
y_vertex = dataset.variables['yVertex'][:]
z_vertex = dataset.variables['zVertex'][:]

x_cell_plane = dataset.variables['planeXCell'][:]
y_cell_plane = dataset.variables['planeYCell'][:]

x_vertex_plane = dataset.variables['planeXVertex'][:]
y_vertex_plane = dataset.variables['planeYVertex'][:]



fig = plt.figure()
ax = fig.add_subplot(111)
ax.scatter(x_cell_plane, y_cell_plane)
ax.scatter(x_vertex_plane, y_vertex_plane)

for i, elem in enumerate(x_cell_plane):
  ax.text(x_cell_plane[i], y_cell_plane[i], str(i))

for i, elem in enumerate(x_vertex_plane):
  ax.text(x_vertex_plane[i], y_vertex_plane[i], str(i))

plt.show()
dataset.close()
'''
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
ax.scatter(x_cell, y_cell, z_cell)
ax.scatter(x_vertex, y_vertex, z_vertex)
ax.set_xlabel('x (m)')
ax.set_ylabel('y (m)')
ax.set_zlabel('z (m)')





for i, elem in enumerate(x_cell):
  ax.text(x_cell[i], y_cell[i], z_cell[i], str(i))

for i, elem in enumerate(x_vertex):
  ax.text(x_vertex[i], y_vertex[i], z_vertex[i], str(i))
'''
plt.show()

#plt.savefig('out.png')
