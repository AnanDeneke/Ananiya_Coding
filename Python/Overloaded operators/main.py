from money import Money
import inputroutines

money = Money()
while True:
    try:
        userInput = int(input('1. Deposit\n2. Withdraw\n3. See current balance\n4. Exit\nEnter your choice: '))
        if userInput < 1 or userInput > 4:
            print('Invalid choice. Try again.')
    except:
        print('Invalid choice. Try again.')
        continue
    if userInput == 1:
        print('Deposit')
        while True:
            try:
                print('Enter the dollar amount: ', end="")
                userDollar = inputroutines.intInRange(0,1000)
                print('Enter the cents amount: ',end = "")
                userCent = inputroutines.intInRange(0,99)
                break
            except:
                print('Invalid input')
        money = Money(userDollar, userCent) + money
        print('Transaction completed.')
    if userInput == 2:
        print('Withdraw')
        while True:
            try:
                print('Enter the dollar amount: ', end="")
                reqDollar = inputroutines.intInRange(0,1000)
                print('Enter the cents amount: ',end = "")
                reqCent = inputroutines.intInRange(0,99)
                break
            except:
                print('Invalid input')
        widBalance = Money(reqDollar, reqCent)
        if money >= widBalance:
            money = money - widBalance
            print('Transaction completed.')
        else:
            print('You do not have enough funds.')
    if userInput == 3:
        print("Your current balance is: " + str(money))
    if userInput == 4:
        print('Good-bye!')
        break
