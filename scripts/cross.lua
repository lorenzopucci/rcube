
-- Copyright (c) 2022 Lorenzo Pucci
-- You may use, distribute and modify this code under the terms of the MIT
-- license.
--
-- You should have received a copy of the MIT license along with this program. If
-- not, see: <https://mit-license.org>.


---------------------------------------------------------------------------------
------------------------------------- Cross -------------------------------------


function compareTables(t1, t2)
    for k, v in pairs(t1) do
        if v ~= t2[k] then
            return false
        end
    end
    return true
end

function getCrossMove(pos)
    if pos["x"] ==  1 then  return "R" end
    if pos["x"] == -1 then  return "L" end
    if pos["z"] ==  1 then  return "F" end
    if pos["z"] == -1 then  return "B" end
end

function placeCrossBlock(color, place)
    local pos = find("w"..color)
    local wOrient = getStickerOrientation(pos, "w")
    local move = getCrossMove(place)

    local moves = ""

    if compareTables(pos, place) and wOrient["axis"] == 1 then
        return moves
    end

    if pos["y"] == -1 and wOrient["axis"] == 1 then -- case 1

        mv = getCrossMove(pos)
        performMove(mv, 2) -- reduce to case 5
        moves = moves..mv.."2"

    elseif pos["y"] == -1 and wOrient["axis"] ~= 1 then -- case 2

        mv = getCrossMove(pos)
        performMove(mv, 1) -- reduce to case 4
        moves = moves..mv

    elseif pos["y"] == 1 and wOrient["axis"] ~= 1 then -- case 3
 
        if (place["x"] == 0 and pos["z"] == 0) or
            (place["z"] == 0 and pos["x"] == 0) then -- can line up with center
        
            mv1 = getCrossMove(pos)
            performMove(mv1, 1)
            moves = moves..mv1

            pos = find("w"..color)
            cOrient = getStickerOrientation(pos, color)

            direction = "'"
            if getFaceColor(cOrient) ~= color then
                performMove(mv1, 2)
                moves = moves..mv1.."2"
                direction = ""
            end

            while pos["y"] ~= -1 do
                performMove(move, 1)
                moves = moves..move
    
                pos = find("w"..color)
            end
            
            performAlgorithm(mv1..direction)
            moves = moves..mv1..direction

            return moves
        end

        algo = ""
        if     pos["x"] ==  1 then  algo = "R'F'UFR"
        elseif pos["x"] == -1 then  algo = "LFU'F'L'"
        elseif pos["z"] ==  1 then  algo = "FRU'R'F'"
        elseif pos["z"] == -1 then  algo = "B'R'URB" end

        performAlgorithm(algo) -- reduce to case 5
        moves = moves..algo
    end

    pos = find("w"..color)
    wOrient = getStickerOrientation(pos, "w")
    
    if pos["y"] == 0 then -- case 4

        if (place["x"] == 0 and place["z"] == pos["z"] and wOrient["axis"] ~= 2)
        or (place["z"] == 0 and place["x"] == pos["x"] and wOrient["axis"] ~= 0)
        then -- Already lined up with the center

            while pos["y"] ~= -1 do
                performMove(move, 1)
                moves = moves..move
    
                pos = find("w"..color)
            end
            return moves
        end

        mv = 0
        direction = 0

        if wOrient["axis"] == 0 then
            if pos["z"] == 1 then mv = "F" else mv = "B"  end
            direction = -wOrient["direction"] * pos["z"]

        elseif wOrient["axis"] == 2 then
            if pos["x"] == 1 then mv = "R" else mv = "L"  end
            direction = wOrient["direction"] * pos["x"]
        end        

        performMove(mv, direction)
        performMove("U", 1)
        performMove(mv, -direction)

        if direction == 1 then
            moves = moves..mv.."U"..mv.."'"
        else
            moves = moves..mv.."'U"..mv
        end
    end

    pos = find("w"..color)
    wOrient = getStickerOrientation(pos, "w")

    if pos["y"] == 1 and wOrient["axis"] == 1 then
         -- case 5

        while pos["x"] ~= place["x"] or pos["z"] ~= place["z"] do
            performAlgorithm("U")
            moves = moves.."U"

            pos = find("w"..color)
        end
    
        performMove(move, 2)
        moves = moves..move.."2"
    end

    return moves
end

function cross()
    print("---- Cross ----")
    print("Green edge: "  ..placeCrossBlock("g", {x=0, y=-1, z=1}))
    print("Blue edge: "   ..placeCrossBlock("b", {x=0, y=-1, z=-1}))
    print("Red edge: "    ..placeCrossBlock("r", {x=-1, y=-1, z=0}))
    print("Orange edge: " ..placeCrossBlock("o", {x=1, y=-1, z=0}))
end


---------------------------------------------------------------------------------

--print("Scramble: "..scramble(12))
--display()

rotateTo("y", "g")
cross()

--display()