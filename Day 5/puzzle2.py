
def printAlmanac(almanac: dict) -> None:


    for key in almanac.keys():

        print(key)

        print("\tMaps:")
        for line in almanac[key]:
            map = line["map"]
            print("\t\t", map)

        print("\tRanges:")
        for line in almanac[key]:
            rng = line["range"]
            print("\t\t", rng)

def printSeeds(seeds: list) -> None:

    for seed in seeds:
        print(seed)

def traverse(sectionMap: dict, index: int) -> int:

    src = sectionMap["source"]
    dest = sectionMap["destination"]
    rng = sectionMap["range"]

    if index < src or index > src + rng - 1:
        return index
    else:
        offset = index - src
        return dest + offset

def sortRanges(ranges: list) -> None:

    for i in range(len(ranges)):
        for j in range(len(ranges) - 1):
            if ranges[j]["startIdx"] > ranges[j+1]["startIdx"]:
                ranges[j], ranges[j+1] = ranges[j+1], ranges[j]

def determineRange(section: list, index: int) -> dict:

    ranges = []
    for line in section:
        ranges.append(line["range"])

    sortRanges(ranges)

    for rng in ranges:
        if (index >= rng["startIdx"]) and (index <= rng["endIdx"]):
            return rng
        
    return None

def determineMap(section: list, index: int) -> dict:

    maps = []
    for line in section:
        maps.append(line["map"])

    for map in maps:
        if (index >= map["source"]) and (index <= map["source"] + map["range"] - 1):
            return map

    return None

def determineSeedRangeInSectionRange(seedStart: int, seedEnd: int, rangeStart: int, rangeEnd: int) -> dict:

    newSeedStart = 0
    newSeedEnd = 0

    if seedStart > rangeEnd or seedEnd < rangeStart:
        return None

    if seedStart < rangeStart:
        newSeedStart = rangeStart
    else:
        newSeedStart = seedStart

    if seedEnd > rangeEnd:
        newSeedEnd = rangeEnd
    else:
        newSeedEnd = seedEnd

    newSeed = {
        "startIdx": newSeedStart,
        "endIdx": newSeedEnd,
        "range": newSeedEnd - newSeedStart + 1
    }

    return newSeed

def determineNewSeeds(mapping: list, seeds: list) -> list:

    newSeedsToMap = []
    newSeedsMapped = []

    ranges = []
    maps = []
    for line in mapping:
        ranges.append(line["range"])
        maps.append(line["map"])

    sortRanges(ranges)

    for range in ranges:

        rangeStart = range["startIdx"]
        rangeEnd = range["endIdx"]

        for seed in seeds:

            seedStart = seed["startIdx"]
            seedEnd = seed["endIdx"]

            newSeed = determineSeedRangeInSectionRange(seedStart, seedEnd, rangeStart, rangeEnd)


            if newSeed is not None:
                newSeedsToMap.append(newSeed)


    for seed in newSeedsToMap:
        map = determineMap(mapping, seed["startIdx"])
        newSeed = {
            "startIdx": traverse(map, seed["startIdx"]),
            "endIdx": traverse(map, seed["endIdx"]),
            "range": seed["range"]
        }
        newSeedsMapped.append(newSeed)
                
    return newSeedsMapped, newSeedsToMap

