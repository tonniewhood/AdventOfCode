
import math
import numpy as np
from skspatial.measurement import area_signed

def findArray(lines) -> list:

    lines = [line.strip() for line in lines]

    linesArray = []
    
    emptyLine = ["." for i in range(len(lines[0]) + 2)]
    linesArray.append(emptyLine)

    for line in lines:
        lineToAppend = ["."]
        for char in line:
            lineToAppend.append(char)
        lineToAppend.append(".")

        linesArray.append(lineToAppend)

    linesArray.append(emptyLine)

    return linesArray

def findStart(linesArray) -> tuple:

    startRow = 0;
    startCol = 0;
    
    for rowIdx in range(len(linesArray)):
        for colIdx in range(len(linesArray[rowIdx])):
            if linesArray[rowIdx][colIdx] == 'S':
                startRow = rowIdx
                startCol = colIdx

    return (startRow, startCol)

def findUpPipeTuples(linesArray, row, col) -> list:

    upPipTuples = []

    charAbove = linesArray[row - 1][col]
    charBelow = linesArray[row + 1][col]

    if charAbove == '|' or charAbove == 'F' or charAbove == '7':
        upPipTuples.append((row - 1, col))
    if charBelow == '|' or charBelow == 'J' or charBelow == 'L':  
        upPipTuples.append((row + 1, col))

    if charAbove == 'S' and len(upPipTuples) < 2:
        upPipTuples.append((row - 1, col))
    if charBelow == 'S' and len(upPipTuples) < 2:
        upPipTuples.append((row + 1, col))

    return upPipTuples
    
def findSidwaysPipeTuples(linesArray, row, col) -> list:

    downPipeTuples = []

    charLeft = linesArray[row][col - 1]
    charRight = linesArray[row][col + 1]

    if charLeft == '-' or charLeft == 'L' or charLeft == 'F':
        downPipeTuples.append((row, col - 1))
    if charRight == '-' or charRight == 'J' or charRight == '7':
        downPipeTuples.append((row, col + 1))

    if charLeft == 'S' and len(downPipeTuples) < 2:
        downPipeTuples.append((row, col - 1))
    if charRight == 'S' and len(downPipeTuples) < 2:
        downPipeTuples.append((row, col + 1))

    return downPipeTuples

def findFPipeTuples(linesArray, row, col) -> list:

    fPipeTuples = []

    charBelow = linesArray[row + 1][col]
    charRight = linesArray[row][col + 1]


    if charBelow == '|' or charBelow == 'J' or charBelow == 'L':
        fPipeTuples.append((row + 1, col))
    if charRight == '-' or charRight == 'J' or charRight == '7':
        fPipeTuples.append((row, col + 1))

    if charBelow == 'S' and len(fPipeTuples) < 2:
        fPipeTuples.append((row + 1, col))
    if charRight == 'S' and len(fPipeTuples) < 2:
        fPipeTuples.append((row, col + 1))

    return fPipeTuples

def findLPipeTuples(linesArray, row, col) -> list:

    lPipeTuples = []

    charAbove = linesArray[row - 1][col]
    charRight = linesArray[row][col + 1]

    if charAbove == '|' or charAbove == 'F' or charAbove == '7':
        lPipeTuples.append((row - 1, col))
    if charRight == '-' or charRight == 'J' or charRight == '7':
        lPipeTuples.append((row, col + 1))

    if charAbove == 'S' and len(lPipeTuples) < 2:
        lPipeTuples.append((row - 1, col))
    if charRight == 'S' and len(lPipeTuples) < 2:   
        lPipeTuples.append((row, col + 1))

    return lPipeTuples

def findJPipeTuples(linesArray, row, col) -> list:

    jPipeTuples = []

    charAbove = linesArray[row - 1][col]
    charLeft = linesArray[row][col - 1]

    if charAbove == '|' or charAbove == 'F' or charAbove == '7':
        jPipeTuples.append((row - 1, col))
    if charLeft == '-' or charLeft == 'L' or charLeft == 'F':
        jPipeTuples.append((row, col - 1))

    if charAbove == 'S' and len(jPipeTuples) < 2:
        jPipeTuples.append((row - 1, col))
    if charLeft == 'S' and len(jPipeTuples) < 2:
        jPipeTuples.append((row, col - 1))

    return jPipeTuples

def find7PipeTuples(linesArray, row, col) -> list:

    sevenPipeTuples = []

    charBelow = linesArray[row + 1][col]
    charLeft = linesArray[row][col - 1]

    if charBelow == '|' or charBelow == 'J' or charBelow == 'L':
        sevenPipeTuples.append((row + 1, col))
    if charLeft == '-' or charLeft == 'L' or charLeft == 'F':
        sevenPipeTuples.append((row, col - 1))

    if charBelow == 'S' and len(sevenPipeTuples) < 2:
        sevenPipeTuples.append((row + 1, col))
    if charLeft == 'S' and len(sevenPipeTuples) < 2:
        sevenPipeTuples.append((row, col - 1))

    return sevenPipeTuples

