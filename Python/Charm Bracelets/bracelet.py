#Ananiya Deneke 6/2/2023
#bracelet.py
#linked list implementation of bracelets with charms
from linked_list import LinkedList
from charm import Charm

class Bracelet(LinkedList):
    def __init__(self, baseValue):
        super().__init__()
        self.__baseValue = baseValue

    def append(self, data):
        if self.isClosed():
            self.open()
            if isinstance(data, Charm):
                super().append(data)
            self.close()
        else:
            if isinstance(data, Charm):
                super().append(data)

    def appraise(self):
        if self.isEmpty():
            return
        curr = self.getHead()
        charmTotal = 0
        while curr:
            charmTotal += curr.getData().getMarketValue()
            if curr.getNext() == self.getHead():
                break
            curr = curr.getNext()
        return round(self.__baseValue + charmTotal, 2)
    
    def close(self):
        if self.isClosed():
            return
        if self.isEmpty():
            return
        curr = self.getHead()
        while curr.getNext():
            if curr.getNext() == self.getHead():
                break
            curr = curr.getNext()
        curr.setNext(self.getHead())

    def open(self):
        curr = self.getHead()
        while curr:
            curr = curr.getNext()
            if (curr.getNext() == self.getHead()):
                curr.setNext(None)
                break

    def isClosed(self):
        if self.isEmpty():
            return False
        curr = self.getHead()
        while curr:
            if curr.getNext() is None:
                return False
            curr = curr.getNext()
            if curr == self.getHead():
                return True
    
    def isOpen(self):
        return not self.isClosed()
    
    def remove(self, item):
        current =  self.getHead()
        previous = None
        found = False
        
        # first find item in the list
        while not found and current != None :
            if current.getData() == item:
                found = True
            else:
                previous = current
                current = current.getNext()
        if current == None:  #list was empty or item was not in the list
            return False
        elif previous == None or previous.getNext() == self.getHead():
            if self.isClosed():
                self.open()                  # item was in the fist node
                self._LinkedList__head = current.getNext()
                self.close()
            else:
                self._LinkedList__head = current.getNext()
            return True
        else:  # item was somewhere after the first node
            previous.setNext(current.getNext())
            return True

    def search(self, item):
        current = self.getHead()
        while current:
            if current.getData() == item:
                return True
            current = current.getNext()
            if current.getNext() == self.getHead():
                if current.getData() == item:
                    return True
                break 
        return False
    
    def __len__(self):    
        if self.getHead() == None:  # if list is empty return 0
            return 0
        current = self.getHead()   #list is not empty and has at least 1 Node
        counter = 1
        while current.getNext() != None and current.getNext() != self.getHead() : # check if theres's another item after the current node
            counter += 1
            current = current.getNext()
        return counter
    
    def __str__(self):    
        mystr = ''
        current = self.getHead() 
        while current:
            mystr += str(current.getData()) + ' --> '
            if current.getNext() == self.getHead():
                break
            current = current.getNext()
        return mystr