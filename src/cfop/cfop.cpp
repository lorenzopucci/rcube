/*
* Copyright (c) 2023 Lorenzo Pucci
* You may use, distribute and modify this code under the terms of the MIT
* license.
*
* You should have received a copy of the MIT license along with this program. If
* not, see: <https://mit-license.org>.
*/

#include <iostream>
#include <algorithm>

#include <rcube.hpp>
#include <solving.hpp>

#include "util.hpp"
#include "algoDButil.hpp"

extern std::vector<AlgoDBItem> algoDb;


CfopSolver::CfopSolver(const rcube::Cube &cube, Color crossColor, bool verbose)
    : _cube(cube), _verbose(verbose), _crossColor(crossColor)
{}

rcube::Algorithm CfopSolver::solve()
{
    rcube::Algorithm algo;

    algo = algo + cross();
    algo = algo + f2l();
    algo = algo + oll();
    algo = algo + pll();

    algo.normalize();
    algo.removeRotations();

    if (_verbose)
    {
        std::cout << "[CFOP] Final algorithm: " << algo.to_string() << std::endl;
        std::cout << "[CFOP] Cube solved in " << algo.length() << " moves\n";
    }
    
    return algo;
}

void rotateU(rcube::Cube *cube, rcube::Algorithm *algo, rcube::Coordinates
    *edgePos = nullptr, rcube::Coordinates *cornerPos = nullptr)
{
    cube->performMove(rcube::Move('U', 1));
    algo->push(rcube::Move('U', 1));

    if (edgePos != nullptr) edgePos->rotate(Axis::Y, 1);
    if (cornerPos != nullptr) cornerPos-> rotate(Axis::Y, 1);
}