def findIntervals(seedStart: int, seedEnd: int, rangeStart: int, rangeEnd: int) -> list:

    intervals = []

    if seedStart == rangeStart:

        # If the new seed is end to end the same, then just return that
        if seedEnd == rangeEnd:
            intervals.append({
                    "startIdx": seedStart,
                    "endIdx": seedEnd,
                    "range": seedEnd - seedStart + 1
                })

        # If the new seed is only the start of the range, then return the start to the end of the newSeed, and everything after as well
        else:    
            intervals.append({
                "startIdx": rangeStart,
                "endIdx": rangeEnd,
                "range": rangeEnd - rangeStart + 1
            })
            intervals.append({
                "startIdx": rangeEnd + 1,
                "endIdx": seedEnd,
                "range": seedEnd - rangeEnd
            })

    # If the new seed is only the end of the range, then return the start of the newSeed to the end, and everything before as well
    elif seedEnd == rangeEnd:
        intervals.append({
            "startIdx": seedStart,
            "endIdx": rangeStart - 1,
            "range": seedEnd - (rangeStart - 1) + 1
        })
        intervals.append({
            "startIdx": rangeStart,
            "endIdx": rangeEnd,
            "range": rangeEnd - rangeStart + 1
        })
    
    # If the new seed is in the middle of the range, then return everything up to the newSeed, the newSeed, and everything after
    else:
        intervals.append({
            "startIdx": seedStart,
            "endIdx": rangeStart - 1,
            "range": seedStart - (rangeStart - 1) + 1
        })
        intervals.append({
            "startIdx": rangeStart,
            "endIdx": rangeEnd,
            "range": rangeEnd - rangeStart + 1
        })
        intervals.append({
            "startIdx": rangeEnd + 1,
            "endIdx": seedEnd,
            "range": seedEnd - (rangeEnd + 1) + 1
        })

    return intervals

def splitIntoIntervals(inputSeeds: list, newRowSeeds: list):

    inputSeedInvervals = inputSeeds

    for rowSeed in newRowSeeds:
        
        rowSeedStart = rowSeed["startIdx"]
        rowSeedEnd = rowSeed["endIdx"]

        for inputSeed in inputSeedInvervals:

            inputSeedStart = inputSeed["startIdx"]
            inputSeedEnd = inputSeed["endIdx"]

            newIntervalSeeds = []

            # If the 
            if rowSeedStart >= inputSeedStart and rowSeedEnd <= inputSeedEnd:
                indexOfInputSeed = inputSeedInvervals.index(inputSeed)
                inputSeedInvervals.pop(indexOfInputSeed)
                
                newIntervalSeeds = findIntervals(inputSeedStart, inputSeedEnd, rowSeedStart, rowSeedEnd)
                
                inputSeedInvervals += newIntervalSeeds
                break

    return [x for x in inputSeedInvervals if x not in newRowSeeds]

def findLowest(seeds: list) -> int:

    lowesetIdx = seeds[0]["startIdx"]
    for seed in seeds:
        if seed["startIdx"] < lowesetIdx:
            lowesetIdx = seed["startIdx"]

    return lowesetIdx

