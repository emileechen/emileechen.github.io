# Inverse Kinematics
# By Casey Currey-Wilson and Emilee Chen

import sys, argparse, math
import numpy as np
from numpy import linalg as LA
import pygame
from pygame.locals import *
from OpenGL.GL import *
from OpenGL.GLU import *
from OpenGL.GLUT import *

MOTION = True
SHADE_CIRCLE = True
RADIUS = 3.0
VEC_A = [1, -0.2, 0]
VEC_B = [0, -0.2, 1]
CIRCLE_CENTER = [0, 0, 0]
STEPS = 1000.0

class Arm:
	# lengths is a list of link lengths
	# angles is a list of tuples (exponential map)
	def __init__(self, angles, lengths):
		self.lengths = lengths
		self.angles = angles
		self.num_links = len(lengths)

	# Calculates end point position p of joint i.
	def get_p(self, i):
		m_list = []
		for j in range(i + 1):
			y = matrixRot(self.angles[i]) * matrixTrans(self.lengths[i], 0, 0)
			m_list.insert(0, y)
		x = matrixMultList(m_list)
		return x * np.matrix([0, 0, 0, 1]).getT()

	def jacobian(self):
		n = self.num_links
		j = []
		epsilon = 0.001
		for y in range(n):
			for x in range(3):
				j.append(vToThree(
					get_p(add_epsilon(self.angles, y, x, epsilon), self.lengths, n-1) -
					get_p(add_epsilon(self.angles, y, x, -1 * epsilon), self.lengths, n-1)) / (2 * epsilon))
		return matrixConcatList(j)

	# Iterative algorithm for inverse kinematics given a goal tuple (x, y, z).
	def i_kinematics(self, epsilon, goal, step=1.0):
		num_links = self.num_links
		ang = self.angles
		p_e = vToThree(get_p(ang, self.lengths, num_links-1))
		new_ang = ang
		while LA.norm(goal - p_e) > epsilon:
			if step < 0.0001:
				return new_ang
			else:
				# print(jacobian(ang, self.lengths))
				# print(goal)
				# print(p_e)
				# print("step: " + str(step))
				dr = (LA.pinv(jacobian(ang, self.lengths)) * (goal - p_e))
				new_ang = []
				new = []
				for i in range(num_links):
					new.append(vectorize(ang)[i] + (step * getvec(dr, i * 3, (i+1) * 3 - 1)))
				new_ang = devector(new)
				new_p_e = vToThree(get_p(new_ang, self.lengths, num_links-1))
				# The new guess is worse		
				# print("distances are: " + str(LA.norm(goal - p_e)) + " vs " + str(LA.norm(goal - new_p_e)))
				if LA.norm(goal - p_e) < LA.norm(goal - new_p_e) + 0.00001:
					step = step/2
				# The new guess is better
				else:
					step = 1.0
					p_e = new_p_e
					ang = new_ang
		return new_ang

	def get_lines(self):
		seg = []
		for i in range(self.num_links):
			# print(i)
			p2 = vToThree(get_p(self.angles, self.lengths, i))
			# print(p2)
			if i == 0:
				p1 = np.matrix([0, 0, 0]).getT()
			else:
				p1 = vToThree(get_p(self.angles, self.lengths, i - 1))
			seg.append((p1.getT().tolist()[0], p2.getT().tolist()[0]))
		return seg


def get_p(angles, lengths, i):
	m_list = []
	for j in range(i + 1):
		y = matrixRot(angles[j]) * matrixTrans(lengths[j], 0, 0)
		m_list.insert(0, y)
	x = matrixMultList(m_list)
	return x * np.matrix([0, 0, 0, 1]).getT()


def jacobian(angles, lengths):
	n = len(lengths)
	j = []
	epsilon = 0.001
	for y in range(n):
		for x in range(3):
			j.append(vToThree(
				get_p(add_epsilon(angles, y, x, epsilon), lengths, n-1) -
				get_p(add_epsilon(angles, y, x, -1 * epsilon), lengths, n-1)) / (2 * epsilon))
	return matrixConcatList(j)


# Adds ep to angles[n][i]
def add_epsilon(angles, n, i, ep):
	new_ang = angles.tolist()
	new_ang[n][i] = new_ang[n][i] + ep
	return np.array(new_ang)


def getvec(l, a, b):
	new_l = []
	for i in range(a, b+1):
		new_l.append(l.tolist()[i])
	return np.matrix(new_l)


def vToThree(v):
	num = v.shape[0]
	v = v.tolist()
	v.pop(num-1)
	return np.matrix(v)
 