def findStartPipeTuples(linesArray, row, col) -> list:

    startPipeTuples = []

    charAbove = linesArray[row - 1][col]
    charBelow = linesArray[row + 1][col]
    charLeft = linesArray[row][col - 1]
    charRight = linesArray[row][col + 1]

    if charAbove == '|' or charAbove == 'F' or charAbove == '7':
        startPipeTuples.append((row - 1, col))
    if charBelow == '|' or charBelow == 'J' or charBelow == 'L':
        startPipeTuples.append((row + 1, col))
    if charLeft == '-' or charLeft == 'L' or charLeft == 'F':
        startPipeTuples.append((row, col - 1))
    if charRight == '-' or charRight == 'J' or charRight == '7':
        startPipeTuples.append((row, col + 1))

    return startPipeTuples

def findAdjacentPipes(linesArray, row, col) -> list:

    adjacentPipes = []
    currentChar = linesArray[row][col]

    if currentChar == '|':
        adjacentPipes = findUpPipeTuples(linesArray, row, col)

    elif currentChar == '-':
        adjacentPipes = findSidwaysPipeTuples(linesArray, row, col)

    elif currentChar == 'L':
        adjacentPipes = findLPipeTuples(linesArray, row, col)

    elif currentChar == 'J':
        adjacentPipes = findJPipeTuples(linesArray, row, col)

    elif currentChar == 'F':
        adjacentPipes = findFPipeTuples(linesArray, row, col)

    elif currentChar == '7':
        adjacentPipes = find7PipeTuples(linesArray, row, col)

    elif currentChar == 'S':
        adjacentPipes = findStartPipeTuples(linesArray, row, col)


    return adjacentPipes

def traversePipes(linesArray, startRow, startCol) -> tuple:
    
    loop = []
    corners = []

    adjacentPipes = findAdjacentPipes(linesArray, startRow, startCol)
    nextPipe = adjacentPipes[0]
    lastPipe = (startRow, startCol)

    distance = 1
    loop.append(nextPipe)

    nextx = adjacentPipes[0][1]
    nexty = adjacentPipes[0][0]

    lastX = adjacentPipes[1][1]
    lastY = adjacentPipes[1][0]
    
    xdiff = nextx - lastX
    ydiff = nexty - lastY

    if math.fabs(xdiff) == 1 and math.fabs(ydiff) == 1:
            corners.append((startRow, startCol))

    while nextPipe != (startRow, startCol):

        distance += 1

        adjacentPipes = findAdjacentPipes(linesArray, nextPipe[0], nextPipe[1])

        nextx = adjacentPipes[0][1]
        nexty = adjacentPipes[0][0]

        lastX = adjacentPipes[1][1]
        lastY = adjacentPipes[1][0]

        xdiff = nextx - lastX
        ydiff = nexty - lastY

        if math.fabs(xdiff) == 1 and math.fabs(ydiff) == 1:
                corners.append((nextPipe[0], nextPipe[1]))

        if lastPipe in adjacentPipes:
            adjacentPipes.remove(lastPipe)

        lastPipe = nextPipe
        nextPipe = adjacentPipes[0]
        loop.append(nextPipe)

    return (loop, corners, distance)    

def findHighestAndLowestColumn(pipeLoop) -> tuple:

    highestColumn = 0
    lowestColumn = 0

    for pipeTuple in pipeLoop:
        if pipeTuple[1] > highestColumn:
            highestColumn = pipeTuple[1]
        if pipeTuple[1] < lowestColumn:
            lowestColumn = pipeTuple[1]

    return (highestColumn, lowestColumn)

def findHighestAndLowestRow(pipeLoop) -> tuple:

    highestRow = 0
    lowestRow = 0

    for pipeTuple in pipeLoop:
        if pipeTuple[0] > highestRow:
            highestRow = pipeTuple[0]
        if pipeTuple[0] < lowestRow:
            lowestRow = pipeTuple[0]

    return (highestRow, lowestRow)

def isListInPositivelyOriented(corners, startRow, startCol) -> bool:

    startingCorner = corners[0]

    if startingCorner[0] == startRow and startingCorner[1] == startCol:
        startingCorner = corners[1]

    firstCornerIsOnTheRight = startingCorner[1] > startCol
    firstCornerIsBelow = startingCorner[0] > startRow

    return firstCornerIsOnTheRight or firstCornerIsBelow

def main():

    with open("pipes.txt", "r") as f:
        lines = f.readlines()

    linesArray = findArray(lines)

    startRow, startCol = findStart(linesArray)

    pipeLoop, corners, distance = traversePipes(linesArray, startRow, startCol)

    furthestDistance = math.floor(distance / 2)

    print("The furthest distance is: " + str(furthestDistance))

    corners = [(x, y) for (y, x) in corners]
    area = area_signed(corners)

    inside = math.fabs(area) - len(pipeLoop) / 2 + 1

    print(inside)

if __name__ == "__main__":
    main()