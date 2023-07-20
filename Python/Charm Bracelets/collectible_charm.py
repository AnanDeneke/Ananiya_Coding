#Ananiya Deneke 6/2/2023
#Collectible Charm class
#inherits from Charm
from charm import Charm

class CollectibleCharm(Charm):
    def __init__(self, name, description, retailPrice, condition, serialNumber):
        super().__init__(name, description, retailPrice, condition)
        self.__serialNumber = serialNumber

    def getMarketValue(self):
        val = self.getRetailPrice() * (self.getCondition().value / 100)
        return round(val,2)
    
    def getSerialNumber(self):
        return self.__serialNumber

    def __str__(self):
        return self.getName() + ' ' + '[' + self.getSerialNumber() +']'
   
    def __lt__(self, other):
        if self.getMarketValue() < other.getMarketValue():
            return True
        else:
            return False 
        
    def __le__(self, other):
        if self.getMarketValue() <= other.getMarketValue():
            return True
        else:
            return False
    
    def __gt__(self, other):
        if self.getMarketValue() > other.getMarketValue():
            return True
        else:
            return False

    def __ge__(self, other):
        if self.getMarketValue() >= other.getMarketValue():
            return True
        else:
            return False

    def __eq__(self, other):
        if self.getName() == other.getName() and self.getDescription() == other.getDescription() and\
           self.getRetailPrice() == other.getRetailPrice() and self.getCondition() == other.getCondition()\
           and self.getSerialNumber() == other.getSerialNumber():
            return True
        else:
            return False
        
    def __ne__(self, other):
        return not self.__eq__(other)

    
    