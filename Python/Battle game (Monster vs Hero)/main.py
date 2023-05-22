# Lab 4 - CS 172
# Ananiya Deneke (ad3923)   Victoria Orlik(vao25)  4/26/2023
# main file


######TODO######    
from characters import Character, Monster, Hero
import random

# This function has two Characters fight
# it returns the winner or None on a tie
def monster_battle(h1, m1):
   
    print("Starting Battle Between")
    print(m1.getName() + ": " + m1.getDescription())
    print(h1.getName() + ": " + h1.getDescription())
    
    #Whose turn is it?
    attacker = None
    defender = None

    ######TODO######    
    #Select Randomly whether h1 or m1 is the initial attacker
    #the other is the initial defender
    choice = random.randint(0,1)
    if choice == 0:
        attacker = m1
        defender = h1
    else:
        attacker = h1
        defender = m1
        
    print(attacker.getName() + " goes first.")
    
    #Loop until someone is unconsious (health < 1) or choose to stop
    stop = False
    while( m1.getHealth() > 0 and h1.getHealth() > 0 and not stop ):
        
        #It will be nice for output to record the damage done
        #######before_health = defender.getHealth()            

        #Check if the attacker is a monster
        if(isinstance(attacker, Monster)):
            #check if defender is defending, if so print out info about the defense
            if(defender.isDefending()):
                print("Our hero is defending with", defender.getDefenseName(), "!")
            
            
            ######TODO######    
            #Have the attacker react.
            #Have the attacker attack.
            #Call the print_results function with the necessary info.
            attacker.react()
            attacker.attack(defender)
            print_results(attacker, defender,attacker.getWeaponName(), attacker.getWeaponDamage())


        else:
            # Ask the user for the next action: attack, defend, or stop.
            action = input('Pick one of these (a)ttack, (d)efend, or sto(p): ')
        
            ######TODO######    
            #Based on the input, either attack, defend, or end loop
            #If they chose to attack, have the attacker react, attack and then
            #call the print_results function with the necessary info.
            if action == 'a':
                print(attacker.react())
                attacker.attack(defender)
                print_results(attacker, defender,attacker.getWeaponName(), defender.getHealth()-attacker.getWeaponDamage())
            if action == 'd':
                attacker.defend()
            if action == 'p':
                break

        
        ######TODO######
        attacker, defender = defender, attacker
        

    ######TODO######    
    #Print out who won.
    #Return who won.
    if attacker.getHealth() > defender.getHealth():
        winner = attacker.getName()
    else:
        winner = defender.getName()
    print(f'Battle is over. let\'s see who has won...\n{winner} is victorious!')
    return winner
    
    
    
#This function prints the status updates
def print_results(attacker, defender, attack, hchange):
    res = attacker.getName() + " used " + attack
    res += " on " + defender.getName() + "\n"
    res += "The attack did " + str(hchange) + " damage."
    print(res)
    print(attacker.getName() + " at " + str(attacker.getHealth()))
    print(defender.getName() + " at " + str(defender.getHealth()))


#----------------------------------------------------
if __name__=="__main__":
    #Every battle should be different, so we need to
    #start the random number generator somewhere "random".
    #With no input Python will set the seed
    random.seed(0)
   
    ######TODO######    
    #Get Monster's name, description, maxHealth, weaponName, weaponDamage, and motivation from the user here.
    #Instantiate a Monster using that info. Note that weaponDamage should be a floating point number.
    name = (input("Enter monster's name: "))
    description = (input("Enter monster's description: "))
    maxHealth = (int(input("Enter a number for monster's health: ")))
    weaponName = (input("Enter monster's weapon name: "))
    weaponDamage = (float(input("Enter monster's weapon damage (as a number): ")))
    motivation = (input("Enter monster's motivation: "))
    myMonster = Monster(name, description, maxHealth, weaponName, weaponDamage, motivation)  #this should be an instance of your Monster class
    
    ######TODO######    
    #Get the Hero's name,description, maxHealth, weaponName, weaponDamage, defenseName from the user here.
    #Instantiate a Hero using that info. Note that weaponDamage should be a floating point number.
    name = (input("Enter hero's name: "))
    description = (input("Enter hero's description: "))
    maxHealth = (int(input("Enter a number for hero's health: ")))
    weaponName = (input("Enter hero's weapon name: "))
    weaponDamage = (float(input("Enter hero's weapon damage (as a number): ")))
    defenseName = (input("Enter the hero's defense name: "))

    myHero = Hero(name, description, maxHealth, weaponName, weaponDamage, defenseName)  #this should be an instance of your Hero class
    
    winner = monster_battle(myHero, myMonster)
