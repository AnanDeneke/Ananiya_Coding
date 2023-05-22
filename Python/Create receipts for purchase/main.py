# Ananiya Deneke(ad3923)   Victoria Orlik(vao25)  4/12/2023
from item import *
from receipt import *
import datetime

# main script
if __name__ == "__main__":
    
    print('Welcome to Receipt Creator')
    itemName = input('Enter Item name: ')
    itemPrice = float(input('Enter Item price: '))
    isTaxable = input('Is the item taxable (yes/no): ')
    if isTaxable == 'yes':
        isTaxable = True
    else:
        isTaxable = False
    
    i = Item(itemName, itemPrice, isTaxable) #instantiate Item class    
    r = Receipt([])  #Instantiate receipt class
    r.addItem(i)
    
    addAnother = input('Add another item (yes/no): ')
    while addAnother == 'yes':
        itemName = input('Enter Item name: ')
        itemPrice = float(input('Enter Item price: '))
        isTaxable = input('Is the item taxable (yes/no): ')
        if isTaxable == 'yes':
            isTaxable = True
        else:
            isTaxable = False       
        r.addItem(Item(itemName, itemPrice, isTaxable))
        addAnother = input('Add another item (yes/no): ')            
    print('----- Receipt time -----')
    #print('{:->20}{:-<20}'.format('Receipt','time'))
    r.receiptToString()