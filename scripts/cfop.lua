-- Copyright (c) 2022 Lorenzo Pucci
-- You may use, distribute and modify this code under the terms of the MIT
-- license.
--
-- You should have received a copy of the MIT license along with this program. If
-- not, see: <https://mit-license.org>.


if IMP_COMMON == nil then dofile("scripts/common.lua") end

dofile("scripts/cross.lua")
dofile("scripts/F2L.lua")
dofile("scripts/OLL.lua")


---------------------------------------------------------------------------------
--------------------------------- CFOP solution ---------------------------------

print("Scramble: "..scramble(12))
rotateTo("y", "g")

cross()
F2L()
OLL()

display()