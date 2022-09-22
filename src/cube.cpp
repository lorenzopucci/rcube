#include <map>
#include <set>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <assert.h>

#include <rcube.hpp>
#include <utility.hpp>

using pairOrientCenter = std::pair<rcube::Orientation, rcube::Center*>;
using pairCoords2DColor = std::pair<rcube::Coordinates2D, Color>;
using pairOrientColor = std::pair<rcube::Orientation, Color>;


int rcube::Cube::getCenterIndex (const Color& color)
{
    for (int index = 0; index < 6; ++index)
    {
        if (centers[index].color == color)
            return index;
    }
}

rcube::Center* rcube::Cube::getCenterPtr (const Color& color)
{
    return centers + getCenterIndex(color);
}

std::string colorize (const char& color)
{
    switch (color)
    {
        case 'y': return "\033[103;30mY\033[0m";
        case 'w': return "\033[47;30mW\033[0m";
        case 'g': return "\033[42;30mG\033[0m";
        case 'o': return "\033[43;30mO\033[0m";
        case 'b': return "\033[44;37mB\033[0m";
        case 'r': return "\033[41;37mR\033[0m";
        default: return "\033[0m" + color;
    }
}

void rcube::Cube::display ()
{
    rcube::Net toDisplay = netRender();
    for (int i = 0; i < 6; ++i)
    {
        std::cout << "\nFace at index: " << i << std::endl;

        for (int y = 1; y >= -1; --y)
        {
            for (int x = -1; x <= 1; ++x)
            {
                std::cout << colorize((char)toDisplay.faces[i].stickers[
                    {x, y}]);
            }
            std::cout << std::endl;
        }
    }
}

rcube::Cube::Cube(const Color& topColor, const Color& frontColor)
{
    // Initializes the cube by assigning a value to:
    // - edges, centers and corners
    // - viewpoint
    // - adjacentCentersPtr
    
    // template
    CenterId adjacentCenters[6][4] = {
        {CenterId::Green, CenterId::Orange, CenterId::Blue, CenterId::Red},
        {CenterId::Green, CenterId::Red, CenterId::Blue, CenterId::Orange},
        {CenterId::White, CenterId::Red, CenterId::Yellow, CenterId::Orange},
        {CenterId::White, CenterId::Orange, CenterId::Yellow, CenterId::Red},
        {CenterId::White, CenterId::Blue, CenterId::Yellow, CenterId::Green},
        {CenterId::White, CenterId::Green, CenterId::Yellow, CenterId::Blue}
    };

    // initialize centers
    char colors[6] = {'w', 'y', 'g', 'b', 'r', 'o'};
    for (int i = 0; i < 6; ++i)
        centers[i] = (rcube::Center) {static_cast<Color>(colors[i])};

    // initialize adjacentCentersPtr
    for (int i = 0; i < 6; ++i)
    {
        std::pair<rcube::Center*, rcube::AdjacentCenters> toAdd(
            centers + i,
            (rcube::AdjacentCenters) {{}}
        );
        for (int x = 0; x < 4; ++x)
        {
            toAdd.second.centers[x] = centers + (int)adjacentCenters[i][x];
        }
        adjacentCentersPtr.insert(toAdd);
    }

    // initialize viewpoint
    viewpoint.insert(pairOrientCenter({Y, 1}, getCenterPtr(topColor)));
    viewpoint.insert(pairOrientCenter({Z, 1}, getCenterPtr(frontColor)));

    rcube::AdjacentCenters adjToFront = adjacentCentersPtr[viewpoint[{Z, 1}]];

    for (int i = 0; i < 4; ++i) // find bottom center
    {
        if (adjToFront.centers[i] == viewpoint[{Y, 1}])
        {
            viewpoint[{Y, -1}] = adjToFront.centers[(i + 2) % 4];
            break;
        }
    }

    rcube::AdjacentCenters adjToBottom = adjacentCentersPtr[viewpoint[{Y, -1}]];

    for (int i = 0; i < 4; ++i)
    {
        if (adjToBottom.centers[i] == viewpoint[{Z, 1}])
        {
            viewpoint[{X, 1}] = adjToBottom.centers[(i + 1) % 4];
            viewpoint[{Z, -1}] = adjToBottom.centers[(i + 2) % 4];
            viewpoint[{X, -1}] = adjToBottom.centers[(i + 3) % 4];
            break;
        }
        assert(i != 3);
    }

    // initialize edges and corners
    int edgesIndex = 0; // index in edges array
    int cornersIndex = 0; // index in corner array

    for (int a1 = 0, d1 = -1; a1 < 2; d1 *= -1)
    {
        rcube::Orientation o1 = {(Axis)a1, d1};
        rcube::Orientation o3 = {(Axis)((a1 + 1) % 2), d1 * (-1 + a1 * 2)};

        for (int d2 = -1; d2 < 2; d2 += 2)
        {
            rcube::Orientation o2 = {Axis::Z, d2};

            Edge edge = {{}, rcube::Coordinates(o1, o2)};
            edge.stickers[0] = {viewpoint[o1]->color, viewpoint[o1], o1};
            edge.stickers[1] = {viewpoint[o2]->color, viewpoint[o2], o2};
            edges[edgesIndex] = edge;
            edgesIndex++;

            Corner corner = {{}, rcube::Coordinates(o1, o2, o3)};
            corner.stickers[0] = edge.stickers[0];
            corner.stickers[1] = edge.stickers[1];
            corner.stickers[2] = {viewpoint[o3]->color, viewpoint[o3], o3};
            corners[cornersIndex] = corner;
            cornersIndex++;
        }

        Edge edge = {{}, rcube::Coordinates(o1, o3)};
        edge.stickers[0] = {viewpoint[o1]->color, viewpoint[o1], o1};
        edge.stickers[1] = {viewpoint[o3]->color, viewpoint[o3], o3};
        edges[edgesIndex] = edge;
        edgesIndex++;

        if (d1 == 1) a1++;
    }
}

