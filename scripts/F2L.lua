-- Copyright (c) 2022 Lorenzo Pucci
-- You may use, distribute and modify this code under the terms of the MIT
-- license.
--
-- You should have received a copy of the MIT license along with this program. If
-- not, see: <https://mit-license.org>.


if IMP_COMMON == nil then dofile("scripts/common.lua") end

---------------------------------------------------------------------------------
--------------------------------- F2L - advanced --------------------------------

fullSlots = {}

function cornerIsInPlace(place, c1, c2)
    local cornerPos = find(c1..c2.."w")
    local wOrient = getStickerOrientation(cornerPos, "w")

    return compareTables(place, cornerPos) and wOrient["axis"] == 1
end

function edgeIsInPlace(c1, c2)
    local edgePos = find(c1..c2)
    local c1Orient = getStickerOrientation(edgePos, c1)
    local c2Orient = getStickerOrientation(edgePos, c2)

    return getFaceColor(c1Orient) == c1 and getFaceColor(c2Orient) == c2
end

function isEmptySlot(pos)
    for k, v in pairs(fullSlots) do
        if pos["x"] == v["x"] and pos["z"] == v["z"] then
            return false
        end
    end
    return true
end

function toBeEmpty(pos, layer)
    if layer == "" then layer = getStickerOrientation(pos, "w") end

    if     layer["axis"] == 0 then return {x=pos["x"], y=1, z=-pos["z"]}
    elseif layer["axis"] == 2 then return {x=-pos["x"], y=1, z=pos["z"]} end
end

function placeCorner(c1, c2)
    -- Puts the corner in the top layer with the  white sticker not facing
    -- upwards, regardless of the edge

    local pos = find(c1..c2.."w")
    local wOrient = getStickerOrientation(pos, "w")
    local moves = ""

    if pos["y"] == 1 then
        if wOrient["axis"] ~= 1 then return "" end

        while not isEmptySlot(pos) do
            performMove("U", 1)
            moves = moves.."U"
            pos = find(c1..c2.."w")
        end

        direction = "'"
        layer = getStickerOrientation(pos, c1)
        mv = getMoveFromOrient(layer)
        performMove(mv, 1)
        moves = moves..mv
        pos = find(c1..c2.."w")

        if pos["y"] ~= 1 then
            performMove(mv, 2)
            moves = moves.."'"
            direction = ""
        end
        pos = find(c1..c2.."w")

        while pos[axisIntToStr(layer["axis"])] == layer["direction"] do
            performMove("U", 1)
            moves = moves.."U"
            pos = find(c1..c2.."w")
        end

        performAlgorithm(mv..direction)
        moves = moves..mv..direction
        return moves
    end

    layer = getStickerOrientation(pos, c1)
    if wOrient["axis"] ~= 1 then layer = wOrient
    elseif layer["axis"] == 1 then layer = getStickerOrientation(pos, c2) end

    direction = "'"
    mv = getMoveFromOrient(layer)
    performMove(mv, 1)
    moves = moves..mv
    pos = find(c1..c2.."w")

    if pos["y"] ~= 1 then
        performMove(mv, 2)
        moves = moves.."'"
        direction = ""
    end
    pos = find(c1..c2.."w")

    while pos[axisIntToStr(layer["axis"])] == layer["direction"] do
        performMove("U", 1)
        moves = moves.."U"
        pos = find(c1..c2.."w")
    end

    performAlgorithm(mv..direction)
    moves = moves..mv..direction
    return moves
end

function computeEdgePlace(c1, c2)
    -- Returns the place of the edge so as it forms an L on the top layer with
    -- the corner
    cornerPos = find(c1..c2.."w")
    wOrient = getStickerOrientation(cornerPos, "w")

    if     wOrient["axis"] == 0 then return {x=0, y=1, z=-cornerPos["z"]}
    elseif wOrient["axis"] == 2 then return {x=-cornerPos["x"], y=1, z=0} end