def main():

    with open("seedMap.txt", "r") as f:
        seedMap = f.read().splitlines()


    for line in seedMap:
        print(line)

    # get the seeds as a list of strings
    seeds = seedMap[0].split(" ")
    seeds.pop(0)

    sectionIdx = 0
    almanac = {}

    for line in seedMap[2:]:
        
        if line == "":
            continue
        else:
            
            char = line[0]

            lineIsDigits = True
            try:
                int(char)
            except ValueError:
                lineIsDigits = False

            if not lineIsDigits:

                dictKey = line
                almanac[dictKey] = []

                sectionIdx += 1

                continue

            else:
            
                lineItems = line.split(" ")

                newMap = {
                    "destination": int(lineItems[0]),
                    "source": int(lineItems[1]),
                    "range": int(lineItems[2]),
                }
                newRange = {
                    "startIdx": int(lineItems[1]),
                    "endIdx": int(lineItems[1]) + int(lineItems[2]) - 1,
                    "range": int(lineItems[2])
                }
                lineDict = {
                    "map": newMap,
                    "range": newRange
                }

                almanac[dictKey].append(lineDict)

    seedRanges = []
    for i in range(0, len(seeds), 2):
        
        seedRanges.append({
            "startIdx": int(seeds[i]),
            "endIdx": int(seeds[i+1]) + int(seeds[i]) - 1,
            "range": int(seeds[i+1])
        })
        
    nextSeeds = seedRanges

    for mapping in almanac.values():
        
        newSeeds, rowSeeds = determineNewSeeds(mapping, nextSeeds)
        subSeeds = splitIntoIntervals(nextSeeds, rowSeeds)

        nextSeeds = newSeeds + subSeeds

    lowestIdx = findLowest(nextSeeds)
    print("The lowest posbbile index is: ", lowestIdx)

    # ===== Case 1: seed-to-soil map =====
    # mapping1 = almanac["seed-to-soil map:"]
    # case1Seeds = [
    #     {"startIdx": 55, "endIdx": 67, "range": 13},
    #     {"startIdx": 79, "endIdx": 92, "range": 14},
    # ]

    # # ===== Case 2: soil-to-fertilizer map =====
    # mapping2 = almanac["soil-to-fertilizer map:"]
    # case2Seeds = [
    #     {"startIdx": 57, "endIdx": 69, "range": 13},
    #     {"startIdx": 81, "endIdx": 94, "range": 14},
    # ]

    # # ===== Case 3: fertilizer-to-water map =====
    # mapping3 = almanac["fertilizer-to-water map:"]
    # case3Seeds = [
    #     {"startIdx": 57, "endIdx": 69, "range": 13},
    #     {"startIdx": 81, "endIdx": 94, "range": 14},
    # ]

    # # ===== Case 4: water-to-light map =====
    # mapping4 = almanac["water-to-light map:"]
    # case4Seeds = [
    #     {"startIdx": 53, "endIdx": 56, "range": 4},
    #     {"startIdx": 61, "endIdx": 69, "range": 9},
    #     {"startIdx": 81, "endIdx": 94, "range": 14},
    # ]

    # # ===== Case 5: light-to-temperature map =====
    # mapping5 = almanac["light-to-temperature map:"]
    # case5Seeds = [
    #     {"startIdx": 46, "endIdx": 49, "range": 4},
    #     {"startIdx": 54, "endIdx": 62, "range": 9},
    #     {"startIdx": 74, "endIdx": 87, "range": 14},
    # ]

    # # ===== Case 6: temperature-to-humidity map =====
    # mapping6 = almanac["temperature-to-humidity map:"]
    # case6Seeds = [
    #     {"startIdx": 45, "endIdx": 55, "range": 11},
    #     {"startIdx": 78, "endIdx": 80, "range": 3},
    #     {"startIdx": 82, "endIdx": 85, "range": 4},
    #     {"startIdx": 90, "endIdx": 98, "range": 9}
    # ]

    # # ===== Case 7: humidity-to-location map =====
    # mapping7 = almanac["humidity-to-location map:"]
    # case7Seeds = [
    #     {"startIdx": 46, "endIdx": 56, "range": 10},
    #     {"startIdx": 78, "endIdx": 80, "range": 3},
    #     {"startIdx": 82, "endIdx": 85, "range": 4},
    #     {"startIdx": 90, "endIdx": 98, "range": 9}
    # ]

    # # ===== Final Result ===== #
    # finalSeeds = [
    #     {"startIdx": 46, "endIdx": 60, "range": 15},
    #     {"startIdx": 82, "endIdx": 84, "range": 3},
    #     {"startIdx": 86, "endIdx": 89, "range": 4},
    #     {"startIdx": 94, "endIdx": 98, "range": 5}
    # ]


    # newSeeds, rowSeeds = determineNewSeeds(mapping7, case7Seeds)
    # subSeeds = splitIntoIntervals(case7Seeds, rowSeeds)

    # # printSeeds(subSeeds)
    # # print(rowSeeds)

    # ## Note, make sure to not add any trimmed seeds if they are also newSeeds

    # nextSeeds = newSeeds + subSeeds
    # # print(trimmedSeeds)
    # # print(rollOverSeeds)
    # print(nextSeeds)


if __name__ == "__main__":
    main()

