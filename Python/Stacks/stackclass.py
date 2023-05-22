# template lab 5
class Stack:
	#Create a New Empty Stack
	def __init__(self):
		self.__S = []
		
	#Display the Stack
	def __str__(self):
		return str(self.__S)
		
	#Add a new element to top of stack
	def push(self, x):
		self.__S.append(x)
		
	#Remove the top element from stack
	def pop(self):
		return self.__S.pop()
		
	#See what element is on top of stack
	#Leaves stack unchanged
	def top(self):
		return self.__S[-1]
	
	#Check to see if the stack is empty
	def isEmpty(self):
		return len(self.__S) == 0