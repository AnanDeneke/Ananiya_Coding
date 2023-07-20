#Ananiya Deneke 6/2/2023
#Travel Charm class
#inherits from Charm
from charm import Charm

class TravelCharm(Charm):
    def __init__(self, name, description, retailPrice, condition, location):
        super().__init__(name, description, retailPrice, condition)
        self.__location = location

    def getMarketValue(self):
        val = 0     #initial value
        if self.getCondition().value < 5:   #if condition is less than acceptable it has no value
            val = 0
        else:
            val = self.getRetailPrice()
        return val

    def getLocation(self):
        return self.__location
    
    def setLocation(self,loc):
        self.__location = loc

    def __str__(self):
        return self.__location + ' charm'
   
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
           self.getMarketValue() == other.getMarketValue() and self.getCondition() == other.getCondition()\
           and self.getLocation() == other.getLocation():
            return True
        else:
            return False
        
    def __ne__(self, other):
        return not self.__eq__(other)
    
    




# Write code below that tests your new class
if __name__ == "__main__":
    try:
        charm1 = TravelCharm('Anan','Travel Charm to commemorate a location',\
                6.00, TravelCharm.Condition.ACCEPTABLE, "Ethiopia")
        charm2 = TravelCharm('Anan','Travel Charm to commemorate a location',\
                10.00, TravelCharm.Condition.ACCEPTABLE, "Ethiopia")
    except:
        print('Cannot initialize charms')
    print('test for charms with different retail price and overloaded operators')
    print(charm1)
    print(charm2)
    print(charm1 == charm2)
    print(charm1 != charm2)
    print(charm1 >= charm2)
    print(charm1 <= charm2)
    print(charm1 > charm2)
    print(charm1 < charm2)
    print('test to see if charm has 0 market value if it less than acceptable')
    charm1.setCondition(TravelCharm.Condition.WORN)
    print(charm1.getMarketValue())
    print('test to see if changing and getting location works')
    charm1.setLocation('India')
    print(charm1.getLocation())
    print('check that market value equals retail price if condition is more than acceptable')
    print(charm2.getMarketValue())
    print('check if location affects equality of charms with everything else identical')
    charm3 = TravelCharm('Anan','Travel Charm to commemorate a location',\
                10.00, TravelCharm.Condition.ACCEPTABLE, "Canada")
    print(charm2 == charm3)    
    