-- Copyright (c) 2022 Lorenzo Pucci
-- You may use, distribute and modify this code under the terms of the MIT
-- license.
--
-- You should have received a copy of the MIT license along with this program. If
-- not, see: <https://mit-license.org>.


if IMP_COMMON == nil then dofile("scripts/common.lua") end

---------------------------------------------------------------------------------
-------------------------------- PLL - simplified -------------------------------

TOP_ORIENT = {axis=1, direction=1}
BACK_LEFT = {x=-1, y=1, z=-1}
BACK_ORIENT = {axis=2, direction=-1}

function permCorners()
    if layerMatches(TOP_ORIENT, "A*Ca*AC*ac*c", BACK_LEFT, BACK_ORIENT) then
        performAlgorithm("$(T-perm)")
        return "$(T-perm)"
    elseif layerMatches(TOP_ORIENT, "A*aC*ca*Ac*C", BACK_LEFT, BACK_ORIENT) then
        performAlgorithm("$(Y-perm)")
        return "$(Y-perm)"
    end
    return ""
end

function permEdges()
    if layerMatches(TOP_ORIENT, "AAACaCacacCc", BACK_LEFT, BACK_ORIENT) then
        while getStickerAt(BACK_LEFT, BACK_ORIENT) ~=
            getStickerAt({x=0, y=1, z=-1}, BACK_ORIENT) do
            performMove("U", 1)
        end
        
        if getStickerAt({x=-1, y=1, z=0}, {axis=0, direction=-1}) ==
        getStickerAt({x=1, y=1, z=1}, {axis=0, direction=1}) then
            performAlgorithm("$(Ub-perm)")
            return "$(Ub-perm)"
        else
            performAlgorithm("$(Ua-perm)")
            return "$(Ua-perm)"
        end

    elseif layerMatches(TOP_ORIENT, "AaACcCaAacCc", BACK_LEFT, BACK_ORIENT) then
        performAlgorithm("$(H-perm)")
        return "$(H-perm)"
    elseif layerMatches(TOP_ORIENT, "ACAcacacaCAC", BACK_LEFT, BACK_ORIENT) then
        performAlgorithm("$(Z-perm)")
        return "$(Z-perm)"
    end
    return ""
end

function PLL()
    print("\n~~~ PLL ~~~")

    print("Corners: "..normalizeAlgo(permCorners()))
    print("Edges: "..normalizeAlgo(permEdges()))

    while not isSolved() do
        performMove("U", 1)
    end
end