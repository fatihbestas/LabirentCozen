'''
Labirentin görüntüsünü düzenledim
Kapanan yolları ekledim
Ziyaret edilen yolları ekledim
'''
import pygame
pygame.init()

width_win = 450
height_win = 450
win = pygame.display.set_mode((width_win, height_win))

pygame.display.set_caption("Simülasyon V1.0")

x = 90
y = 90
width_shape = 20
height_shape = 20
robot_loc_x = x + 4
robot_loc_y = y + 4
vel = 22

maze_runner = open('maze runner.txt','r')
maze_file = open('maze.txt','r')
move_file = open('Hamle Sayisi.txt','r')
maze = maze_file.readlines()
moves = move_file.readlines()
m=0
z=0
t=3 # buraya robotun hareket etmediği ama hamle kabul edilen hamlelerin sayısını yazıcaz
z += t
run = True
while run:
    pygame.time.delay(100)

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            run = False

    #keys = pygame.key.get_pressed()
    if z<int(moves[0]): # Hamle sayısı kadar işlem yapmalıyız burda yoksa dosyanın sonunda loc1 değişkeni hataya sebep oluyor
        loc1 = maze_runner.readline()
    
        if loc1[0] == 'c':
            robot_loc_x -= vel
        elif loc1[0] == 'x':
            robot_loc_x += vel
        elif loc1[0] == 'y':
            robot_loc_y -= vel
        elif loc1[0] == 'v':
            robot_loc_y += vel
        z += 1
            
    win.fill((0,0,0))
    for i in range(25):
        for a in range(25):
            if i%2 == 0 and a%2 == 0: # Noktalar
                if maze[i+25*m][a] == 'o':
                    pygame.draw.rect(win, (255, 255, 255), (x, y, 2, 2))
                    x += 2
                elif maze[i+25*m][a] == ' ':
                    pygame.draw.rect(win, (255, 255, 255), (x, y, 2, 2))
                    x += 2
            if i%2 == 0 and a%2 == 1: # Yatay geçitler ve duvarlar 
                if maze[i+25*m][a] == 'o':
                    pygame.draw.rect(win, (255, 255, 255), (x, y, 20, 2))
                    x += 20
                elif maze[i+25*m][a] == ' ':
                    pygame.draw.rect(win, (0, 0, 0), (x, y, 20, 2))
                    x += 20
            if i%2 == 1 and a%2 == 0: # Dikey geçitler ve duvarlar
                if maze[i+25*m][a] == 'o':
                    pygame.draw.rect(win, (255, 255, 255), (x, y, 2, 20))
                    x += 2
                elif maze[i+25*m][a] == ' ':
                    pygame.draw.rect(win, (0, 0, 0), (x, y, 2, 20))
                    x += 2
            if i%2 == 1 and a%2 == 1: # Kareler
                if maze[i+25*m][a] == ' ':
                    pygame.draw.rect(win, (0, 0, 0), (x, y, 20, 20))
                    x += 20
                elif maze[i+25*m][a] == '.': # Ziyaret edilen kareler
                    pygame.draw.rect(win, (0, 0, 0), (x, y, 20, 20))
                    pygame.draw.rect(win, (0,0, 255), (x+9, y+9, 2, 2))
                    x += 20
                elif maze[i+25*m][a] == 'x':
                    pygame.draw.rect(win, (255, 255, 0), (x+6, y+6, 8, 8))
                    x += 20
                
        x = 90
        if i%2 == 0:
            y += 2
        else:
            y += 20
    m += 1
    if m > int(moves[0])-1: 
        m = int(moves[0])-1
    x = 90
    y = 90
    pygame.draw.rect(win, (255, 0, 0), (robot_loc_x, robot_loc_y, 16, 16))
    pygame.display.update()
    
pygame.quit()
