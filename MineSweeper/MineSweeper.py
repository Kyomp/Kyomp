import pyautogui
import time
import random
from pyscreeze import Box


# pyautogui.FAILSAFE=False

def click(a):
    pyautogui.click(a[0] + 7, a[1] + 7)


def clickList(a):
    for k in a:
        click(k)


def rightClick(a):
    pyautogui.rightClick(a[0] + 7, a[1] + 7)


def rightClickList(a):
    for e in a:
        rightClick(e)


time.sleep(1)
possible = list(pyautogui.locateAllOnScreen('Untouched.png'))
possibleSpace = set(possible)
safe = set()
safe.add(possible[random.randint(0, len(possible) - 1)])
flagged = set(pyautogui.locateAllOnScreen('Flagged.png'))
definite = set()
noUse = set()


def getAround(a):
    returnVal = set()
    returnVal.clear()
    for n in range(-1, 2):
        for m in range(-1, 2):
            returnVal.add(Box(a[0] + n * 16, a[1] + m * 16, a[2], a[3]))
    returnVal.difference_update({a})
    return returnVal


while len(safe) > 0 or len(definite) > 0:
    clickList(safe)
    safe.clear()
    rightClickList(definite)
    definite.clear()
    pyautogui.moveTo(10, 10)
    bomb = list(pyautogui.locateAllOnScreen('Detonate.png'))
    if len(bomb) > 0:
        break
    possibleSpace = set(pyautogui.locateAllOnScreen('Untouched.png'))
    possibleDangers = list()
    for i in range(1, 9):
        Is = set(pyautogui.locateAllOnScreen(str(i) + ".png"))
        Is.difference_update(noUse.intersection(Is))
        for num in Is:
            around = getAround(num)
            UntouchedZone = around.intersection(possibleSpace)
            if len(UntouchedZone) == 0:
                noUse.add(num)
                continue
            DangerZone = around.intersection(flagged)
            if len(DangerZone) == i:
                safe.update(UntouchedZone.difference(DangerZone))
                noUse.add(num)
            elif len(UntouchedZone) + len(DangerZone) == i:
                definite.update(UntouchedZone)
                noUse.add(num)
            else:
                possibleDangers.append([UntouchedZone, i - len(DangerZone)])
            flagged.update(definite)
            possibleSpace.difference_update(safe)
    if len(safe) == 0 and len(definite) == 0:
        for i in possibleDangers:
            for j in possibleDangers:
                if i[0].issuperset(j[0]):
                    if i[1] == j[1]:
                        safe.update(i[0].difference(j[0]))
                    elif i[1] - j[1] == len(i[0].difference(j[0])):
                        definite.update(i[0].difference(j[0]))
                    else:
                        possibleDangers.remove(i)
                        possibleDangers.append([i[0].difference(j[0]), i[1] - j[1]])
                        break
    if len(safe) == 0 and len(definite) == 0 and len(possibleSpace)>0:
        print("the dice goes BRRRRRRRRRRRR")
        safe.add(random.choice(tuple(possibleDangers[random.randint(0, len(possibleDangers) - 1)][0])))
    flagged.update(definite)
pyautogui.keyDown('alt')
pyautogui.press('tab')
pyautogui.keyUp('alt')