rcube::Algorithm CfopSolver::cross()
{
    rcube::Algorithm algo;
    auto crossEdges = getAdjacentColors(_crossColor);
    algo = algo + _cube.rotateTo(getOppositeColor(_crossColor), crossEdges[0]);

    for (Color edgeColor : crossEdges)
    {
    
    rcube::Coordinates edgePos = _cube.find(_crossColor, edgeColor);
    rcube::Orientation cOrient = _cube.getStickerOrientation(edgePos, _crossColor);
    rcube::Coordinates place = getBlockPlace(&_cube, _crossColor, edgeColor);

    // skip if already in place
    if (edgePos == place && cOrient.axis == Axis::Y) continue;

    // with -Y orientation but wrong place -> pull to +Y
    if (edgePos.y() == -1 && cOrient.axis == Axis::Y)
    {
        if (edgeColor == crossEdges[0])
        {
            while (edgePos != place)
            {
                _cube.performMove(rcube::Move('D', 1));
                algo.push(rcube::Move('D', 1));
                edgePos.rotate(Axis::Y, -1);
            }
            continue;
        }
        rcube::Move mv(_cube.getStickerOrientation(edgePos, edgeColor), 2);
        _cube.performMove(mv);
        algo.push(mv);
    }

    // in boottom layer but with wrong orientation -> pull to top layer
    else if (edgePos.y() == -1 && cOrient.axis != Axis::Y)
    {
        rcube::Move mv(cOrient, 2);
        _cube.performMove(mv);
        algo.push(mv);

        edgePos.rotate(cOrient.axis, 2);
    }

    // in top layer but with orientation != +Y -> pair with center and pull down
    if (edgePos.y() == 1 && cOrient.axis != Axis::Y)
    {
        rcube::Algorithm tmpAlgo;

        if (place.coords[cOrient.axis] != 0)
        {
            rotateU(&_cube, &algo, &edgePos);
            cOrient.rotate(Axis::Y, 1);
        }
        rcube::Move mv(cOrient, cOrient.direction * (place.x() +
            place.z()) * (cOrient.axis - 1));
        tmpAlgo.push(mv);
        edgePos.rotate(cOrient.axis, mv.direction * cOrient.direction);

        rcube::Move mv1({Axis::X, place.x()}, -place.x() * edgePos.z());
        if (place.z() != 0)
        {
            mv1 = rcube::Move({Axis::Z, place.z()}, place.z() * edgePos.x());
        }
        tmpAlgo.push(mv1);

        if (_cube.getStickerAt(rcube::Coordinates(mv.getAffectedFace(),
            {Axis::Y, -1}), {Axis::Y, -1}) == _crossColor)
        {
            tmpAlgo.push(mv.getInverted());
        }

        _cube.performAlgorithm(tmpAlgo);
        algo = algo + tmpAlgo;
        continue;
    }

    edgePos = _cube.find(_crossColor, edgeColor);
    cOrient = _cube.getStickerOrientation(edgePos, _crossColor);

    // in middle layer -> line
    if (edgePos.y() == 0)
    {
        // already lined up with center -> just move down
        if ((place.x() == 0 && place.z() == edgePos.z() && cOrient.axis ==
            Axis::X) || place.z() == 0 && place.x() == edgePos.x() &&
            cOrient.axis == Axis::Z)
        {
            rcube::Move mv(_cube.getFaceOrientation(edgeColor), 1);

            while (edgePos.y() != -1)
            {
                _cube.performMove(mv);
                algo.push(mv);
                edgePos = _cube.find(_crossColor, edgeColor);
            }
            continue;
        }

        rcube::Move mv('U', 1);

        if (cOrient.axis == Axis::X)
        {
            mv = rcube::Move({Axis::Z, edgePos.z()},
                -cOrient.direction * edgePos.z());
        }
        else if (cOrient.axis == Axis::Z)
        {
            mv = rcube::Move({Axis::X, edgePos.x()},
                cOrient.direction * edgePos.x());
        }
        rcube::Algorithm tmpAlgo({mv, rcube::Move('U', 1)});

        if (_cube.getStickerAt(rcube::Coordinates(mv.getAffectedFace(),
            {Axis::Y, -1}), {Axis::Y, -1}) == _crossColor)
        {
            tmpAlgo.push(mv.getInverted());
        }

        _cube.performAlgorithm(tmpAlgo);
        algo = algo + tmpAlgo;

        edgePos = _cube.find(_crossColor, edgeColor);
        cOrient = _cube.getStickerOrientation(edgePos, _crossColor);
    }

    // orientation +Y -> line up with center and bring down
    if (edgePos.y() == 1)
    {
        while (edgePos.x() != place.x() || edgePos.z() != place.z())
        {
            _cube.performMove(rcube::Move('U', 1));
            algo.push(rcube::Move('U', 1));
            edgePos = _cube.find(_crossColor, edgeColor);
        }

        rcube::Move mv(_cube.getStickerOrientation(edgePos, edgeColor), 2);
        _cube.performMove(mv);
        algo.push(mv);
    }


    } // for (Color edgeColor : crossEdges)

    algo.normalize();

    if (_verbose)
    {
        std::cout << "[CFOP] Cross: " << algo.to_string() << std::endl;
    }

    return algo;
}

bool isEmptySlot(const rcube::Coordinates &pos,
    const std::vector<rcube::Coordinates> &fullSlots)
{
    for (rcube::Coordinates place : fullSlots)
    {
        if (place.x() == pos.x() && place.z() == pos.z()) return false;
    }
    return true;
}

rcube::Coordinates toBeEmpty(const rcube::Coordinates &pos,
    const rcube::Orientation &layer)
{
    if (layer.axis == Axis::X)
    {
        return rcube::Coordinates(pos.x(), 1, -pos.z());
    }
    return rcube::Coordinates(-pos.x(), 1, pos.z());
}

rcube::Coordinates computeEdgePlace(const rcube::Coordinates &cPos,
    const rcube::Orientation &cOrient)
{
    if (cOrient.axis == Axis::X) return rcube::Coordinates(0, 1, -cPos.z());
    return rcube::Coordinates(-cPos.x(), 1, 0);
}

