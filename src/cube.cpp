#include <map>
#include <set>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <iterator>

#include <rcube.hpp>
#include <utility.hpp>

std::map<char, int> moveToFace = {
    {UP, TOP_FACE},
    {DOWN, BOTTOM_FACE},
    {FRONT, FRONT_FACE},
    {BACK, BACK_FACE},
    {RIGHT, RIGHT_FACE},
    {LEFT, LEFT_FACE}
};

int rcube::Cube::getCenterIndex (char color)
{
    for (int index = 0; index < 6; ++index)
    {
        if (centers[index].color == color)
            return index;
    }
}

rcube::Center* rcube::Cube::getCenterPtr (char color)
{
    return &centers[getCenterIndex(color)];
}

std::string printColorized (char color)
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
    char** toDisplay = this->render();
    for (int i = 0; i < 6; ++i)
    {
        std::cout << "\nFace at index: " << i << std::endl;

        for (int k = 0; k < 9;)
        {
            int limit = k + 3;
            for (; k < limit; ++k)
            {
                std::cout << printColorized(*(*(toDisplay + i) + k)) << " ";
            }
        }
        std::cout << std::endl;
    }
}
/*
void rcube::Cube::displayRaw ()
{
    std::cout << "############ Centers are:" << std::endl;
    for (int i = 0; i < 6; ++i)
    {
        std::cout << printColorized(centers[i].color) << " \n";
    }
    std::cout << "\n############ Edges are:" << std::endl;
    for (int i = 0; i < 12; ++i)
    {
        std::cout << printColorized(edges[i].stickers[0].color) << " -> " << 
        printColorized(edges[i].stickers[0].face->color) << "\n";
        std::cout << printColorized(edges[i].stickers[1].color) << " -> " << 
        printColorized(edges[i].stickers[1].face->color) << "\n\n";
    }
    std::cout << "\n############ Corners are:" << std::endl;
    for (int i = 0; i < 8; ++i)
    {
        std::cout << printColorized(corners[i].stickers[0].color) << " -> " << 
        printColorized(corners[i].stickers[0].face->color) << "\n";
        std::cout << printColorized(corners[i].stickers[1].color) << " -> " << 
        printColorized(corners[i].stickers[1].face->color) << "\n";
        std::cout << printColorized(corners[i].stickers[2].color) << " -> " << 
        printColorized(corners[i].stickers[2].face->color) << "\n\n";
    }
}
*/

rcube::Cube::Cube(Color topColor, Color frontColor)
{
    // Initialises the cube by assigning a value to:
    // - edges, centers and corners
    // - viewpoint
    // - adjacentCentersPtr
    
    // template
    CenterIndices adjacentCenters[6][4] = {
        {GREEN_CENTER, ORANGE_CENTER, BLUE_CENTER, RED_CENTER},
        {GREEN_CENTER, RED_CENTER, BLUE_CENTER, ORANGE_CENTER},
        {WHITE_CENTER, RED_CENTER, YELLOW_CENTER, ORANGE_CENTER},
        {WHITE_CENTER, ORANGE_CENTER, YELLOW_CENTER, RED_CENTER},
        {WHITE_CENTER, BLUE_CENTER, YELLOW_CENTER, GREEN_CENTER},
        {WHITE_CENTER, GREEN_CENTER, YELLOW_CENTER, BLUE_CENTER}
    };

    for (int i = 0; i < 6; ++i)
        centers[i] = (rcube::Center) {(char)static_cast<Color>(i)}

    for (int i = 0; i < 6; ++i)
    {
        std::pair<rcube::Center*, rcube::AdjacentCenters> toAdd(
            &centers[i],
            (rcube::AdjacentCenters){}
        );
        for (int x = 0; x < 4; ++x)
        {
            toAdd.second.centers[x] = &centers[adjacentCenters[i][x]];
        }
        adjacentCentersPtr.insert(toAdd);
    }

    viewpoint.insert(std::pair<rcube::Orientation, rcube::Center*> (
        (rcube::Orientation) {X, 1},
        getCenterPtr(topColor)
    ));
    viewpoint.insert(std::pair<rcube::Orientation, rcube::Center*> (
        (rcube::Orientation) {Z, 1},
        getCenterPtr(frontColor)
    ));
    
}

