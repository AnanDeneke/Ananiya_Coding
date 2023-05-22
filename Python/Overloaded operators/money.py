#File Name:   money.py
#Money class definition and operator overloading for the class
#Ananiya Deneke 4/54/2023

class Money:
    def __init__(self, dollars = 0, cents = 0):
        self.__dollars = dollars
        self.__cents = cents
    
    #Get the cents value
    def getCents(self):
        return self.__cents
    
    #Get the Dollar value
    def getDollars(self):
        return self.__dollars
    
    def addDollars(self, dollars):
        self.__dollars += dollars
    
    def addCents(self, cents):
        self.__cents += cents
        if self.__cents >= 100:
            self.__cents = self.__cents - 100         #lower by 100 and increase dollar value by 1  
            self.addDollars(1)

#Overloading methods
    def __add__(self, other):
        dollarsSum = self.getDollars() + other.getDollars()
        centsSum = self.getCents() + other.getCents()
        if centsSum >= 100:
            centsSum = centsSum - 100         #lower by 100 and increase dollar value by 1  
            dollarsSum += 1
        return Money(dollarsSum, centsSum)
    
    def __sub__(self, other):
        if self.getDollars() > other.getDollars():
            dollarDiff =  self.getDollars() - other.getDollars()
            if self.getCents() >= other.getCents():
                centDiff = self.getCents() - other.getCents()
            else:
                centDiff = 100 + self.getCents() - other.getCents() #add 100 because we're removing 1 for the dollar
                dollarDiff -= 1
        else:
            dollarDiff =  other.getDollars() - self.getDollars()
            if other.getCents() >= self.getCents():
                centDiff = other.getCents() - self.getCents()
            else:
                centDiff = 100 + other.getCents() - self.getCents() #add 100 because we're removing 1 for the dollar
                dollarDiff -= 1
        return Money(dollarDiff,centDiff)

    def __mul__(self, n):
        centsMul = self.getCents() * n
        dollarsMul = (self.getDollars() * n) + centsMul // len(str(centsMul))
        if centsMul >= 100:
            centsMul = int(str(centsMul)[-2:])
        return Money(dollarsMul, centsMul)
    
    def __str__(self):
            return f'${self.getDollars()}.{self.getCents():02}'

    def __eq__(self,other):
        return self.getDollars() == other.getDollars() and self.getCents() == other.getCents()

    def __ne__(self,other):
        return self.getDollars() != other.getDollars() or self.getCents() != other.getCents()
    
    def __lt__(self, other):
        if self.getDollars() < other.getDollars():
            return True
        elif self.getDollars() == other.getDollars() and self.getCents() < other.getCents():
            return True
        else:
            return False
    
    def __gt__(self, other):
        if self.getDollars() > other.getDollars():
            return True
        elif self.getDollars() == other.getDollars() and self.getCents() > other.getCents():
            return True
        else:
            return False
    
    def __le__(self, other):
        if self.getDollars() < other.getDollars():
            return True
        elif self.getDollars() == other.getDollars() and self.getCents() <= other.getCents():
            return True
        else:
            return False
    
    def __ge__(self, other):
        if self.getDollars() > other.getDollars():
            return True
        elif self.getDollars() == other.getDollars() and self.getCents() >= other.getCents():
            return True
        else:
            return False
    def __getitem__(self, index):
        if index == 0:
            return self.getDollars()
        elif index == 1:
            return self.getCents()
        else:
            raise IndexError("Invalid index.")