def normalize(v):
	n = LA.norm(v)
	if n == 0:
		return v
	else:
		return np.divide(v, n)


def vectorize(l):
	new_l = []
	for a in l.tolist():
		new_l.append(np.matrix(a).getT())
	return new_l


def devector(l):
	new_l = []
	for a in l:
		new_l.append(a.getT().tolist()[0])
	return np.array(new_l)


# Returns a translation matrix with tx ty and tz.
def matrixTrans(x, y, z):
	return np.matrix(  [[1, 0, 0, x],
						[0, 1, 0, y],
						[0, 0, 1, z],
						[0, 0, 0, 1]]	)


# Returns a scaling matrix with sx sy and sz.
def matrixScal(x, y, z):
	return np.matrix(   [[x, 0, 0, 1],
						[0, y, 0, 1],
						[0, 0, z, 1],
						[0, 0, 0, 1]]	)


# Returns a rotation matrix from an expmap (rx, ry, rz)
def matrixRot(expmap):
	r = normalize(expmap)
	# theta = rotation angle in degrees
	theta = LA.norm(expmap)

	# Antisymmetric matrix
	A = np.matrix( [[0,	-1 * r[2], r[1]],
					[r[2], 0, -1 * r[0]],
					[-1 * r[1], r[0], 0]])

	# Rodriguez Formula
	M = (np.identity(3) + (A * np.sin(theta))) + ((A * A) * (1 - np.cos(theta)))
	return matrixToFour(M)


# Cross product matrix
def matrixCP(r):
	return np.matrix([[0, -1 * r[2], r[1]],
					  [r[2], 0, -1 * r[0]],
					  [-1 * r[2], r[0], 0]])


def matrixToThree(m):
	return np.matrix([[m.item(0, 0), m.item(0, 1), m.item(0, 2)],
					  [m.item(1, 0), m.item(1, 1), m.item(1, 2)],
					  [m.item(2, 0), m.item(2, 1), m.item(2, 2)]])


def matrixToFour(m):
	return np.matrix([[m.item(0, 0), m.item(0, 1), m.item(0, 2), 0],
					  [m.item(1, 0), m.item(1, 1), m.item(1, 2), 0],
					  [m.item(2, 0), m.item(2, 1), m.item(2, 2), 0], 
					  [0, 0, 0, 1]])


def matrixConcat(m, n):
	new = []
	for i in range(len(m)):
		new.append(m.tolist()[i] + n.tolist()[i])
	return np.matrix(new)

def matrixConcatList(l):
	m = l[0]
	for i in range(1, len(l)):
		m = matrixConcat(m, l[i])
	return m

def matrixMultList(l):
	m = np.identity(l[0].shape[0])
	for n in l:
		m = n * m
	return np.matrix(m)

def matrixSplit(m):
	s = m.shape[0]
	num = m.shape[1] / s
	m_list = []
	for i in range(num):
		temp_list = []
		for col in range(s):
			temp_list2 = []
			for row in range(s):
				temp_list2.append(m[row].tolist()[0][col + (s * i)])
			temp_list.append(temp_list2)
		m_list.append(np.matrix(temp_list))
	return m_list
	


# There are two ways to give inputs.
# As a file:
	# On separate lines: goal position, length of each link (in order from the root link)
	# In the file:
		# x y z
		# len1
		# len2
		# ...
# As arg inputs:
	# x y z len1 len2 ...

args = sys.argv
lengths = []
goal = []

# Input using a file.
if (len(args) == 2):
	filename = args[1]
	input_file = open(filename, "r")
	goal_list = float(input_file.readline())
	for line in input_file:
		lengths.append(float(line))
# Input using args.
else:
	goal_list = [float(args[1]), float(args[2]), float(args[3])]
	for i in range(4, len(args)):
		lengths.append(float(args[i]))


goal = np.matrix(goal_list).getT()

num_links = len(lengths)
orig_arm = Arm(np.array([(0.0, 0.0, 0.0)] * num_links), lengths)

#begin render code
colors = [[1, 0, 0], [1, 1, 0], [0, 1, 0], [0, 0, 1], [1, 0, 1], [0, 1, 1], [0.5, 0, 1.0]]
goal_points = []
if MOTION:
	theta = 0
	step = STEPS / 360.0
	goal_points = []
pygame.init()
display = (800,800)
pygame.display.set_mode(display, DOUBLEBUF|OPENGL)
glClearColor(0.0, 0.0, 0.0, 1.0)
glShadeModel(GL_FLAT)
glEnable(GL_DEPTH_TEST)
glEnable(GL_BLEND)
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)
glMatrixMode(GL_PROJECTION)
gluPerspective(40.0, 1.0, 1.0, 40.0)
gluLookAt(4, 7, 10, 0, 0, 0, 0, 1, 0)

