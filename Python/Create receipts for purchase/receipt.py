# File Name:   receipts.py
# Purpose:     A basic class to represent receipt
# Date:        April 12, 2023
import item
class Receipt:
    # constructor
    def __init__(self, purchases = [], tax_rate = 0.07):
        self.__tax_rate = tax_rate
        self.__purchases = purchases
    # getters
    def receiptToString(self):
        totalPrice = 0
        totalTax = 0
        for item in self.__purchases:
            print('{:_<20}{:_>20.2f}'.format(item.itemToString(), item.getPrice()))
            totalPrice += item.getPrice()
            totalTax += item.getTax(self.__tax_rate)
        print('{:_<20}{:_>20.2f}'.format('Sub Total', totalPrice))
        print('{:_<20}{:_>20.2f}'.format('Tax', totalTax))
        print('{:_<20}{:_>20.2f}'.format('Total', totalPrice + totalTax))

    #add item to list
    def addItem(self, i):
        self.__purchases.append(i)
    