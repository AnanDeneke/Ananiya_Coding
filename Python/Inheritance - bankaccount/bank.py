#Homework 3 - CS172
#VAnaniya Deneke (ad3923) 4/29/2023

# This class defines a Bank account
from abc import ABC, abstractmethod

class BankAccount(ABC):
    __nextAccountNumber = 1000
    def __init__(self, owner, balance = 0):
        self.__owner = owner
        self.__accountNumber = BankAccount.__nextAccountNumber
        self.__balance = balance
        BankAccount.__nextAccountNumber += 1

    def getOwner(self):
        return self.__owner
    
    def getBalance(self):
        return self.__balance  
  
    def getAccountNumber(self):
        return self.__accountNumber
    
    def deposit(self, num):
        self.__balance += num

    def withdraw(self, num):
        self.__balance -= num

    def __eq__(self, other):
        if isinstance(other, BankAccount):
            if (self.__accountNumber == other.__accountNumber and self.__owner == other.__owner and self.__balance == other.__balance):
                return True
        return False
        
    def __str__(self):
        myStr = f'Account Number: {self.__accountNumber}\n'
        myStr += f'Account Owner: {self.__owner}\n'
        myStr += f'Account Balance: ${self.__balance:.2f}'
        return myStr
    
    def getNextAccountNumber():
        return BankAccount.__nextAccountNumber
    
    @abstractmethod
    def endOfMonth(self):
        pass

class Savings(BankAccount):

    def __init__(self, owner, balance = 0, interestRate = 3.25 ):
        super().__init__(owner, balance)
        self.__interestRate = interestRate
    
    def getInterestRate(self):
        return self.__interestRate

    def setInterestRate(self, interestRate):
        self.__interestRate = interestRate

    def __eq__(self, other):
        if isinstance(other, Savings):
            if (super().__eq__(other) and self.__interestRate == other.__interestRate):
                return True
        return False
        
    def __str__(self):
        myStr = super().__str__()
        myStr += f'\nAnnual Interest Rate: {self.__interestRate:.2f}%'
        return myStr
       
    def endOfMonth(self):
        interest = self.getBalance() * (self.__interestRate / 1200)
        self.deposit(interest)

class Checking(BankAccount):
    def __init__(self, owner,  balance = 0, transactions = 0 ):
        super().__init__(owner, balance)
        self.__transactions = transactions
    
    def getTransactionsNum(self):
        return self.__transactions

    def deposit(self, num):
        super().deposit(num)
        self.__transactions += 1

    def withdraw(self, num):
        super().withdraw(num)
        self.__transactions += 1

    def __eq__(self, other):
        if isinstance(other, Checking):
            if (super().__eq__(other) and self.__transactions == other.__transactions):
                return True
        return False
        
    def __str__(self):
        myStr = super().__str__()
        myStr += f'\nTransactions this month: {self.__transactions}'
        return myStr
       
    def endOfMonth(self):
        if self.__transactions > 7:
            self.withdraw(5.0)
        self.__transactions = 0