rcube::Cube::Cube (Color topColor, Color frontColor)
{
    /*
     * Initialises the cube by assigning a value to:
     * - edges and corners
     * - viewpoint
     * - adjacentCentersPtr
    */

    // template
    int adjacentCenters[6][4] = {
        {GREEN_CENTER, ORANGE_CENTER, BLUE_CENTER, RED_CENTER},
        {GREEN_CENTER, RED_CENTER, BLUE_CENTER, ORANGE_CENTER},
        {WHITE_CENTER, RED_CENTER, YELLOW_CENTER, ORANGE_CENTER},
        {WHITE_CENTER, ORANGE_CENTER, YELLOW_CENTER, RED_CENTER},
        {WHITE_CENTER, BLUE_CENTER, YELLOW_CENTER, GREEN_CENTER},
        {WHITE_CENTER, GREEN_CENTER, YELLOW_CENTER, BLUE_CENTER}
    };

    // initialize centers
    char colorsList[6] = {WHITE, YELLOW, GREEN, BLUE, RED, ORANGE};
    for (int i = 0; i < 6; ++i)
        centers[i] = (rcube::Center) {colorsList[i]};

    // convert template to adjacentCentersPtr
    Center* tmp[4];
    for (int i = 0; i < 6; ++i)
    {
        std::pair<rcube::Center*, rcube::AdjacentCenters> toAdd (
            &centers[i],
            (rcube::AdjacentCenters) {}
        );
        for (int k = 0; k < 4; ++k)
        {
            toAdd.second.centers[k] = &centers[adjacentCenters[i][k]];
        }
        adjacentCentersPtr.insert(toAdd);
    }

    // assign value to viewpoint
    viewpoint[TOP_FACE] = getCenterPtr(topColor);
    viewpoint[FRONT_FACE] = getCenterPtr(frontColor);
    rcube::AdjacentCenters adjacentToFront = adjacentCentersPtr[viewpoint[FRONT_FACE]];

    for (int i = 0; i < 4; ++i)
    {
        if (adjacentToFront.centers[i]->color == topColor)
        {
            viewpoint[BOTTOM_FACE] = adjacentToFront.centers[(i + 2) % 4];
            break;
        }
    }

    rcube::AdjacentCenters adjacentToBottom = adjacentCentersPtr[viewpoint[BOTTOM_FACE]];
    for (int i = 0;; ++i)
    {
        if (adjacentToBottom.centers[i] == viewpoint[FRONT_FACE])
        {
            for (int k = 0; k < 4; ++k)
            {
                viewpoint[k + 1] = adjacentToBottom.centers[(i + k + 3) % 4];
            }
            break;
        }
    }

    // create edges and corners
    int edgesIndex = 0; // index in edges array
    int cornersIndex = 0; // index in corner array
    for (int i = 1; i < 5; ++i)
    {
        for (int k = 0; k < 6; k += 5) // both top and bottom
        {
            Edge edge;
            edge.stickers[0] = (rcube::Sticker){viewpoint[k]->color, viewpoint[k]};
            edge.stickers[1] = (rcube::Sticker){viewpoint[i]->color, viewpoint[i]};
            edges[edgesIndex] = edge;
            edgesIndex++;

            Corner corner;
            corner.stickers[0] = (rcube::Sticker){viewpoint[k]->color, viewpoint[k]};
            corner.stickers[1] = (rcube::Sticker){viewpoint[i]->color, viewpoint[i]};
            int index = 1 + (i % 4);
            corner.stickers[2] = (rcube::Sticker){viewpoint[index]->color, viewpoint[index]};
            corners[cornersIndex] = corner;
            ++cornersIndex;
        }

        Edge edge;
        edge.stickers[0] = (rcube::Sticker){viewpoint[i]->color, viewpoint[i]};
        int index = 1 + (i % 4);
        edge.stickers[1] = (rcube::Sticker){viewpoint[index]->color, viewpoint[index]};
        edges[edgesIndex] = edge;
        edgesIndex++;
    }
}

void rcube::Cube::changeViewpoint (rcube::Move* move)
{
    int intactFace; // they are 2 actually, but knowing one of them is enough
    switch (move->face)
    {
        case ROTATE_X:
            intactFace = RIGHT_FACE; break;
        case ROTATE_Y:
            intactFace = TOP_FACE; break;
        case ROTATE_Z:
            intactFace = FRONT_FACE; break;
        default: return;
    }
    rcube::Center** centersToSwap = adjacentCentersPtr[viewpoint[intactFace]
        ].centers;

    int swappingIndexes[4]; // indexes of centersToSwap in viewpoint
    for (int i = 0; i < 4; ++i)
    {
        for (int k = 0; k < 6; ++k)
        {
            if (viewpoint[k] == *(centersToSwap + i))
            {
                swappingIndexes[i] = k;
                break;
            }
        }
    }

    // swap centers
    for (int i = 0; i < 4; ++i)
    {
        int index = (i - move->direction + 4) % 4;
        viewpoint[swappingIndexes[i]] = *(centersToSwap + index);
    }
}

