#Ananiya Deneke
#Drawable class
#5/16/2023
#some code taken from lab tutorial
import pygame
from abc import ABC, abstractmethod

class Drawable(ABC):
	def __init__(self, x = 0, y = 0):
		self.__visible = True
		self.__x = x
		self.__y = y
		
	def getLoc(self):
		return (self.__x, self.__y)
		
	def setLoc(self, p):
		self.__x = p[0]
		self.__y = p[1]

	def isVisible(self):
		return self.__visible
	
	def setVisible(self, visible):
		if visible == True:
			self.__visible = True
		else:
			self.__visible = False
	def intersects(self, other):
		rect1 = self.get_rect()
		rect2 = other.get_rect()
		if (rect1.x < rect2.x + rect2.width) and (rect1.x + rect1.width > rect2.x) and (rect1.y < rect2.y + rect2.height) and (rect1.height + rect1.y > rect2.y):
			return True
		return False

	@abstractmethod
	def draw(self, surface):
		pass

	@abstractmethod
	def get_rect(self):
		pass

