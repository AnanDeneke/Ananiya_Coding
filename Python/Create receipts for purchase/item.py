# File Name:   item.py
# Purpose:     A basic class to represent items
# Date:        April 12, 2023

class Item:
    # constructor
    def __init__(self, name, price, taxable):
        self.__name = name
        self.__price = price
        self.__taxable = taxable
    # getters
    def itemToString(self):
        return self.__name
    
    def getPrice(self):
        return self.__price
    
    def getTax(self, taxRate):
        if self.__taxable:
            return self.__price * taxRate
        else:
            return 0
 

    