void performMultipliedAlgo (
    rcube::Algorithm* base,
    rcube::Cube* instance,
    int factor)
{
    // multiply algorithm (repeat it factor times)
    // note: only tested with factor = {-1, 1, 2}
    for (auto it = base->algorithm.begin(); it < base->algorithm.end(); ++it)
    {
        it->direction = it->direction * factor;
        if (it->direction == -2)
            it->direction = 2;
        if (it->direction == 4)
            base->algorithm.erase(it);
    }

    instance->performAlgorithm(base);
}

void rcube::Cube::performMove (rcube::Move* move)
{
    switch (move->face)
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
            performMultipliedAlgo (new Algorithm("RL'x'"), this, move->direction);
            return;
        case EQUATOR:
            performMultipliedAlgo (new Algorithm("UD'y'"), this, move->direction);
            return;
        case SIDE:
            performMultipliedAlgo (new Algorithm("F'Bz"), this, move->direction);
            return;
    }
}

void rcube::Cube::rotateLayer (rcube::Move* move)
{
    rcube::Center* face = viewpoint[moveToFace[move->face]];
    rcube::AdjacentCenters adjCenters = adjacentCentersPtr[face];

    for (int i = 0; i < 12; ++i) // iterate through edges
    {
        for (int k = 0; k < 2; ++k) // iterate through stickers
        {
            if (edges[i].stickers[k].face != face) continue;

            int x = (k + 1) % 2; // get the other sticker
            for (int y = 0; y < 4; ++y)
            {
                if (adjCenters.centers[y] == edges[i].stickers[x].face)
                {
                    edges[i].stickers[x].face = adjCenters
                        .centers[(y + move->direction) % 4];
                }
            }
        }
    }

    for (int i = 0; i < 8; ++i) // iterate through corners
    {
        for (int k = 0; k < 3; ++k) // iterate through stickers
        {
            if (edges[i].stickers[k].face != face) continue;

            // get other two stickers
            for (int x = (k + 1) % 3; x != k; x = (x + 1) % 3)
            {
                for (int y = 0; y < 4; ++y)
                {
                    if (adjCenters.centers[y] == edges[i].stickers[x].face)
                    {
                        edges[i].stickers[x].face = adjCenters
                            .centers[(y + move->direction) % 4];
                    }
                }
            }
        }
    }
}

void rcube::Cube::performAlgorithm (rcube::Algorithm* algorithm)
{
    for (rcube::Move move : algorithm->algorithm)
        this->performMove(&move);
}

