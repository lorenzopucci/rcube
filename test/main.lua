-- Copyright (c) 2022 Lorenzo Pucci
-- You may use, distribute and modify this code under the terms of the MIT
-- license.
--
-- You should have received a copy of the MIT license along with this program. If
-- not, see: <https://mit-license.org>.


-- This script performs a number of tests to ensure all the library's
-- features work correctly

passed = true

function test(got, expected)
    if got == expected then
        io.write("\27[32m[OK]\27[00m\n")
    else
        io.write("\27[31m[FAILED]\27[00m\n")
        passed = false
    end
end

function testList(got, expected)
    local matches = true

    for k, v in pairs(expected) do
        if v ~= got[k] then
            matches = false
            break
        end
    end

    if matches then
        io.write("\27[32m[OK]\27[00m\n")
    else
        io.write("\27[31m[FAILED]\27[00m\n")
        passed = false
    end
end


io.write("Is solved ... ")
test(isSolved(), true)


print("\n~~~ Performing M2E2S2 ~~~")
performAlgorithm("M2E2S2")

io.write("Face match: stars ... ")
test(faceMatches({axis=0, direction=1}, "*********"), true)

io.write("Face match: colors ... ")
test(faceMatches({axis=1, direction=1}, "WYWYWYWYW"), true)

io.write("Face match: not colors ... ")
test(faceMatches({axis=0, direction=-1}, "rowyORroy"), true)

io.write("Layer match: colors in central layer ... ")
test(layerMatches({axis=2, direction=0}, "YWYOROWYWROR"), true)

io.write("Layer match: colors in lateral layer ... ")
test(layerMatches({axis=0, direction=-1}, "YWYGBGroygoy"), true)

io.write("Find: corner ... ")
testList(find("wbo"), {x= -1, y= 1, z= -1})


print("\n~~~ Performing M2UR ~~~")
performAlgorithm("M2E2S2 M2UR")

io.write("Face match: letters ... ")
test(faceMatches({axis=1, direction=1}, "MMNmmAMMA"), true)

io.write("Face match: mixture 1 ... ")
test(faceMatches({axis=2, direction=-1}, "AOOaGrA**"), true)

io.write("Face match: mixture 2.0 ... ")
test(faceMatches({axis=2, direction=1}, "**wGBAaBC"), true)

io.write("Face match: mixture 2.1 ... ")
test(faceMatches({axis=2, direction=1}, "**wGBMmBC"), false)

io.write("Layer match: letters ... ")
test(layerMatches({axis=1, direction=1}, "MMAMMPcmmpPp"), true)

io.write("Layer match: mixture 1 ... ")
test(layerMatches({axis=1, direction=-1}, "AADW*dywOGrZ"), false)

io.write("Layer match: mixture 2 ... ")
test(layerMatches({axis=0, direction=0}, "AaAR*gAAyGGr"), true)

io.write("Find: center ... ")
testList(find("y"), {x= 0, y= 1, z= 0})

io.write("Find: edge ... ")
testList(find("yg"), {x= 1, y= 0, z= -1})

io.write("Get orientation: corner ... ")
testList(getStickerOrientation({x=1, y=1, z=-1}, "w"), {axis=2, direction=-1})

io.write("Get orientation: edge ... ")
testList(getStickerOrientation({x=1, y=-1, z=0}, "r"), {axis=0, direction=1})

io.write("Get orientation: center ... ")
testList(getStickerOrientation({x=0, y=-1, z=0}, "w"), {axis=1, direction=-1})


print("\n~~~ Restoring the cube ~~~")
performAlgorithm("R'U'M2")

io.write("Perform algorithm ... ")
test(isSolved(), true)

print("\n~~~ Algorithm operations ~~~")

io.write("Normalize: 1 ... ")
test(normalizeAlgo("RLUUUy"), "RLU'y")

io.write("Normalize: 2 ... ")
test(normalizeAlgo("RFF'BD2DD"), "RB")

io.write("Normalize: 3 ... ")
test(normalizeAlgo("BB'BBFRR2"), "B2FR'")

io.write("Reverse: 1 ... ")
test(reverseAlgo("R2URUR'U'R'U'R'UR'"), "RU'RURURU'R'U'R2")

io.write("Reverse: 2 ... ")
test(reverseAlgo("BDRUR'U'y2F'"), "Fy2URU'R'D'B'")

io.write("Remove rotations: 1 ... ")
test(removeRotations("RyRF'"), "RBR'")

io.write("Remove rotations: 2 ... ")
test(removeRotations("x2UR'y'LDzF2R"), "DR'FUL2D")

if passed then
    print("\nAll tests passed")
else
    print("\nErrors found")
end