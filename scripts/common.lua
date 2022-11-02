-- Copyright (c) 2022 Lorenzo Pucci
-- You may use, distribute and modify this code under the terms of the MIT
-- license.
--
-- You should have received a copy of the MIT license along with this program. If
-- not, see: <https://mit-license.org>.


IMP_COMMON = 1

function compareTables(t1, t2)
    for k, v in pairs(t1) do
        if v ~= t2[k] then
            return false
        end
    end
    return true
end

function getMoveFromOrient(o)
    if     compareTables(o, {axis=0, direction=-1}) then return "L"
    elseif compareTables(o, {axis=0, direction= 1}) then return "R"
    elseif compareTables(o, {axis=1, direction=-1}) then return "D"
    elseif compareTables(o, {axis=1, direction= 1}) then return "U"
    elseif compareTables(o, {axis=2, direction=-1}) then return "B"
    elseif compareTables(o, {axis=2, direction= 1}) then return "F" end
end

function axisIntToStr(axis)
    if     axis == 0 then return "x"
    elseif axis == 1 then return "y"
    elseif axis == 2 then return "z" end
end

function moveUpCorner(colors)
    local pos = find(colors)
    moves = ""
    mv = getMoveFromOrient({axis=0, direction=pos["x"]})

    direction = "'"
    performMove(mv, 1)
    moves = moves..mv

    pos = find(colors)

    if pos["y"] ~= 1 then
        performMove(mv, 2)
        moves = moves.."'"
        direction = ""
    end

    performAlgorithm("U"..mv..direction)
    moves = moves.."U"..mv..direction

    return moves
end