list  = [" " for x in range(9)]#global variable
print(list)
print()
for x in range(0,9,3):
    print(x)
#funtion #1
def board():
    for x in range(0,9,3):
        a = "|{}|{}|{}|".format(list[x],list[x+1],list[x+2])
        print(a)
    print()
    
#funtion #2    
def player_move(icon):
    if icon == "x":
        number = 1
    elif icon == "o":
        number = 2
        
    print("Your turn player {}".format(number))
    choice=int(input("Enter your move (1-9): ").strip())
    print()
    
    if choice not in (range(1,10)):
        print("Invalid entry")
        return player_move(icon)
    if list[choice-1] == " ":
        list[choice-1] = icon        
    else:
        print("This place is already taken!")
        print()
    
    
#funtion #3
def is_victory(icon):
    if(list[0]==icon and list[1]==icon and list[2]==icon) or\
      (list[3]==icon and list[4]==icon and list[5]==icon) or\
      (list[6]==icon and list[7]==icon and list[8]==icon) or\
      (list[0]==icon and list[3]==icon and list[6]==icon) or\
      (list[1]==icon and list[4]==icon and list[7]==icon) or\
      (list[2]==icon and list[5]==icon and list[8]==icon) or\
      (list[0]==icon and list[4]==icon and list[8]==icon) or\
      (list[2]==icon and list[4]==icon and list[6]==icon):
        return True
    
    else:
       return False

#funtion #4    
def is_draw():
    if " " not in list:
        return True
    else:
        return False




    
#Kind of our main like in c      
print("This is our tic tac toe board!")
print()




while True:
    board()
    player_move("x")
    board()
    if is_victory("x"):
       print("Congratulations! X Wins!")
       break
    elif is_draw():
        print("Its a draw")
        break
    player_move("o")
    if is_victory("o"):
       board()
       print("Congratulations! O Wins!")
       break
    elif is_draw():
        print("Its a draw!")
        break
    
    