new_arm = Arm(orig_arm.i_kinematics(0.01, goal), lengths)
lines = new_arm.get_lines()

#main loop
while True:
	#allow exiting loop
	for event in pygame.event.get():
		if event.type == pygame.QUIT:
			pygame.quit()
			quit()
	if MOTION:
		# move point along circle
		x = CIRCLE_CENTER[0] + (RADIUS * math.cos(math.radians(theta)) * VEC_A[0]) + (RADIUS * math.sin(math.radians(theta)) * VEC_B[0])
		y = CIRCLE_CENTER[1] + (RADIUS * math.cos(math.radians(theta)) * VEC_A[1]) + (RADIUS * math.sin(math.radians(theta)) * VEC_B[1])
		z = CIRCLE_CENTER[2] + (RADIUS * math.cos(math.radians(theta)) * VEC_A[2]) + (RADIUS * math.sin(math.radians(theta)) * VEC_B[2])
		goal_list = [x,y,z]
		goal = np.matrix(goal_list).getT()
		goal_points.append(goal_list)

		#calc new arm lines
		orig_arm = new_arm
		new_arm = Arm(orig_arm.i_kinematics(0.01, goal), lengths)
		lines = new_arm.get_lines()

		#adjust theta in circle
		theta += step
		if theta >= 360:
			theta = 0
			#goal_points = []

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT)
	glPushMatrix()
	glDisable(GL_LIGHTING)

	#draw baseplate thing
	glLineWidth(1.0)
	glColor3f(0.25, 0.25, 0.25)
	glBegin(GL_LINES)
	glVertex3f(0, 0, -20)
	glVertex3f(0, 0, 20)
	glEnd()
	glBegin(GL_LINES)
	glVertex3f(-20, 0, 0)
	glVertex3f(20, 0, 0)
	glEnd()
	for i in range(25):
		glBegin(GL_LINES)
		glVertex3f(-20, 0, i)
		glVertex3f(20, 0, i)
		glEnd()
		glBegin(GL_LINES)
		glVertex3f(-20, 0, -i)
		glVertex3f(20, 0, -i)
		glEnd()
		glBegin(GL_LINES)
		glVertex3f(-i, 0, -20)
		glVertex3f(-i, 0, 20)
		glEnd()
		glBegin(GL_LINES)
		glVertex3f(i, 0, -20)
		glVertex3f(i, 0, 20)
		glEnd()
	

	glColor3f(1, 1, 0)
	#draw sphere at origin
	glutSolidSphere(0.05, 100, 100)
	glColor3f(1, 1, 1)

	#actually draw lines
	glLineWidth(3.0)
	for i in range(len(lines)):
		glColor3fv(colors[i])
		glBegin(GL_LINES)
		glVertex3fv(lines[i][0])
		glVertex3fv(lines[i][1])
		glEnd()

	#draw circle
	glColor3f(1,1,1)
	if MOTION:
		if SHADE_CIRCLE:
			if len(goal_points) > 1:
				glColor4f(1, 1, 1, 0.3)
				glBegin(GL_TRIANGLES)
				for i in range(1, len(goal_points)):
					glVertex3fv(goal_points[i])
					glVertex3fv(goal_points[i-1])
					glVertex3fv(CIRCLE_CENTER)
				glEnd()
			glColor3f(1,1,1)
			glBegin(GL_LINE_STRIP)
			for point in goal_points:
				glVertex3fv(point)
			glEnd()
		else:
			glBegin(GL_LINE_STRIP)
			for point in goal_points:
				glVertex3fv(point)
			glEnd()
	glPopMatrix()

	# draw sphere at current goal
	glColor3f(1,1,1)
	glPushMatrix()
	glTranslatef(goal_list[0], goal_list[1], goal_list[2])
	glutSolidSphere(0.1, 100, 100)
	glPopMatrix()
	pygame.display.flip()
	pygame.time.wait(10)


	## do the calculations, get line segment vertices



# print(new_arm.angles)
# print(goal_list)
# print("segments: " + str(new_arm.get_lines()))
# render([([0, 0, 0], [1, 1, 1])], [0, 0, 0])
#render(new_arm.get_lines(), goal_list, orig_arm, new_arm, lengths)


# Resources:
# https://studywolf.wordpress.com/2013/04/11/inverse-kinematics-of-3-link-arm-with-constrained-minimization-in-python/
# http://www.quantstart.com/articles/Jacobi-Method-in-Python-and-NumPy


