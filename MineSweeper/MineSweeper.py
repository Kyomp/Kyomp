import pyautogui
import time
import random

from pyscreeze import Box
# pyautogui.FAILSAFE=False

def click(a):
    pyautogui.click(a[0]+7,a[1]+7)
def clickList(a):
    for i in a:
        click(i)
def rightClick(a):
    pyautogui.rightClick(a[0]+7,a[1]+7)
def rightClickList(a):
    for i in a:
        rightClick(i)
time.sleep(1)
possible = list(pyautogui.locateAllOnScreen('Untouched.png'))
possibleSpace = set(possible)
safe = set()
safe.add(possible[random.randint(0,len(possible)-1)])
flagged = set(pyautogui.locateAllOnScreen('Flagged.png'))
definite = set()
def getAround(a):
    returnVal = set()
    returnVal.clear()
    for i in range(-1,2):
        for j in range(-1,2):
            returnVal.add(Box(a[0]+i*16,a[1]+j*16,a[2],a[3]))
    returnVal.difference_update({a})
    return returnVal
while len(safe)>0 or len(definite)>0:
    clickList(safe)
    safe.clear()
    rightClickList(definite)
    definite.clear()
    pyautogui.moveTo(10,10)
    possibleSpace = set(pyautogui.locateAllOnScreen('Untouched.png'))
    possibleDangers = list()
    for i in range(1,9):
        Is = set(pyautogui.locateAllOnScreen(str(i)+".png"))
        for num in Is:
            around = getAround(num)
            UntouchedZone = around.intersection(possibleSpace)
            DangerZone = around.intersection(flagged)
            if len(DangerZone) == i:
                safe.update(UntouchedZone.difference(DangerZone))
            elif len(UntouchedZone)+len(DangerZone)==i:
                definite.update(UntouchedZone)
            else:
                possibleDangers.append([UntouchedZone,i-len(DangerZone)])
            flagged.update(definite)
            possibleSpace.difference_update(safe)
    if len(safe)==0 and len(definite)==0:
        for i in possibleDangers:
            for j in possibleDangers:
                if i[0].issuperset(j[0]):
                    if i[1]==j[1]:
                        safe.update(i[0].difference(j[0]))
                    elif i[1]-j[1]==len(i[0].difference(j[0])):
                        definite.update(i[0].difference(j[0]))
                if j[0].issuperset(i[0]):
                    if i[1] == j[1]:
                        safe.update(i[0].difference(j[0]))
                    elif j[1] - i[1] == len(j[0].difference(i[0])):
                        definite.update(j[0].difference(i[0]))
                flagged.update(definite)