end

function placeEdge(c1, c2)

    local pos = find(c1..c2)
    local place = computeEdgePlace(c1, c2)
    local cornerPos = find(c1..c2.."w")

    moves = ""

    if pos["y"] == 1 then
        if compareTables(pos, place) then return "" end

        layer = {}

        while true do
            layer = getStickerOrientation(cornerPos, "w")
            if pos[axisIntToStr(layer["axis"])] ~= 0 then
                layer = getStickerOrientation(cornerPos, c1)
                if layer["axis"] == 1 then
                    layer = getStickerOrientation(cornerPos, c2)
                end
            end
            if isEmptySlot(toBeEmpty(cornerPos, layer)) then break end
            performMove("U", 1)
            moves = moves.."U"
            cornerPos = find(c1..c2.."w")
            pos = find(c1..c2)
        end

        if moves ~= "" then pos = find(c1..c2) end

        dest = computeEdgePlace(c1, c2)

        -- In this case, form the pair directly
        if getStickerAt(pos, {axis=1, direction=1}) == getStickerAt(cornerPos,
            {axis=1, direction=1}) and layer ==
            getStickerOrientation(cornerPos, "w") then
            dest = cornerPos
            dest[axisIntToStr(layer["axis"])] = 0
        end

        direction = "'"
        mv = getMoveFromOrient(layer)
        performMove(mv, 1)
        moves = moves..mv
        cornerPos = find(c1..c2.."w")

        if cornerPos["y"] == 1 then
            performMove(mv, 2)
            moves = moves.."'"
            direction = ""
        end
        
        while not compareTables(pos, dest) do
            performMove("U", 1)
            moves = moves.."U"
            pos = find(c1..c2)
        end

        performAlgorithm(mv..direction)
        return moves..mv..direction
    end

    layer = {}
    while true do
        if place["x"] == pos["x"] then
            layer = {axis=0, direction=pos["x"]}
            break
        elseif place["z"] == pos["z"] then
            layer = {axis=2, direction=pos["z"]}
        end
        performMove("U", 1)
        moves = moves.."U"
        place = computeEdgePlace(c1, c2)        
    end

    mv = getMoveFromOrient(layer)
    performMove(mv, 1)
    moves = moves..mv
    pos = find(c1..c2)
    direction = "'"

    if pos["y"] ~= 1 then
        performMove(mv, 2)
        moves = moves.."'"
        direction = ""
    end
    
    cornerPos = find(c1..c2.."w")
    wOrient = getStickerOrientation(cornerPos, "w")

    while wOrient["axis"] ~= layer["axis"] or wOrient["direction"] ==
        layer["direction"] do
        performMove("U", 1)
        moves = moves.."U"

        cornerPos = find(c1..c2.."w")
        wOrient = getStickerOrientation(cornerPos, "w")
    end

    performAlgorithm(mv..direction)
    return moves..mv..direction
end

function getThirdPlace(cornerPos)
    thirdPos = cornerPos
    wOrient = getStickerOrientation(cornerPos, "w")
    thirdPos[axisIntToStr(wOrient["axis"])] = -wOrient["direction"]
    return thirdPos
end

