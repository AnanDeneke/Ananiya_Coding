#Homework 3 - CS172
#VAnaniya Deneke (ad3923) 4/29/2023

# Main file
from bank import BankAccount, Savings, Checking

bankAccounts = []

def checkFloat(x):
    while True:
        try:
            value = float(input(x))
            if value >= 0.0:
                return value
            else:
                print("Enter a greater than or equal to zero: ")
        except:
            print("Invalid input: an float value was expected. Try again: ")

def checkInt(y):
    while True:
        try:
            value = int(input(y))
            return value
        except:
            print("Invalid input: an integer value was expected. Try again: ")

def savingsAccount():
    owner = input("Enter owner's name: ")
    print("Enter initial balance: ", end ='')
    balance = checkFloat("")
    savingsAccount = Savings(owner, balance)
    bankAccounts.append(savingsAccount)
    print("Account added")

def checkingAccount():
    owner = input("Enter owner's name: ")
    print("Enter initial balance: ", end ='')
    balance = checkFloat("")
    checkingAccount = Checking(owner, balance)
    bankAccounts.append(checkingAccount)
    print("Account added")

def deposit():
    print("Enter account number: ",end = '')
    # accountNumber = checkInt("Enter account number: ")
    accountNumber = checkInt('')
    account = findAccount(accountNumber)
    if account:
        print("Enter amount to deposit: ")
        amount = checkFloat("")
        account.deposit(amount)
    else:
        print("That account number does not exist")

def withdraw():
    print("Enter account number: ")
    accountNumber = checkInt('')
    account = findAccount(accountNumber)
    if account:
        print("Enter amount to withdraw: ", end ='')
        amount = checkFloat("")
        if account.getBalance() >= amount:
            account.withdraw(amount)
        else:
            print("You do not have enough funds")
    else:
        print("That account number does not exist")

def endOfMonth():
    for account in bankAccounts:
        account.endOfMonth()
    print("End of month operations have been performed")

def displaySavingsAccounts():
    for account in bankAccounts:
        if isinstance(account, Savings):
            print(account)

def displayCheckingAccounts():
    for account in bankAccounts:
        if isinstance(account, Checking):
            print(account)

def displayAllAccounts():
    for account in bankAccounts:
        print(account)

def findAccount(accountNumber):
    for account in bankAccounts:
        if account.getAccountNumber() == accountNumber:
            return account
    return False

while True:
    try:
        print("1. Create Savings Account")
        print("2. Create Checking Account")
        print("3. Deposit")
        print("4. Withdraw")
        print("5. Perform End of Month Operations")
        print("6. Display Savings Accounts")
        print("7. Display Checking Accounts")
        print("8. Display All Accounts")
        print("9. Exit")
        choice = int(input("Enter your choice: "))
    except:
        print("Invalid choice. Try again.")
    else:
        if choice == 1:
            print('Savings Account')
            savingsAccount()
        elif choice == 2:
            print('Checking Account')
            checkingAccount()
        elif choice == 3:
            print('Deposit')
            deposit()
        elif choice == 4:
            print('Withdraw')
            withdraw()
        elif choice == 5:
            endOfMonth()
        elif choice == 6:
            displaySavingsAccounts()
        elif choice == 7:
            displayCheckingAccounts()
        elif choice == 8:
            displayAllAccounts()
        elif choice == 9:
            print("Good-bye!")
            break
        else:
            print("Invalid choice. Try again.")