void rcube::Cube::performMove (const rcube::Move& move)
{
    switch (move.face)
    {
        case UP:
        case FRONT:
        case RIGHT:
        case LEFT:
        case DOWN:
        case BACK:
            rotateLayer(move); return;

        case ROTATE_X:
        case ROTATE_Y:
        case ROTATE_Z:
            changeViewpoint(move); return;

        case MIDDLE:
        case EQUATOR:
        case SIDE:
        {
            std::map<MoveFace, std::string> algos = {
                {MIDDLE, "RL'x'"},
                {EQUATOR, "UD'y'"},
                {SIDE, "F'Bz"}
            };

            rcube::Algorithm algo(algos[move.face]);
            performAlgorithm(algo * move.direction);
            return;
        }
    }
}

void rcube::Cube::changeViewpoint (const rcube::Move& move)
{
    Axis affectedAxis[2]; // all the axis apart form the rotation one
    for (int i = 0; i < 2; ++i)
        affectedAxis[i] = (Axis)(((int)move.axis + i + 1) % 3);

    if (move.direction == MoveDirection::DOUBLE)
    {
        // maintain same axis but swap direction
        for (int i = 0; i < 2; ++i)
        {
            rcube::Center* tmp = viewpoint[{affectedAxis[i], -1}];
            viewpoint[{affectedAxis[i], -1}] = viewpoint[{affectedAxis[i], 1}];
            viewpoint[{affectedAxis[i], 1}] = tmp;
        }
    }
    else
    {
        for (int i = 0; i < 4; ++i)
        {
            // at each iteration, swap the faces between different axis at
            // the same/oppposite directions according to move.direction

            int dir = (i % 2) * 2 - 1;
            int a = i % 3;
            if (a != 0) a = 1;

            rcube::Center* tmp = viewpoint[{affectedAxis[a], dir}];
            viewpoint[{affectedAxis[a], dir}] =
                viewpoint[{affectedAxis[(a + 1) % 2], move.direction * dir}];
            viewpoint[{affectedAxis[(a + 1) % 2], move.direction * dir}] = tmp;
        }
    }
}

