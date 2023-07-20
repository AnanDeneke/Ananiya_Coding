#Ananiya Deneke 6/2/2023
#Friendship Charm class
#inherits from Charm
from charm import Charm

class FriendshipCharm(Charm):
    def __init__(self, name, description, retailPrice, condition, symbol):
        super().__init__(name, description, retailPrice, condition)
        self.__symbol = symbol

    def getMarketValue(self):
        val = self.getCondition().value / 100
        return round(val, 2)
    
    def getSymbol(self):
        return self.__symbol

    def __str__(self):
        return self.__symbol
   
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
        if self.getMarketValue() == other.getMarketValue():
            return True
        else:
            return False
        
    def __ne__(self, other):
        return not self.__eq__(other)
    
    