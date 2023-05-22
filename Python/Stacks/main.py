#Name: Ananiya Deneke (ad3923) Victoria Orlik(vao25)
#Purpose: main file for Lab 6, postfix calculator
#Date: 17 May 2023

from stackclass import Stack

def postfix(exp):
    myStack = Stack()
    expression_split = exp.split(" ")
    for x in expression_split:
        if x.isdigit() or (x[0] == '-' and x[1:].isdigit()): #check for negative numbers as well we use x[1:] so we don't get an index error
            myStack.push(x)
        if x == '+' or x == '-' or x == '/' or x == '*':
            right =  float(myStack.pop())
            left = float(myStack.pop())
            if x == "+":
                result = left + right
            elif x == "-":
                result = left - right
            elif x == "*":
                result = left * right
            elif x == "/":
                result = left / right
            myStack.push(result)
    return myStack.top()

if __name__ == "__main__":
    print('Welcome to Postfix Calculator')
    print('Enter exit to quit')

    while True:
        expression = input('Enter Expression')
        if expression == 'exit':
            break
        finalResult = postfix(expression)
        print(f'Result: {finalResult}')
