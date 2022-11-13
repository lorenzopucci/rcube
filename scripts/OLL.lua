-- Copyright (c) 2022 Lorenzo Pucci
-- You may use, distribute and modify this code under the terms of the MIT
-- license.
--
-- You should have received a copy of the MIT license along with this program. If
-- not, see: <https://mit-license.org>.


if IMP_COMMON == nil then dofile("scripts/common.lua") end

---------------------------------------------------------------------------------
-------------------------------- OLL - simplified -------------------------------

TOP_ORIENT = {axis=1, direction=1}
BACK_LEFT = {x=-1, y=1, z=-1}
BACK_ORIENT = {axis=2, direction=-1}

function yellowCross()
    if faceMatches(TOP_ORIENT, "*Y*YYY*Y*") then return "" end

    if faceMatches(TOP_ORIENT, "*y*YYY*y*", BACK_LEFT) then
        performAlgorithm("FRUR'U'F'")
        return "FRUR'U'F'"
    end

    if faceMatches(TOP_ORIENT, "*Y*YYy*y*", BACK_LEFT) then
        performAlgorithm("FURU'R'F'")
        return "FURU'R'F'"
    end

    if faceMatches(TOP_ORIENT, "*y*yYy*y*") then
        performAlgorithm("FRUR'U'F'U2FURU'R'F'")
        return "FRUR'U'F'U2FURU'R'F'"
    end
end

function sevenCases()
    if faceMatches(TOP_ORIENT, "YYYYYYYYY") then return "" end

    if layerMatches(TOP_ORIENT, "YyYyyyYyYyyy", BACK_LEFT, BACK_ORIENT) then
        performAlgorithm("FRUR'U'RUR'U'RUR'U'F'")
        return "FRUR'U'RUR'U'RUR'U'F'"

    elseif layerMatches(TOP_ORIENT, "yyyyyyYyYyyy", BACK_LEFT, BACK_ORIENT) then
        performAlgorithm("R2DR'U2RD'R'U2R'")
        return "R2DR'U2RD'R'U2R'"

    elseif layerMatches(TOP_ORIENT, "YyyyyyyyYyyy", BACK_LEFT, BACK_ORIENT) then
        performAlgorithm("rUR'U'r'FRF'")
        return "rUR'U'r'FRF'"

    elseif faceMatches(TOP_ORIENT, "yYyYYYYYy", BACK_LEFT) then
        if getStickerAt({x=1, y=1, z=1}, {axis=2, direction=1}) == "y" then
            performAlgorithm("RUR'URU2R'")
            return "RUR'URU2R'"
        else
            performAlgorithm("U'L'U'LU'L'U2L")
            return "L'U'LU'L'U2L"
        end

    elseif layerMatches(TOP_ORIENT, "yyYyyyYyyYyY", BACK_LEFT, BACK_ORIENT) then
        performAlgorithm("RU2R2U'R2U'R2U2R")
        return "RU2R2U'R2U'R2U2R"

    elseif layerMatches(TOP_ORIENT, "yyyYyyyyYyyy", BACK_LEFT, BACK_ORIENT) then
        performAlgorithm("L'RUR'U'LURU'R'")
        return "L'RUR'U'LURU'R'"
    end
end

function OLL()
    print("\n~~~ OLL ~~~")
    print("Yellow cross: "..yellowCross())
    print("OLL: "..sevenCases())
end