function placeF2Lpair(c1, c2, place)

    if cornerIsInPlace(place, c1, c2) and edgeIsInPlace(c1, c2) then
        fullSlots[#fullSlots + 1] = place
        return ""
    end

    local moves = ""

    moves = moves..placeCorner(c1, c2)
    moves = moves..placeEdge(c1, c2)

    cornerPos = find(c1..c2.."w")
    edgePos = find(c1..c2)

    if getStickerAt(cornerPos, {axis=1, direction=1}) ~=
    getStickerAt(edgePos, {axis=1, direction=1}) then

        while cornerPos["x"] ~= place["x"] or cornerPos["z"] ~= place["z"] do
            performAlgorithm("U")
            moves = moves.."U"
            cornerPos = find(c1..c2.."w")
        end

        wOrient = getStickerOrientation(cornerPos, "w")
        mv = getMoveFromOrient(wOrient)
        performMove(mv, 1)
        moves = moves..mv
        direction = "'"
        cornerPos = find(c1..c2.."w")

        if cornerPos["y"] ~= 1 then
            performMove(mv, 2)
            moves = moves.."'"
            direction = ""
        end

        performMove("U", 1)
        moves = moves.."U"
        cornerPos = find(c1..c2.."w")

        if cornerPos["x"] ~= place["x"] or cornerPos["z"] ~= place["z"] then
            performMove("U", 2)
            moves = moves.."'"
        end

        performAlgorithm(mv..direction)
        moves = moves..mv..direction

        fullSlots[#fullSlots + 1] = place
        return normalizeAlgo(moves)
    end

    if cornerPos["x"] ~= edgePos["x"] and cornerPos["z"] ~= edgePos["z"] then

        while not isEmptySlot(toBeEmpty(cornerPos, "")) do
            performMove("U", 1)
            moves = moves.."U"
            cornerPos = find(c1..c2.."w")
        end
        wOrient = getStickerOrientation(cornerPos, "w")
        mv = getMoveFromOrient(wOrient)
        performMove(mv, 1)
        moves = moves..mv
        direction = "'"
        cornerPos = find(c1..c2.."w")
        if cornerPos["y"] == 1 then
            performMove(mv, 2)
            moves = moves.."'"
            direction = ""
        end

        performMove("U", 2)
        moves = moves.."U2"

        performAlgorithm(mv..direction)
        moves = moves..mv..direction
    end
    cornerPos = find(c1..c2.."w")
    thirdPos = getThirdPlace(cornerPos)

    while thirdPos["x"] ~= place["x"] or thirdPos["z"] ~= place["z"] do
        performMove("U", 1)
        moves = moves.."U"
        cornerPos = find(c1..c2.."w")
        thirdPos = getThirdPlace(cornerPos)
    end

    cornerPos = find(c1..c2.."w")
    edgePos = find(c1..c2)
    wOrient = getStickerOrientation(cornerPos, "w")
    layer = {axis=wOrient["axis"], direction=-wOrient["direction"]}
    mv = getMoveFromOrient(layer)
    performMove(mv, 1)
    moves = moves..mv
    direction = "'"

    -- Since the layer to turn has no known pieces, to verify it turned in the
    -- right direction this code checks if there is a white edge (part of the
    -- cross) that went up.
    lookupPos = {x=thirdPos["x"], y=0, z=thirdPos["z"]}
    lookupOrient = {axis=(layer["axis"] + 2) % 4,
        direction=edgePos[axisIntToStr((layer["axis"] + 2) % 4)]}

    if getStickerAt(lookupPos, lookupOrient) ~= "w" then
        performMove(mv, 2)
        moves = moves.."'"
        direction = ""
    end

    performMove("U", 1)
    moves = moves.."U"
    cornerPos = find(c1..c2.."w")

    if cornerPos["x"] ~= place["x"] or cornerPos["z"] ~= place["z"] then
        performMove("U", 2)
        moves = moves.."'"
    end

    performAlgorithm(mv..direction)
    moves = moves..mv..direction

    fullSlots[#fullSlots + 1] = place
    return normalizeAlgo(moves)
end

function F2L()
    print("\n---- F2L ----")
    print("Green and orange pair: "..placeF2Lpair("g", "o", {x=1, y=-1, z=1}))
    print("Orange and blue pair: " ..placeF2Lpair("o", "b", {x=1, y=-1, z=-1}))
    print("Blue and red pair: "    ..placeF2Lpair("b", "r", {x=-1, y=-1, z=-1}))
    print("Red and green pair: "   ..placeF2Lpair("r", "g", {x=-1, y=-1, z=1}))
end