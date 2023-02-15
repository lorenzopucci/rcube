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
        rcube::Move mv(_cube.getStickerOrientation(edgePos, edgeColor), 2);
        _cube.performMove(mv);
        algo.push(mv);
    }

    // in boottom layer but with wrong orientation -> pull to middle layer
    else if (edgePos.y() == -1 && cOrient.axis != Axis::Y)
    {
        rcube::Move mv(_cube.getStickerOrientation(edgePos, _crossColor), 1);
        _cube.performMove(mv);
        algo.push(mv);
    }

    // in top layer but with orientation != +Y -> flip to +Y
    else if (edgePos.y() == 1 && cOrient.axis != Axis::Y)
    {
        rcube::Algorithm tmpAlgo;
        
        if      (edgePos.x() ==  1) tmpAlgo = rcube::Algorithm("R'F'UFR");
        else if (edgePos.x() == -1) tmpAlgo = rcube::Algorithm("LFU'F'L'");
        else if (edgePos.z() ==  1) tmpAlgo = rcube::Algorithm("FRU'R'F'");
        else if (edgePos.z() == -1) tmpAlgo = rcube::Algorithm("B'R'URB");

        _cube.performAlgorithm(tmpAlgo);
        algo = algo + tmpAlgo;
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

        rcube::Algorithm tmpAlgo({mv, rcube::Move('U', 1), mv.getInverted()});
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

void rotateU(rcube::Cube *cube, rcube::Algorithm *algo, rcube::Coordinates
    *edgePos = nullptr, rcube::Coordinates *cornerPos = nullptr)
{
    cube->performMove(rcube::Move('U', 1));
    algo->push(rcube::Move('U', 1));

    if (edgePos != nullptr) edgePos->rotate(Axis::Y, 1);
    if (cornerPos != nullptr) cornerPos-> rotate(Axis::Y, 1);
}

bool pairIsFormed(rcube::Cube *cube, const rcube::Coordinates &cPos, const
    rcube::Coordinates &ePos, const rcube::Orientation &cOrient)
{
    if (cube->getStickerAt(cPos, {Axis::Y, 1}) != cube->getStickerAt(ePos,
        {Axis::Y, 1})) return false;

    if (cOrient.axis == Axis::Z) return cPos.x() == ePos.x();
    return cPos.z() == ePos.z();
}

rcube::Algorithm CfopSolver::f2l()
{
    rcube::Algorithm algo;
    std::vector<rcube::Coordinates> fullSlots;
    auto crossColors = getAdjacentColors(_crossColor);

    for (int i = 0; i < 4; ++i)
    {
    
    Color cols[2] = {crossColors[i], crossColors[(i + 1) % 4]};
    rcube::Coordinates edgePos = _cube.find(cols[0], cols[1]);
    rcube::Coordinates cornerPos = _cube.find(cols[0], cols[1], _crossColor);
    rcube::Orientation cOrient = _cube.getStickerOrientation(cornerPos, _crossColor);

    /****************************** Place corner ******************************/

    if (cornerPos.y() == 1 && cOrient.axis == Axis::Y)
    {
        while (!isEmptySlot(cornerPos, fullSlots))
        {
            rotateU(&_cube, &algo, nullptr, &cornerPos);
        }

        rcube::Move mv1 = pullUp(cornerPos);
        rcube::Orientation layer = mv1.getAffectedFace();
        rcube::Algorithm tmpAlgo({mv1, getUmove(cornerPos.getRotated(layer.axis,
            layer.direction * mv1.direction), mv1.getAffectedFace()),
            mv1.getInverted()});

        _cube.performAlgorithm(tmpAlgo);
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

        _cube.performAlgorithm(tmpAlgo);
        algo = algo + tmpAlgo;
    }

    cornerPos = _cube.find(cols[0], cols[1], _crossColor);
    edgePos = _cube.find(cols[0], cols[1]);
    cOrient = _cube.getStickerOrientation(cornerPos, _crossColor);

    /******************************* Place edge *******************************/

    rcube::Coordinates edgePlace = computeEdgePlace(cornerPos, cOrient);

    if (edgePos.y() == 1 && edgePos != edgePlace && !pairIsFormed(&_cube,
        cornerPos, edgePos, cOrient))
    {
        rcube::Orientation layer;

        while (true)
        {
            layer = {Axis::X, cornerPos.x()};
            if (edgePos.x() != 0) layer = {Axis::Z, cornerPos.z()};

            if (isEmptySlot(toBeEmpty(cornerPos, layer), fullSlots)) break;

            rotateU(&_cube, &algo, &edgePos, &cornerPos);
        }

        cOrient = _cube.getStickerOrientation(cornerPos, _crossColor);
        edgePlace = computeEdgePlace(cornerPos, cOrient);

        // They are ready to form a pair
        /*
        if (_cube.getStickerAt(cornerPos, {Axis::Y, 1}) == _cube.getStickerAt(
            edgePos, {Axis::Y, 1}) && layer == cOrient)
        {
            edgePlace = cornerPos;
            edgePlace.coords[layer.axis] = 0;
        }*/

        rcube::Move mv(layer, 1);
        if (cornerPos.getRotated(layer.axis, layer.direction).y() != -1)
        {
            mv = mv.getInverted();
        }
        _cube.performMove(mv);
        algo.push(mv);

        while (edgePos != edgePlace)
        {
            rotateU(&_cube, &algo, &edgePos);
        }

        _cube.performMove(mv.getInverted());
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
            rotateU(&_cube, &algo, &edgePlace, &cornerPos);
        }

        rcube::Move mv(layer, 1);
        if (edgePos.getRotated(layer.axis, layer.direction).y() != 1)
        {
            mv = mv.getInverted();
        }
        _cube.performMove(mv);
        algo.push(mv);

        cOrient = _cube.getStickerOrientation(cornerPos, _crossColor);
        while (cOrient != layer.getInv())
        {
            rotateU(&_cube, &algo);
            cOrient.rotate(Axis::Y, 1);
        }

        _cube.performMove(mv.getInverted());
        algo.push(mv.getInverted());
    }

    /******************************** Join pair ********************************/

    rcube::Coordinates cPlace = getBlockPlace(&_cube, _crossColor, cols[0], cols[1]);
    edgePos = _cube.find(cols[0], cols[1]);
    cornerPos = _cube.find(cols[0], cols[1], _crossColor);

    if (_cube.getStickerAt(edgePos, {Axis::Y, 1}) != _cube.getStickerAt(
        cornerPos, {Axis::Y, 1}))
    {
        while (cornerPos.x() != cPlace.x() || cornerPos.z() != cPlace.z())
        {
            rotateU(&_cube, &algo, &edgePos, &cornerPos);
        }

        cOrient = _cube.getStickerOrientation(cornerPos, _crossColor);
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
        _cube.performAlgorithm(tmpAlgo);
        algo = algo + tmpAlgo;
    }
    else
    {
        if (cornerPos.x() != edgePos.x() && cornerPos.z() != edgePos.z())
        {
            cOrient = _cube.getStickerOrientation(cornerPos, _crossColor);

            while (!isEmptySlot(toBeEmpty(cornerPos, cOrient), fullSlots))
            {
                rotateU(&_cube, &algo, &edgePos, &cornerPos);
                cOrient.rotate(Axis::Y, 1);
            }

            cOrient = _cube.getStickerOrientation(cornerPos, _crossColor);
            rcube::Move mv(cOrient, 1);
            if (cornerPos.getRotated(cOrient.axis, cOrient.direction).y() != -1)
                mv = mv.getInverted();

            rcube::Algorithm tmpAlgo({mv, rcube::Move('U', 2), mv.getInverted()});
            _cube.performAlgorithm(tmpAlgo);
            algo = algo + tmpAlgo;

            edgePos = _cube.find(cols[0], cols[1]);
            cornerPos = _cube.find(cols[0], cols[1], _crossColor);
        }

        cOrient = _cube.getStickerOrientation(cornerPos, _crossColor);
        rcube::Coordinates thirdPos = cornerPos;
        thirdPos.coords[cOrient.axis] *= -1;

        while (thirdPos.x() != cPlace.x() || thirdPos.z() != cPlace.z())
        {
            rotateU(&_cube, &algo, &thirdPos, &cornerPos);
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
        _cube.performAlgorithm(tmpAlgo);
        algo = algo + tmpAlgo;
    }

    fullSlots.push_back(cPlace);

    } // for (int i = 0; i < 4; ++i)

    algo.normalize();

    if (_verbose)
    {
        std::cout << "[CFOP] F2L: " << algo.to_string() << std::endl;
    }

    return algo;
}

rcube::Algorithm CfopSolver::oll()
{
    // Currently a two phase oll (cross + corners)
    rcube::Algorithm preA1("");
    rcube::Algorithm a1("");
    rcube::Algorithm preA2("");
    rcube::Algorithm a2("");

    const rcube::Orientation TOP = {Axis::Y, 1};
    const rcube::Orientation BACK = {Axis::Z, -1};
    const rcube::Coordinates BACK_LEFT(-1, 1, -1);

    char topColor = (char)getOppositeColor(_crossColor);

    /********************************** Cross **********************************/

    if (_cube.faceMatches(TOP, "*a*AAA*a*", BACK_LEFT, &preA1))
    {
        a1 = rcube::Algorithm("$(T2)");
    }
    else if (_cube.faceMatches(TOP, "*A*AAa*a*", BACK_LEFT, &preA1))
    {
        a1 = rcube::Algorithm("FURU'R'F'");
    }
    else if (_cube.faceMatches(TOP, "*a*aAa*a*", BACK_LEFT, &preA1))
    {
        a1 = rcube::Algorithm("$(O1)");
    }
    _cube.performAlgorithm(a1);

    /*********************************** OLL ***********************************/

    if (_cube.faceMatches(TOP, "aAaAAAAAa", BACK_LEFT, &preA2)) // otherwise ambiguous
    {
        if (_cube.getStickerAt(rcube::Coordinates(1, 1, 1), {Axis::Z, 1}) ==
            getOppositeColor(_crossColor))
        {
            a2 = rcube::Algorithm("$(sune)");
        }
        else
        {
            a2 = rcube::Algorithm("U'$(anti-sune)");
        }
    }
    else
    {
        for (AlgoDBItem item : algoDb)
        {
            if (item.type != AlgoType::OLL || item.match == "") continue;

            std::string match = item.match;
            std::replace(match.begin(), match.end(), 'a', topColor);
            std::replace(match.begin(), match.end(), 'A',
                (char)toupper(topColor));

            if (_cube.layerMatches(TOP, match, BACK_LEFT, BACK, &preA2))
            {
                a2 = rcube::Algorithm(item.algo);
                break;
            }
        }
    }
    _cube.performAlgorithm(a2);
    

    rcube::Algorithm algo = preA1 + a1 + preA2 + a2;
    algo.normalize();

    if (_verbose)
    {
        std::cout << "[CFOP] OLL: " << algo.to_string() << std::endl;
    }

    return algo;
}

rcube::Algorithm CfopSolver::pll()
{
    // Currently a two phase pll (corners + edges)
    rcube::Algorithm preA1("");
    rcube::Algorithm a1("");
    rcube::Algorithm preA2("");
    rcube::Algorithm a2("");

    const rcube::Orientation TOP = {Axis::Y, 1};
    const rcube::Orientation BACK = {Axis::Z, -1};
    const rcube::Coordinates BACK_LEFT(-1, 1, -1);

    char topColor = (char)getOppositeColor(_crossColor);

    /********************************* Corners *********************************/

    if (_cube.layerMatches(TOP, "M*Nm*MN*mn*n", BACK_LEFT, BACK, &preA1))
    {
        a1 = rcube::Algorithm("$(T-perm)");
    }
    else if (_cube.layerMatches(TOP, "M*mN*nm*Mn*N", BACK_LEFT, BACK, &preA1))
    {
        a1 = rcube::Algorithm("$(Y-perm)");
    }
    _cube.performAlgorithm(a1);

    /********************************** Edges **********************************/

    if (_cube.layerMatches(TOP, "MMMNmNmnmnNn", BACK_LEFT, BACK, &preA2)) // otherwise ambiguous
    {
        while (_cube.getStickerAt(BACK_LEFT, BACK) !=
            _cube.getStickerAt(rcube::Coordinates(0, 1, -1), BACK))
        {
            _cube.performMove(rcube::Move('U', 1));
            a1.push(rcube::Move('U', 1));
        }

        if (_cube.getStickerAt(rcube::Coordinates(-1, 1, 0), {Axis::X, -1}) ==
            _cube.getStickerAt(rcube::Coordinates(1, 1, 1), {Axis::X, 1}))
        {
            a2 = rcube::Algorithm("$(Ub-perm)");
        }
        else
        {
            a2 = rcube::Algorithm("$(Ua-perm)");
        }
    }
    else
    {
        for (AlgoDBItem item : algoDb)
        {
            if (item.type != AlgoType::PLL || item.match == "") continue;

            if (_cube.layerMatches(TOP, item.match, BACK_LEFT, BACK, &preA2))
            {
                a2 = rcube::Algorithm(item.algo);
                break;
            }
        }
    }
    _cube.performAlgorithm(a2);

    while (!_cube.isSolved())
    {
        _cube.performMove(rcube::Move('U', 1));
        a2.push(rcube::Move('U', 1));
    }
    

    rcube::Algorithm algo = preA1 + a1 + preA2 + a2;
    algo.normalize();

    if (_verbose)
    {
        std::cout << "[CFOP] PLL: " << algo.to_string() << std::endl;
    }

    return algo;
}