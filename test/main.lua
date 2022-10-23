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


io.write("Is solved: 1 ... ")
test(isSolved(), true)


print("\n~~~ Performing M2E2S2 ~~~")
performAlgorithm("M2E2S2")

io.write("Match: stars ... ")
test(matches(0, 1, "*********"), true)

io.write("Match: colors ... ")
test(matches(1, 1, "WYWYWYWYW"), true)

io.write("Match: not colors ... ")
test(matches(0, -1, "rowyORroy"), true)


print("\n~~~ Performing M2UR ~~~")
performAlgorithm("M2E2S2 M2UR")

io.write("Match: letters ... ")
test(matches(1, 1, "AACaaDAAD"), true)

io.write("Match: mixture 1 ... ")
test(matches(2, -1, "AOOaGrA**"), true)

io.write("Match: mixture 2 ... ")
test(matches(2, 1, "**wGBAaBC"), false)


print("\n~~~ Restoring the cube ~~~")
performAlgorithm("R'U'M2")

io.write("Perform algorithm ... ")
test(isSolved(), true)


if passed then
    print("\nAll tests passed")
else
    print("\nErrors found")
end