bool pairIsFormed(rcube::Cube *cube, const rcube::Coordinates &cPos, const
    rcube::Coordinates &ePos, const rcube::Orientation &cOrient)
{
    if (cube->getStickerAt(cPos, {Axis::Y, 1}) != cube->getStickerAt(ePos,
        {Axis::Y, 1})) return false;

    if (cOrient.axis == Axis::Z) return cPos.x() == ePos.x();
    return cPos.z() == ePos.z();
}

rcube::Algorithm solveF2lPair(rcube::Cube cube, Color crossColor, Color c1,
    Color c2, const std::vector<rcube::Coordinates> &fullSlots)
{
    rcube::Algorithm algo;
    
    rcube::Coordinates edgePos = cube.find(c1, c2);
    rcube::Coordinates cornerPos = cube.find(c1, c2, crossColor);
    rcube::Orientation cOrient = cube.getStickerOrientation(cornerPos, crossColor);

    if (blockIsInPlace(&cube, edgePos) && blockIsInPlace(&cube, cornerPos))
    {
        return algo;
    }

    /****************************** Place corner ******************************/

    if (cornerPos.y() == 1 && cOrient.axis == Axis::Y)
    {
        while (!isEmptySlot(cornerPos, fullSlots))
        {
            rotateU(&cube, &algo, nullptr, &cornerPos);
        }

        rcube::Move mv1 = pullUp(cornerPos);
        rcube::Orientation layer = mv1.getAffectedFace();
        rcube::Algorithm tmpAlgo({mv1, getUmove(cornerPos.getRotated(layer.axis,
            layer.direction * mv1.direction), mv1.getAffectedFace()),
            mv1.getInverted()});

        cube.performAlgorithm(tmpAlgo);
        algo = algo + tmpAlgo;
    }
    else if (cornerPos.y() == -1)
    {
        rcube::Orientation layer = (cOrient.axis != Axis::Y) ? cOrient :
            (rcube::Orientation){Axis::X, cornerPos.x()};
        
        int direction = (cornerPos.getRotated(layer.axis, layer.direction).y());
        rcube::Move mv(layer, (direction == 1) ? 1 : -1);
        rcube::Algorithm tmpAlgo({mv, getUmove(cornerPos, layer),
            mv.getInverted()});

        cube.performAlgorithm(tmpAlgo);
        algo = algo + tmpAlgo;
    }

    cornerPos = cube.find(c1, c2, crossColor);
    edgePos = cube.find(c1, c2);
    cOrient = cube.getStickerOrientation(cornerPos, crossColor);

    /******************************* Place edge *******************************/

    rcube::Coordinates edgePlace = computeEdgePlace(cornerPos, cOrient);

    if (edgePos.y() == 1 && edgePos != edgePlace && !pairIsFormed(&cube,
        cornerPos, edgePos, cOrient))
    {
        rcube::Orientation layer;

        while (true)
        {
            layer = cOrient;
            if (edgePos.coords[layer.axis] == layer.direction)
            {
                layer.axis = (Axis)((layer.axis + 2) % 4); // 0->2, 2->0
                layer.direction = cornerPos.coords[layer.axis];
            }

            if (isEmptySlot(toBeEmpty(cornerPos, layer), fullSlots)) break;

            rotateU(&cube, &algo, &edgePos, &cornerPos);
            cOrient.rotate(Axis::Y, 1);
        }
        edgePlace = computeEdgePlace(cornerPos, cOrient);

        // ready to form a pair
        if (cube.getStickerAt(cornerPos, {Axis::Y, 1}) == cube.getStickerAt(
            edgePos, {Axis::Y, 1}) && layer == cOrient &&
            edgePos.coords[cOrient.axis] != cOrient.direction)
        {
            edgePlace = cornerPos;
            edgePlace.coords[layer.axis] = 0;
        }

        rcube::Move mv(layer, 1);
        if (cornerPos.getRotated(layer.axis, layer.direction).y() != -1)
        {
            mv = mv.getInverted();
        }
        cube.performMove(mv);
        algo.push(mv);

        while (edgePos != edgePlace)
        {
            rotateU(&cube, &algo, &edgePos);
        }

        cube.performMove(mv.getInverted());
        algo.push(mv.getInverted());
    }

    else if (edgePos.y() == 0)
    {
        rcube::Orientation layer;

        while (true)
        {
            if (edgePos.x() == edgePlace.x())
            {
                layer = {Axis::X, edgePos.x()};
                break;
            }
            else if (edgePos.z() == edgePlace.z())
            {
                layer = {Axis::Z, edgePos.z()};
                break;
            }
            rotateU(&cube, &algo, &edgePlace, &cornerPos);
        }

        rcube::Move mv(layer, 1);
        if (edgePos.getRotated(layer.axis, layer.direction).y() != 1)
        {
            mv = mv.getInverted();
        }
        cube.performMove(mv);
        algo.push(mv);

        cOrient = cube.getStickerOrientation(cornerPos, crossColor);
        while (cOrient != layer.getInv())
        {
            rotateU(&cube, &algo);
            cOrient.rotate(Axis::Y, 1);
        }

        cube.performMove(mv.getInverted());
        algo.push(mv.getInverted());
    }

    /******************************** Join pair ********************************/

    rcube::Coordinates cPlace = getBlockPlace(&cube, crossColor, c1, c2);
    edgePos = cube.find(c1, c2);
    cornerPos = cube.find(c1, c2, crossColor);

    if (cube.getStickerAt(edgePos, {Axis::Y, 1}) != cube.getStickerAt(
        cornerPos, {Axis::Y, 1}))
    {
        while (cornerPos.x() != cPlace.x() || cornerPos.z() != cPlace.z())
        {
            rotateU(&cube, &algo, &edgePos, &cornerPos);
        }

        cOrient = cube.getStickerOrientation(cornerPos, crossColor);
        rcube::Move mv(cOrient, 1);
        rcube::Coordinates afterRot = cornerPos.getRotated(cOrient.axis,
            cOrient.direction);
        if (afterRot.y() != 1)
        {
            mv = mv.getInverted();
            afterRot.rotate(cOrient.axis, 2);
        }
        rcube::Move mv1 = getUmove(afterRot, cOrient).getInverted();

        rcube::Algorithm tmpAlgo({mv, mv1, mv.getInverted()});
        cube.performAlgorithm(tmpAlgo);
        algo = algo + tmpAlgo;
    }
    else
    {
        if (cornerPos.x() != edgePos.x() && cornerPos.z() != edgePos.z())
        {
            cOrient = cube.getStickerOrientation(cornerPos, crossColor);

            while (!isEmptySlot(toBeEmpty(cornerPos, cOrient), fullSlots))
            {
                rotateU(&cube, &algo, &edgePos, &cornerPos);
                cOrient.rotate(Axis::Y, 1);
            }

            cOrient = cube.getStickerOrientation(cornerPos, crossColor);
            rcube::Move mv(cOrient, 1);
            if (cornerPos.getRotated(cOrient.axis, cOrient.direction).y() != -1)
                mv = mv.getInverted();

            rcube::Algorithm tmpAlgo({mv, rcube::Move('U', 2), mv.getInverted()});
            cube.performAlgorithm(tmpAlgo);
            algo = algo + tmpAlgo;

            edgePos = cube.find(c1, c2);
            cornerPos = cube.find(c1, c2, crossColor);
        }

        cOrient = cube.getStickerOrientation(cornerPos, crossColor);
        rcube::Coordinates thirdPos = cornerPos;
        thirdPos.coords[cOrient.axis] *= -1;

        while (thirdPos.x() != cPlace.x() || thirdPos.z() != cPlace.z())
        {
            rotateU(&cube, &algo, &thirdPos, &cornerPos);
            cOrient.rotate(Axis::Y, 1);
        }

        rcube::Move mv1(cOrient.getInv(), 1);
        if (cPlace.getRotated(cOrient.axis, -cOrient.direction).y() != 1)
            mv1 = mv1.getInverted();

        rcube::Move mv2('U', 1);
        rcube::Coordinates afterRot = cornerPos.getRotated(Axis::Y, 1);
        if (afterRot.x() != cPlace.x() || afterRot.z() != cPlace.z())
            mv2 = mv2.getInverted();

        rcube::Algorithm tmpAlgo({mv1, mv2, mv1.getInverted()});
        cube.performAlgorithm(tmpAlgo);
        algo = algo + tmpAlgo;
    }

    return algo;
}

