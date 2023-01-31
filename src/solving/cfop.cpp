/*
* Copyright (c) 2023 Lorenzo Pucci
* You may use, distribute and modify this code under the terms of the MIT
* license.
*
* You should have received a copy of the MIT license along with this program. If
* not, see: <https://mit-license.org>.
*/

#include <iostream>

#include <rcube.hpp>
#include <solving.hpp>

#include "util.hpp"


CfopSolver::CfopSolver(const rcube::Cube &cube, Color crossColor, bool verbose)
    : _cube(cube), _verbose(verbose), _crossColor(crossColor)
{}

rcube::Algorithm CfopSolver::solve()
{
    rcube::Algorithm algo;

    algo = algo + cross();
    //algo = algo + f2l();
    //algo = algo + pll();
    //algo = algo + oll();

    algo.normalize();
    algo = algo.removeRotations();

    std::cout << "[CFOP] Final algorithm: " << algo.to_string() << std::endl;
    std::cout << "[CFOP] Cube solved in " << algo.length() << " moves\n";
    
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