-- Copyright (c) 2022 Lorenzo Pucci
-- You may use, distribute and modify this code under the terms of the MIT
-- license.
--
-- You should have received a copy of the MIT license along with this program. If
-- not, see: <https://mit-license.org>.

if IMP_COMMON == nil then dofile("scripts/common.lua") end

---------------------------------------------------------------------------------
--------------------------------- White corners ---------------------------------

function cornerIsInPlace(c1, c2)
    local cornerPos = find(c1..c2.."w")
    local c1Orient = getStickerOrientation(cornerPos, c1)
    local c2Orient = getStickerOrientation(cornerPos, c2)

    return getFaceColor(c1Orient) == c1 and getFaceColor(c2Orient) == c2
end

function placeCorner(c1, c2)
    local pos = find(c1..c2.."w")

    if cornerIsInPlace(c1, c2) then
        return ""
    end

    moves = ""

    if pos["y"] == -1 then
        moves = moves..moveUpCorner(c1..c2.."w")
        pos = find(c1..c2.."w")
    end

    while true do
        f1 = getFaceColor({axis=2, direction=1})
        f2 = getFaceColor({axis=0, direction=1})
        if (f1 == c1 or f1 == c2) and (f2 == c1 or f2 == c2) then break end

        performMove("y", 1)
        moves = moves.."y"

        pos = find(c1..c2.."w")
    end

    while not compareTables(pos, {x=1, y=1, z=1}) do
        performMove("U", 1)
        moves = moves.."U"
        pos = find(c1..c2.."w")
    end

    local wOrient = getStickerOrientation(pos, "w")

    while not compareTables(wOrient, {axis=1, direction=-1}) do
        performAlgorithm("$(sexy-move)")
        moves = moves.."RUR'U'"
        pos = find(c1..c2.."w")
        wOrient = getStickerOrientation(pos, "w")
    end

    return normalizeAlgo(moves)
end

function whiteCorners()
    print("\n---- White corners ----")
    print("Orange and green corner: "..placeCorner("o", "g"))
    print("Blue and orange corner: "..placeCorner("b", "o"))
    print("Red and blue corner: "..placeCorner("r", "b"))
    print("Green and red corner: "..placeCorner("g", "r"))
end

---------------------------------------------------------------------------------
------------------------------ Middle layer edges -------------------------------

algoL = "U'L'ULUFU'F'"
algoR = "URU'R'U'F'UF"

function placeEdge(c1, c2)
    local pos = find(c1..c2)
    local moves = ""

    if pos["y"] == 0 then -- case 1

        local c1Orient = getStickerOrientation(pos, c1)
        local c2Orient = getStickerOrientation(pos, c2)

        if getFaceColor(c1Orient) == c1 and getFaceColor(c2Orient) == c2 then
            return ""
        end

        while not compareTables(pos, {x=1, y=0, z=1}) do
            performMove("y", 1)
            moves = moves.."y"
            pos = find(c1..c2)
        end

        performAlgorithm(algoR) -- reduce to case 2
        moves = moves..algoR
        pos = find(c1..c2)
    end

    if pos["y"] == 1 then -- case 2

        frontColor = c1
        topColor = c2
        if getStickerOrientation(pos, c1)["axis"] == 1 then
            frontColor = c2
            topColor = c1
        end

        while getFaceColor({axis=2, direction=1}) ~= frontColor do
            performMove("y", 1)
            moves = moves.."y"
        end

        pos = find(c1..c2)

        while not compareTables(pos, {x=0, y=1, z=1}) do
            performMove("U", 1)
            moves = moves.."U"
            pos = find(c1..c2)
        end

        if getFaceColor({axis=0, direction=-1}) == topColor then
            performAlgorithm(algoL)
            return normalizeAlgo(moves..algoL)

        elseif getFaceColor({axis=0, direction=1}) == topColor then
            performAlgorithm(algoR)
            return normalizeAlgo(moves..algoR)
        end
    end
end

function middleLayer()
    print("\n---- Middle layer ----")
    print("Orange and green edge: "..placeEdge("o", "g"))
    print("Blue and orange edge: "..placeEdge("b", "o"))
    print("Red and blue edge: "..placeEdge("r", "b"))
    print("Green and red edge: "..placeEdge("g", "r"))
end

--print("Scramble: "..scramble(12))
--rotateTo("y", "g")

--whiteCorners()

--display()