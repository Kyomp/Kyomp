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
flagged = set(pyautogui.locateAllOnScreen('Flagged.png', confidence=0.8, region=reg))
safe = set()
a = pyautogui.locateOnScreen('start.png', confidence=0.8, region=reg)
if len(flagged) == 0:
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
        # If you want the game to continue playing until it wins
        click(pyautogui.locateOnScreen('dead.png', confidence=0.8))
        possibleSpace = set(pyautogui.locateAllOnScreen('Untouched.png', confidence=0.8, region=reg))
        safe.clear()
        definite.clear()
        safe.add(possible[random.randint(0, len(possible) - 1)])
        flagged.clear()
        noUse.clear()
        noUse = [set(), set(), set(), set(), set(), set(), set(), set()]
        continue
    possibleSpace = set(pyautogui.locateAllOnScreen('Untouched.png', confidence=0.8, region=reg))
    possibleDangers = list()
    for i in range(1, 9):
        Is = set(pyautogui.locateAllOnScreen(str(i) + ".png", confidence=0.8, region=reg))
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
                must = i - len(DangerZone)
                possibleDangers.append([UntouchedZone, must, must])
            flagged.update(definite)
            possibleSpace.difference_update(safe)
    if len(safe) == 0 and len(definite) == 0:
        print('logic')
        for i in possibleDangers:
            if len(i[0]) == 0:
                continue
            ToBeAdded = []
            for j in possibleDangers:
                if len(j[0]) == 0:
                    continue
                if i == j:
                    continue
                if i[0] == j[0]:
                    if i[2] - i[1] > j[2] - j[1]:
                        if i in possibleDangers:
                            possibleDangers.remove(i)
                        break
                    else:
                        possibleDangers.remove(j)
                        continue
                if len(i[0].intersection(j[0])) > 0:
                    inter = i[0].intersection(j[0])
                    i_new = i[0].difference(j[0])
                    j_new = j[0].difference(i[0])
                    must_atleast = max(i[1] - len(i_new), j[1] - len(j_new), 0)
                    must_atmost = min(i[2], j[2], len(inter))
                    IDanger = [i_new, max(i[1] - must_atmost, 0), min(i[2] - must_atleast, len(i_new))]
                    JDanger = [j_new, max(j[1] - must_atmost, 0), min(j[2] - must_atleast, len(j_new))]
                    going = [inter, must_atleast, must_atmost]
                    if going not in (possibleDangers or ToBeAdded) and len(going) > 0:
                        possibleDangers.append(going)
                    if must_atleast == must_atmost:
                        if i[2] == must_atleast:
                            safe.update(i_new)
                            # if i in possibleDangers:
                            #     possibleDangers.remove(i)
                        elif i[1] - must_atleast == len(i_new):
                            definite.update(i_new)
                            # if i in possibleDangers:
                            #     possibleDangers.remove(i)
                        elif IDanger not in (possibleDangers or ToBeAdded) and len(i_new) > 0:
                            ToBeAdded.append(IDanger)
                        if j[2] == must_atleast:
                            safe.update(j_new)
                        elif j[1] - must_atleast == len(j_new):
                            definite.update(j_new)
                        elif JDanger not in (possibleDangers or ToBeAdded) and len(j_new) > 0:
                            ToBeAdded.append(JDanger)
                    else:
                        if IDanger not in (possibleDangers or ToBeAdded) and len(i_new) > 0:
                            ToBeAdded.append(IDanger)
                        if JDanger not in (possibleDangers or ToBeAdded) and len(j_new) > 0:
                            ToBeAdded.append(JDanger)
            possibleDangers.extend(ToBeAdded)
    if len(safe) == 0 and len(definite) == 0 and len(possibleSpace) > 0:
        break
        print("the dice goes BRRRRRRRRRRRR")
        if len(possibleDangers) > 0:
            safe.add(random.choice(tuple(possibleDangers[random.randint(0, len(possibleDangers) - 1)][0])))
        elif len(possibleSpace) > 0:
            safe.add(random.choice(tuple(possibleSpace)))
    flagged.update(definite)
clickList(safe)
rightClickList(definite)
pyautogui.keyDown('alt')
pyautogui.press('tab')
pyautogui.keyUp('alt')
