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
possible = list(pyautogui.locateAllOnScreen('Untouched.png', confidence=0.8))
possibleSpace = set(possible)
topLeft = possible[0]
bottomRight = possible[-1]
reg = (topLeft[0]-10, topLeft[1]-10, bottomRight[0]-topLeft[0]+26, bottomRight[1]-topLeft[1]+26)
flagged = set()
safe = set()
a = pyautogui.locateOnScreen('start.png', confidence=0.8, region=reg)
if type(a) is not Box:
    safe.add(possible[random.randint(0, len(possible) - 1)])
else:
    safe.add(a)
definite = set()
noUse = [set(), set(), set(), set(), set(), set(), set(), set()]


def getAround(a):
    returnVal = set()
    returnVal.clear()
    for n in range(-1, 2):
        for m in range(-1, 2):
            returnVal.add(Box(a[0] + n * 16, a[1] + m * 16, a[2], a[3]))
    returnVal.difference_update({a})
    return returnVal

while len(possibleSpace) > 0:
    rightClickList(definite)
    definite.clear()
    clickList(safe)
    safe.clear()
    pyautogui.moveTo(10, 10)
    if len(list(pyautogui.locateAllOnScreen('Detonate.png', confidence=0.8, region=reg))) > 0:
        print("BOOM")
        #If you want to script to stop after a game over
        break
        #If you want the game to continue playing until it wins
        # click(pyautogui.locateOnScreen('dead.png', confidence=0.8))
        # possibleSpace = set(pyautogui.locateAllOnScreen('Untouched.png', confidence=0.8, region=reg))
        # safe.clear()
        # definite.clear()
        # safe.add(possible[random.randint(0, len(possible) - 1)])
        # flagged.clear()
        # noUse.clear()
        # noUse = [set(), set(), set(), set(), set(), set(), set(), set()]
        # continue
    possibleSpace = set(pyautogui.locateAllOnScreen('Untouched.png', confidence=0.8, region=reg))
    possibleDangers = list()
    for i in range(1, 9):
        Is = set(pyautogui.locateAllOnScreen(str(i) + "2.png", confidence=0.8, region=reg))
        Is.difference_update(noUse[i-1])
        for num in Is:
            around = getAround(num)
            UntouchedZone = around.intersection(possibleSpace)
            if len(UntouchedZone) == 0:
                noUse[i-1].add(num)
                continue
            DangerZone = around.intersection(flagged)
            if len(DangerZone) == i:
                safe.update(UntouchedZone.difference(DangerZone))
                noUse[i-1].add(num)
            elif len(UntouchedZone) + len(DangerZone) == i:
                definite.update(UntouchedZone)
                noUse[i-1].add(num)
            else:
                possibleDangers.append([UntouchedZone, i - len(DangerZone)])
            flagged.update(definite)
            possibleSpace.difference_update(safe)
    if len(safe) == 0 and len(definite) == 0:
        for i in possibleDangers:
            for j in possibleDangers:
                if i == j:
                    continue
                if i[0].issuperset(j[0]):
                    if i[1] == j[1]:
                        safe.update(i[0].difference(j[0]))
                    elif i[1] - j[1] == len(i[0].difference(j[0])):
                        definite.update(i[0].difference(j[0]))
                    else:
                        possibleDangers.append([i[0].difference(j[0]), i[1] - j[1]])
                    possibleDangers.remove(i)
                    break
                elif len(i[0].intersection(j[0])) > 0:
                    inter = i[0].intersection(j[0])
                    i_new = i[0].difference(j[0])
                    j_new = j[0].difference(i[0])
                    must_atleast = max(i[1] - len(i_new), j[1] - len(j_new), 0)
                    must_atmost = min(i[1], j[1], len(inter))
                    if must_atleast > 0 and must_atleast == must_atmost:
                        add = False
                        if len(i_new) > 0:
                            if i[1] == must_atleast:
                                safe.update(i_new)
                                possibleDangers.remove(i)
                                add = True
                            elif i[1] - must_atleast == len(i_new):
                                definite.update(i_new)
                                possibleDangers.remove(i)
                                add = True
                        if len(j_new) > 0:
                            if j[1] == must_atleast:
                                safe.update(j_new)
                                possibleDangers.remove(j)
                                add = True
                            elif j[1] - must_atleast == len(j_new):
                                definite.update(j_new)
                                possibleDangers.remove(j)
                                add = True
                        if add:
                            possibleDangers.append([inter, must_atleast])
                            break
    if len(safe) == 0 and len(definite) == 0:
        print("the dice goes BRRRRRRRRRRRR")
        if len(possibleDangers) > 0:
            safe.add(random.choice(tuple(possibleDangers[random.randint(0, len(possibleDangers) - 1)][0])))
        elif len(possibleSpace) > 0:
            safe.add(random.choice(tuple(possibleSpace)))
    flagged.update(definite)
pyautogui.keyDown('alt')
pyautogui.press('tab')
pyautogui.keyUp('alt')