char** rcube::Cube::render ()
{
    /*
     * This 150-lines function has been a hassle to implement. Basically,
     * stickers are first put in a table representing the the 2D net of the cube,
     * then this table is split into different faces.
    */
    char tmpTable[7][12];
    std::set<rcube::Center*> outerCenters;

    // put centers in place
    for (int i = 0; i < 6; ++i)
    {
        if (i == TOP_FACE || i == BOTTOM_FACE)
        {
            outerCenters.insert(viewpoint[i]);

            for (int k = 1; k < 11; k += 3)
            {
                tmpTable[i*6/5][k] = viewpoint[i]->color;
                tmpTable[i*6/5][k-1] = '0';
                tmpTable[i*6/5][k+1] = '0';
            }
        }
        else
        {
            tmpTable[3][(i*3)-2] = viewpoint[i]->color;
        }
    }

    // put edges in place
    for (int i = 0; i < 12; ++i)
    {
        int stickerIndex = 0;
        if (outerCenters.find(edges[i].stickers[stickerIndex].face) !=
            outerCenters.end())
            ++stickerIndex;
        
        char center1 = edges[i].stickers[stickerIndex].face->color;
        char center2 = edges[i].stickers[(stickerIndex + 1) % 2].face->color;
        char sticker1 = edges[i].stickers[stickerIndex].color;
        char sticker2 = edges[i].stickers[(stickerIndex + 1) % 2].color;

        // locate center1
        int column;
        for (column = 1; column < 12; column = column + 3)
        {
            if (tmpTable[3][column] == center1) break;
        }

        // center2 is on the same column as center1. If so, handle it
        for (int k = 0; k < 7; k = (k + 1) * 6)
        {
            if (tmpTable[k][1] == center2)
            {
                tmpTable[abs(k - 2)][column] = sticker1;
                tmpTable[abs(k - 1)][column] = sticker2;
                goto nextEdge;
            }
        }

        // handle the second case (center2 is on the same row as center1)
        for (int k = -1; k < 2; k = k + 2)
        {
            int column2 = (column + (k * 3) + 12) % 12;

            if (tmpTable[3][column2] == center2)
            {
                tmpTable[3][column+k] = sticker1;
                tmpTable[3][column2-k] = sticker2;
                goto nextEdge;
            }
        }
    nextEdge:;
    }

    // put corners in place
    for (int i = 0; i < 8; ++i)
    {
        int stickerIndex = 0;
        while (outerCenters.find(corners[i].stickers[stickerIndex].face) ==
            outerCenters.end())
            ++stickerIndex;
        

        char cornerCenters[3]; // first element is in outer face
        char cornerStickers[3]; // first element is in outer face
        for (int k = 0; k < 3; ++k)
        {
            cornerCenters[k] = corners[i].stickers[(stickerIndex + k) % 3]
                .face->color;
            cornerStickers[k] = corners[i].stickers[(stickerIndex + k) % 3]
                .color;
        }

        // locate centers corresponding to stickers at index 1 and 2
        int columns[2];
        for (int k = 0; k < 2; ++k)
        {
            for (columns[k] = 1; columns[k] < 12; columns[k] += 3)
            {
                if (tmpTable[3][columns[k]] == cornerCenters[k + 1]) break;
            }
        }

        // swap stickers at index 1 and 2 according to their position
        if (columns[2] < columns[1])
        {
            int intTmp;
            char charTmp;

            intTmp = columns[1];
            columns[1] = columns[2];
            columns[2] = intTmp;

            charTmp = cornerStickers[1];
            cornerStickers[1] = cornerStickers[2];
            cornerStickers[2] = charTmp;
        }

        // center2 is on the same column as center1. If so, handle it
        for (int k = 0; k < 7; k = (k + 1) * 6)
        {
            if (tmpTable[k][1] == cornerCenters[0])
            {
                tmpTable[abs(k - 2)][columns[0]+1] = cornerStickers[1];
                tmpTable[abs(k - 2)][columns[1]-1] = cornerStickers[2];
                tmpTable[abs(k - 1)][columns[0]+1] = cornerStickers[0];
                tmpTable[abs(k - 1)][columns[1]-1] = cornerStickers[0];
            }
        }
    }
/*
    std::cout << "\n\n";
    for (int y = 0; y < 7; ++y)
    {
        for (int x = 0; x < 12; ++x)
        {
            if ((y == 0 || y == 5) && (x - 1) % 3 != 0)
                std::cout << " ";
            std::cout << printColorized(tmpTable[y][x]);
        }
        std::cout << std::endl;
    }
*/
    // lastly, divide the table into single faces
    char** pattern; // to return
    pattern = (char**)malloc(sizeof(char*) * 6);
    for (int i = 0; i < 6 ; ++i) {
        pattern[i] = new char[9];
    }

    for (int i = 1; i < 5; ++i) // non-outer faces
    {
        for (int k = 0; k < 9; ++k)
        {
            int row = 2 + (int)k/3;
            int column = (i - 1)*3 + (k % 3);
            *(pattern[i] + k) = tmpTable[row][column];
        }
    }

    // outer faces
    pattern[0] = new char[9] {
        tmpTable[1][11], tmpTable[1][10], tmpTable[1][9],
        tmpTable[1][1], tmpTable[0][1], tmpTable[1][7],
        tmpTable[1][3], tmpTable[1][4], tmpTable[1][5]
    };
    pattern[5] = new char[9] {
        tmpTable[5][3], tmpTable[5][4], tmpTable[5][5],
        tmpTable[5][1], tmpTable[6][1], tmpTable[5][7],
        tmpTable[5][11], tmpTable[5][10], tmpTable[5][9],
    };

    return pattern;
}

rcube::Cube::~Cube ()
{
    return;
}