rcube::Algorithm CfopSolver::f2l()
{
    rcube::Algorithm algo;
    std::vector<rcube::Coordinates> fullSlots;
    auto crossColors = getAdjacentColors(_crossColor);

    while (fullSlots.size() < 4)
    {
        rcube::Algorithm pairAlgo, tmpAlgo;

        int lowestIdx;
        int lowestSize = 100;

        for (int i = 0; i < 4; ++i)
        {
            Color c1 = crossColors[i];
            Color c2 = crossColors[(i + 1) % 4];

            if (std::find(fullSlots.begin(), fullSlots.end(),
                getBlockPlace(&_cube, _crossColor, c1, c2)) != fullSlots.end())
            {
                continue;
            }

            tmpAlgo = solveF2lPair(_cube, _crossColor, c1, c2, fullSlots);

            if (tmpAlgo.length() < lowestSize)
            {
                lowestIdx = i;
                lowestSize = tmpAlgo.length();
                pairAlgo = tmpAlgo;
            }
        }

        _cube.performAlgorithm(pairAlgo);
        algo = algo + pairAlgo;

        Color c1 = crossColors[lowestIdx];
        Color c2 = crossColors[(lowestIdx + 1) % 4];
        fullSlots.push_back(getBlockPlace(&_cube, _crossColor, c1, c2));
    }

    algo.normalize();

    if (_verbose)
    {
        std::cout << "[CFOP] F2L: " << algo.to_string() << std::endl;
    }

    return algo;
}

