#Ananiya Deneke
#Main file
#5/16/2023
#some code taken from lab tutorial

import pygame
from ball import Ball
from paddle import Paddle
from text import Text

def pause_game(surface, message):
    paused = True
    pauseText = Text(message, 50, 300)
    while paused:
        for event in pygame.event.get():
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_SPACE:
                    paused = False
                elif event.key == pygame.K_ESCAPE:
                    pygame.quit()
                    quit()
            elif event.type == pygame.QUIT:
                pygame.quit()
                quit()
        pauseText.setMessage(message)
        pauseText.draw(surface)
        pygame.display.flip()

def restart_game():
    game_code()

def game_code():
    pygame.init()
    surface = pygame.display.set_mode((800, 600))
    DREXEL_BLUE = (7, 41, 77)

    myBall = Ball(400, 300, 25, DREXEL_BLUE)
    myPaddle = Paddle(200, 25, DREXEL_BLUE)
    myScoreBoard = Text("Score: 0", 10, 10)
    myLevel = Text("Level: 1", 700, 10)

    fpsClock = pygame.time.Clock()

    clockSpeed = 60
    clockCounter = 1
    numHits = 0

    green = (0, 255, 0)
    yellow = (255, 255, 0)
    red = (255, 0, 0)
    white = (255, 255, 255)
    lightBlue = (0, 255, 255)

    running = True

    while running:
        # Different clock speeds for different levels
        if clockSpeed == 60:
            surface.fill(lightBlue)
            myScoreBoard.setMessage("Score: " + str(numHits))
            myLevel.setMessage("Level: 1")

        if clockSpeed == 120:
            surface.fill(yellow)
            myLevel.setMessage("Level: 2")

        if clockSpeed == 240:
            surface.fill(red)
            myLevel.setMessage("Level: 3")

        if clockSpeed > 240:
            surface.fill(green)
            pause_game(surface, 'Game Won!!! - press Space to play again')
            restart_game()
        
        myBall.draw(surface)
        myPaddle.draw(surface)
        myScoreBoard.draw(surface)
        myLevel.draw(surface)

        if myBall.intersects(myPaddle):
            myBall.setYSpeed(myBall.getSpeed()[0] * - 2)
            numHits += 1
            myScoreBoard.setMessage("Score: " + str(numHits))

        if numHits >= clockCounter * 1:
            clockSpeed *= 2
            clockCounter += 1
        # Player loses when paddle fails to catch ball at the top surface
        if myBall.getLoc()[1] > (600 - 20 - 25): # account for ball radius LHS and paddle height and 20 pixel offset for RHS
            pause_game(surface, 'Level Failed - press Space to try again')
            restart_game()
        myBall.move()

        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False
            elif event.type == pygame.KEYDOWN:
                if event.key == pygame.K_ESCAPE:
                    pause_game(surface, "GAME PAUSED - press Space to Continue or Esc again to Quit")

        pygame.display.update()
        fpsClock.tick(clockSpeed)
    pygame.quit()
    exit()


game_code()
