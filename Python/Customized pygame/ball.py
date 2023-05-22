#Ananiya Deneke
#Ball class
#5/16/2023
#some code taken from lab tutorial

from drawable import Drawable
import pygame

class Ball(Drawable):

    def __init__(self, x=0, y=0, radius=10, color=(0, 0, 0)):
        super().__init__(x, y)
        self.__color = color
        self.__radius = radius
        self.__xSpeed = 1
        self.__ySpeed = 1
    
    def move(self):    
        currentX, currentY = self.getLoc()
        newX = currentX + self.__xSpeed
        newY = currentY + self.__ySpeed
        self.setLoc((newX, newY))
        surface = pygame.display.get_surface()
        width, height = surface.get_size() 
        
        if newX <= self.__radius or newX + self.__radius >= width:
            self.__xSpeed *= -1
        if newY <= self.__radius or newY + self.__radius >= height: 
            self.__ySpeed *= -1 
    
    def getSpeed(self):
        return (self.__xSpeed, self.__ySpeed)
    
    def setXSpeed (self, newXSpeed):
        self.__xSpeed += newXSpeed

    def setYSpeed (self, newYSpeed):
        self.__ySpeed += newYSpeed        
    
    def draw(self, surface):
        if self.isVisible():
            pygame.draw.circle(surface, self.__color, self.getLoc(), self.__radius)

    def get_rect(self):
        x, y = self.getLoc()
        diameter = self.__radius * 2
        rect = pygame.Rect(x - self.__radius, y - self.__radius, diameter, diameter)
        return rect