void rcube::Cube::rotateLayer(const rcube::Move& move)
{
    rcube::Orientation o = move.getAffectedFace();
    int step = o.direction * move.direction;

    for (int i = 0; i < 12; ++i)
    {
        if (edges[i].location.coords[o.axis] == o.direction)
        {
            edges[i].location.rotate(o.axis, step);
            for (int k = 0; k < 2; ++k)
            {
                if (edges[i].stickers[k].orientation == o) continue;

                edges[i].stickers[k].orientation.rotate(o.axis, step);
                edges[i].stickers[k].face =
                    viewpoint[edges[i].stickers[k].orientation];
            }
        }

        if (i >=8) continue;

        if (corners[i].location.coords[o.axis] == o.direction)
        {
            corners[i].location.rotate(o.axis, step);
            for (int k = 0; k < 3; ++k)
            {
                if (corners[i].stickers[k].orientation == o) continue; 

                corners[i].stickers[k].orientation.rotate(o.axis, step);
                corners[i].stickers[k].face =
                    viewpoint[corners[i].stickers[k].orientation];
            }
        }
    }
}

void rcube::Cube::performAlgorithm (const rcube::Algorithm& algorithm)
{
    for (rcube::Move move : algorithm.algorithm)
        performMove(move);
}

rcube::Net rcube::Cube::netRender()
{
    rcube::Net net; // to return

    std::map<rcube::Center*, int> faceMapping; // face: index in net.faces
    for (int a = 0, d = 1; a < 3; d *= -1)
    {
        rcube::Orientation o = {static_cast<Axis>(a), d};
        rcube::Center* center = viewpoint[o];
        int index = faceMapping.size();

        faceMapping[center] = index;

        // insert center
        net.faces[index].stickers.insert(pairCoords2DColor(
            rcube::Coordinates2D(rcube::Coordinates(o), o),
            center->color
        ));

        if (d == -1) a++;
    }

    for (int i = 0; i < 12; ++i)
    {
        // edges
        rcube::Coordinates coords3D = edges[i].location;

        for (int k = 0; k < 2; ++k) // iterate through stickers
        {
            int index = faceMapping[edges[i].stickers[k].face];
            rcube::Orientation o = edges[i].stickers[k].orientation;

            net.faces[index].stickers.insert(pairCoords2DColor(
                rcube::Coordinates2D(coords3D, o),
                edges[i].stickers[k].color
            ));
        }

        if (i >= 8) continue;

        // corners
        coords3D = corners[i].location;

        for (int k = 0; k < 3; ++k) // iterate through stickers
        {
            int index = faceMapping[corners[i].stickers[k].face];
            rcube::Orientation o = corners[i].stickers[k].orientation;

            net.faces[index].stickers.insert(pairCoords2DColor(
                rcube::Coordinates2D(coords3D, o),
                corners[i].stickers[k].color
            ));
        }
    }

    return net;
}

rcube::BlockArray rcube::Cube::blockRender()
{
    rcube::BlockArray pattern; // to return
    
    int patternIndex = 0;

    for (int a = 0, d = -1; a < 3; d *= -1) // centers
    {
        rcube::Orientation o {static_cast<Axis>(a), d};
        pattern.blocks[patternIndex] = rcube::Block {rcube::Coordinates(o), {}};

        pattern.blocks[patternIndex].stickers.insert(pairOrientColor(
            o,
            viewpoint[o]->color
        ));
        
        patternIndex++;
        if (d == 1) a++;
    }

    for (int i = 0; i < 12; ++i)
    {
        // edges
        pattern.blocks[patternIndex] = rcube::Block {edges[i].location, {}};
        for (int k = 0; k < 2; ++k)
        {
            pattern.blocks[patternIndex].stickers.insert(pairOrientColor(
                edges[i].stickers[k].orientation,
                edges[i].stickers[k].color
            ));
        }
        patternIndex++;

        if (i >= 8) continue;

        // corners
        pattern.blocks[patternIndex] = rcube::Block {corners[i].location, {}};
        for (int k = 0; k < 3; ++k)
        {
            pattern.blocks[patternIndex].stickers.insert(pairOrientColor(
                corners[i].stickers[k].orientation,
                corners[i].stickers[k].color
            ));
        }
        patternIndex++;
    }
    assert(patternIndex == 26);

    return pattern;
}