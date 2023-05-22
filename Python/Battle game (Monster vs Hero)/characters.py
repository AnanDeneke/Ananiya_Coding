# Lab 4 - CS 172
# Ananiya Deneke (ad3923)   Victoria Orlik(vao25)  4/26/2023
# This class defines a generic Character
# It includes attributes and many implemented methods, in addition to an abstract
# methods __str__ and react
from abc import ABC, abstractmethod

### DO NOT CHANGE ANYTHING BELOW IN THIS Character CLASS ####
class Character(ABC):
    def __init__(self, name, description, maxHealth, weaponName, weaponDamage):
        self.__name = name
        self.__health = maxHealth
        self.__description = description
        self.__weaponName = weaponName
        self.__weaponDamage = weaponDamage

    @abstractmethod
    def __str__(self):
        pass
    
    @abstractmethod
    def react(self):
        pass
    
    def getName(self):
        return self.__name
    
    def getDescription(self):
        return self.__description
    
    def getWeaponName(self):
        return self.__weaponName
    
    def getWeaponDamage(self):
        return self.__weaponDamage
    
    def attack(self, enemy):
        enemy.takeDamage(self.__weaponDamage)
    
    def takeDamage(self, amount):
        self.__health -= amount
    
    def getHealth(self):
        return self.__health
    
    
## TODO: Create a Monster class that inherits from the Character class.
class Monster(Character):
    def __init__(self, name, description, maxHealth , weaponName, weaponDamage, motivation):
        #TODO
        Character.__init__(self, name, description, maxHealth, weaponName, weaponDamage)
        self.__motivation = motivation
        
    def __str__(self):
        #TODO
        #Return a string in the form:
        #<name> is a <description>
        #Weapon: <weapon>
        #Current Health: <health>
        #Motivation: <motivation>
        myStr =  f'<{Character.getName(self)}> is a {Character.getDescription(self)}\n'
        myStr += f'Weapon: <{Character.getWeaponName(self)}\n'
        myStr += f'Current health: <{Character.getHealth(self)}\n'
        myStr += f'Motivation: <{self.__motivation}>'
        return myStr

    
    def react(self):
        #TODO
        #return a string in the form:
        #<name> laughs maniacally.
        return f'<{Character.getName(self)}> laughs maniacally.'
    
    def getMotivation(self):
        #TODO
        #Return the monster's motivation
        return self.__motivation
    
## TODO: Create a Hero class that inherits from the Character class.
class Hero(Character):
    def __init__(self, name, description, maxHealth, weaponName, weaponDamage, defenseName, defenseStatus = False):
        #TODO
        #Initialize parent class with provided information
        #Also creates attributes for defense name (string), and defending status (boolean)
        Character.__init__(self, name, description, maxHealth, weaponName, weaponDamage)
        self.__defenseName = defenseName
        self.__defenseStatus = defenseStatus
        
    def __str__(self):
        #TODO
        #Return a string in the form:  
        #Our hero <name> is a <description>
        #Weapon: <weapon>
        #Defense: <defenseName>
        #Current Health: <health>
        #Defense Status: <isDefending>
        myStr =  f'Our hero <{Character.getName(self)}> is a {Character.getDescription(self)}\n'
        myStr += f'Weapon: <{Character.getWeaponName(self)}\n'
        myStr += f'Defense: <{self.__defenseName}>\n'
        myStr += f'Current health: <{Character.getHealth(self)}\n'
        myStr += f'Defense Status: <{self.__defenseStatus}>'
        return myStr
        
    def react(self):
        #TODO
        #Return a string in the form:
        #<name> charges bravely.
        return f'<{Character.getName(self)}> charges bravely.'
        
    def getDefenseName(self):
        #TODO
        #Return the defense name   
        return self.__defenseName   
    def isDefending(self):
        #TODO
        #Return the defense status
        return self.__defenseStatus
    
    def defend(self):
        #TODO
        #Changes defense status to True
        self.__defenseStatus = True
        
    def takeDamage(self, amount):
        #TODO
        #Check defense status
        #If it is enabled (True), reduce the amount by 50%, and change the defense status to false.
        #Regardless, apply the final amount to the hero by calling its parent class' takeDamage method.
        if self.__defenseStatus == True:
            amount = amount - 0.5 * amount
            self.__defenseStatus = False
        Character.takeDamage(self,amount)