rcube::Algorithm CfopSolver::oll()
{
    rcube::Algorithm preAlgo("");
    rcube::Algorithm algo("");

    const rcube::Orientation TOP = {Axis::Y, 1};
    const rcube::Orientation BACK = {Axis::Z, -1};
    const rcube::Coordinates BACK_LEFT(-1, 1, -1);

    for (AlgoDBItem item : algoDb)
    {
        if (item.type != AlgoType::OLL || item.match == "") continue;

        if (_cube.layerAndFaceMatch(TOP, item.match, BACK_LEFT, BACK, &preAlgo))
        {
            algo = rcube::Algorithm(item.algo);
            break;
        }
    }
    _cube.performAlgorithm(algo);
    

    algo = preAlgo + algo;
    algo.normalize();

    if (_verbose)
    {
        std::cout << "[CFOP] OLL: " << algo.to_string() << std::endl;
    }

    return algo;
}

rcube::Algorithm CfopSolver::pll()
{
    rcube::Algorithm preAlgo("");
    rcube::Algorithm algo("");

    const rcube::Orientation TOP = {Axis::Y, 1};
    const rcube::Orientation BACK = {Axis::Z, -1};
    const rcube::Coordinates BACK_LEFT(-1, 1, -1);

    for (AlgoDBItem item : algoDb)
    {
        if (item.type != AlgoType::PLL || item.match == "") continue;
        
        if (_cube.layerMatches(TOP, item.match, BACK_LEFT, BACK, &preAlgo))
        {
            algo = rcube::Algorithm(item.algo);
            break;
        }
    }
    _cube.performAlgorithm(algo);

    while (!_cube.isSolved())
    {
        _cube.performMove(rcube::Move('U', 1));
        algo.push(rcube::Move('U', 1));
    }
    

    algo = preAlgo + algo;
    algo.normalize();

    if (_verbose)
    {
        std::cout << "[CFOP] PLL: " << algo.to_string() << std::endl;
    }

    return algo;
}