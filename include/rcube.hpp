/*
* Copyright (c) 2022 Lorenzo Pucci
* You may use, distribute and modify this code under the terms of the MIT
* license.
*
* You should have received a copy of the MIT license along with this program. If
* not, see: <https://mit-license.org>.
*/

#include <map>
#include <vector>
#include <string>

#include <utility.hpp>

#ifndef RCUBE
#define RCUBE

namespace rcube
{
    struct Orientation
    {
        /*
        * This struct represents an orientation, which consists of an axis
        * (X, Y or Z) and a sign (+1 or -1). Orientations are used to identify
        * faces (for instance, the top face has the orientation +Y) and thus
        * understand which face a sticker belongs to.
        */
        Axis axis;
        int direction; // +1 or -1

        bool operator<(const rcube::Orientation& a) const;
        bool operator==(const rcube::Orientation& a) const;
        bool operator!=(const rcube::Orientation& a) const;

        void rotate(const Axis& axis, int step);
        /*
        * Rotate around axis of step * 90°
        * @param axis: the rotation axis
        * @param step: the number of 90° rotations to apply (clockwise relatively
        * to someone looking at the origin from the positive side of the axis)
        */

       rcube::Orientation getRotated(const Axis &axis, int step) const;
       /*
        * The same as Orientation::rotate, but the new orientation is returned
        * rather than applied to the current instance.
        */

       static std::vector<rcube::Orientation> iterate();
       /*
       * Returns a vector containing all 6 possible orientations in the following
       * order: -X, +X, -Y, +Y, -Z, +Z
       * This is used for iteration through objects like rcube::Net
       */
    };

    struct Coordinates
    {
        /*
        * This struct is used to represent the 3D coordinates of the cube's
        * blocks. Therefore, each coordinate is only meant to be -1, 0 or 1.
        */

        Coordinates() = default;

        Coordinates(const int& x, const int& y, const int& z);
        /*
        * Default constructor: the values are directly assigned to coords
        */

        Coordinates(const rcube::Orientation& o1);
        /*
        * Constructor for centers: the resulting coordinates are the ones of the
        * center of the face at orientation o1
        */

        Coordinates(const rcube::Orientation& o1, const rcube::Orientation& o2);
        /*
        * Constructor for edges: the resulting coordinates are the ones of the
        * edge that sits between the two faces at orientation o1 and o2
        */

        Coordinates(const rcube::Orientation& o1, const rcube::Orientation& o2,
            const rcube::Orientation& o3);
        /*
        * Constructor for corners: the resulting coordinates are the ones of the
        * corner that sits between the three faces at orientation o1, o2 and o3
        */
        
        int coords[3];
        /*
        * The values are stored like this to make it easier to iterate through
        * them. They can be accessed using the corresponding int values of the
        * axes (see the Axis enum in include/utility.hpp) like coords[Axis::X]
        */

        inline int x() const { return coords[Axis::X]; }
        inline int y() const { return coords[Axis::Y]; }
        inline int z() const { return coords[Axis::Z]; }
        /*
        * These three functions return the x, y or z values.
        */

        bool operator==(const rcube::Coordinates &a) const;
        bool operator!=(const rcube::Coordinates &a) const;

        std::string to_string() const;

        void rotate(const Axis& axis, int step);
        /*
        * Rotate around axis of step * 90°
        * @param axis: the rotation axis
        * @param step: the number of 90° rotations to apply (clockwise relatively
        * to someone looking at the origin from the positive side of the axis)
        */
    };

    struct Coordinates2D
    {
        /*
        * This struct is used the represent the 2D coordinates of a sticker
        * relatively to its face. It is only used for net renderings
        * (see rcube::Face).
        */

        Coordinates2D();
        ~Coordinates2D() = default;

        Coordinates2D(const int& x, const int& y);
        /*
        * Default constructor: the values are directly assigned to coords
        */
        
        Coordinates2D(const rcube::Coordinates& coords3D,
            const rcube::Orientation& o);
        /*
        * The resulting coordinates are the ones of the sticker with orientation
        * o belonging to the block placed coords3D in the 3D space.
        * There is no control over where the 2D axes will end up being: the
        * resulting face may not be rotated to the most intuitive position.
        */

        int coords[2];
        /*
        * The coordinates are stored as in rcube::Coordinates
        */

        inline int x() const { return coords[Axis::X]; }
        inline int y() const { return coords[Axis::Y]; }
        /*
        * These three functions return the x or y values.
        */

        bool operator<(const rcube::Coordinates2D& a) const;
        bool operator==(const rcube::Coordinates2D& a) const;

        void rotate(int step);
        /*
        * Rotates around (0;0) of step * 90°
        * @param step: the number of 90° rotations to apply (positive =
        * anticlockwise)
        */
    };


    // the following structs are meant for internal use only
    struct Sticker
    {
        // A single colored tile of the cube
        Color color;
        rcube::Orientation orientation;
    };
    struct PieceTemplate
    {
        rcube::Coordinates location = rcube::Coordinates(0, 0, 0);
    };

    // Center, Edge and Corner are the three type of blocks the cube is made of
    struct Center : PieceTemplate, Sticker
    {
        Center() = default;
        Center(const Color &color, const rcube::Orientation &o);
        // the coordinates are worked out by one of the constructors of
        // rcube::Coordinates
    };
    struct Edge : PieceTemplate
    {
        rcube::Sticker stickers[2];
    };
    struct Corner : PieceTemplate
    {
        rcube::Sticker stickers[3];
    };


    // the following structs are the results of the various renderings
    struct Block
    {
        /*
        * This struct represents an unspecified block of the cube (center, edge
        * or corner). It is used for each block of rcube::BlockArray
        */

        rcube::Coordinates position = rcube::Coordinates(0, 0, 0);

        std::map<rcube::Orientation, Color> stickers;
        /*
        * The stickers are stored in a map along with their orientation. The
        * map does not contain all the possibile orientations but only the ones
        * where a sticker is.
        */
    };
    struct BlockArray
    {
        /*
        * This struct represent a full cube divided into single blocks. All the
        * 26 blocks are stored as a rcube::Block. This is the result of
        * rcube::Cube::blockRender().
        */
        rcube::Block blocks[26];
    };

    struct Face
    {
        /*
        * This struct represent a single face of the cube. Each of the 9 stickers
        * is stored in a map along with its 2D coordinates. It is used for each
        * face of rcube::Net
        */
        std::map<rcube::Coordinates2D, Color> stickers;
    };
    struct Net
    {
        /*
        * This struct represents the 2D net of the cube. Each of the 6 faces is
        * stored as a rcube::Face in a map along with its orientation. This is
        * the result of rcube::Cube::netRender()
        */
        std::map<rcube::Orientation, rcube::Face> faces;
    };


    class Move {
        /*
        * This class represent a single cube's move, which can either be:
        * - the rotation of a layer (R, L, U, D, F, B, M, E or S)
        * - the rotation of the whole cube in the 3D space (x, y or z)
        * Each of these moves can be performed clockwise, conterclockwise or
        * twice. This property is also a class member.
        * 
        * To apply a move, use it as the argument of rcube::Cube::performMove().
        */
    public:

        Move(MoveFace face, MoveDirection direction);
        /*
        * The values provided are directly assigned to this->face and
        * this->direction
        * @param face: the move to perform (see the MoveFace enum in
        * include/utility.hpp)
        * @param direction: the rotation direction (see the MoveDirection enum
        * in include/utility.hpp)
        * 
        * If you want to use the standard notation (like "U'" for rotating the top
        * face conterclockwise), use the rcube::Algorithm class instead.
        */

	    Move(char face, int direction);
        /*
        * This constructor is the same as the one before, but the values are
        * first provided as a char and an int, then converted into MoveFace and
        * MoveDirection.
        */

        ~Move() = default;

        std::string to_string() const;
        /*
        * Returns the move as a string in the standard notation (like "F'"
        * or "U2").
        */

        rcube::Orientation getAffectedFace() const;
        /*
        * Returns the orientation of the face that the move applies on. This can
        * only be used for the folllowing moves: R, L, U, D, F, B
        */

	    MoveFace face; // the kind of move to perform
        MoveDirection direction; // the direction of the rotation
        Axis axis; // the rotation axis
    };

    class Algorithm {
        /*
        * This class represents an ordered set of moves.
        */

    public:
        Algorithm() = default;

        Algorithm (const std::string& fromString);
        /*
        * The algorithm is generated from a string in the default notation (like
        * "RUR'U'"). Characters '(', ')', '[', ']', spaces and tabs are also
        * allowed (they will be ignored).
        * 
        * NOTE: this constructor may throw a std::invalid_argument exception
        * if the string contains an invalid character.
        */

        Algorithm (const std::vector<rcube::Move>& fromVector);
        /*
        * The algorithm is provided from a std::vector of rcube::Move.
        */

        ~Algorithm() = default;

        rcube::Algorithm operator*(const int& factor);
        /*
        * Multiplies the algorithm by a factor. The algorithm is NOT repeated
        * factor times, but all the directions of the moves are multiplied by it.
        * NOTE: only designed and tested for factor = {-1, 1, 2}
        */

       rcube::Algorithm operator+(const rcube::Algorithm a);
        /*
        * Concatenates two algorithms
        */

       inline void push(const rcube::Move &m);
        /*
        * Appends a rcube::Move to the algorithm
        */

       static rcube::Algorithm generateScramble(const int &length = 12);
       /*
       * Generates a random algorithm of given length (default=12)
       */

        std::string to_string() const;
        /*
        * Returns the algorithm as a string in the standard notation
        * (e.g. "RUR'U'").
        */

       inline int length() const;
       /*
       * Returns the length of the algorithm (number of moves).
       */

        std::vector<rcube::Move> algorithm;
        // the algorithm is stored as a std::vector of rcube::Move
    };

    class Cube {
        /*
        * This class represents a 3*3*3 Rubik's cube. After the cube is
        * initialized, the functions performMove and performAlgorithm are used to
        * apply moves and algorithm. The current status of the cube can then be
        * retrieved by rendering either a rcube::Net or a rcube::BlockArray
        */
    public:

        Cube (const Color& topColor = Color::White,
            const Color& frontColor = Color::Green);
        /*
        * Initializes a solved cube with the <topColor> face on top and the
        * <frontColor> one in front.
        * 
        * NOTE: this function may throw a `std::invalid_argument` exception if
        * topColor and frontColor are not two adjacent colors.
        */

        ~Cube() = default;

        void performMove (const rcube::Move& move);
        /*
         * Applies a rcube::Move
        */

        void performAlgorithm (const rcube::Algorithm& algorithm);
        /*
         * Applies a rcube::Algorithm
        */

        void scramble(const int &length = 12, rcube::Algorithm* dest = nullptr);
        /*
         * Performs a randomly generated algorithm.
         * @param length: the number of moves in the algorithm (default=12)
         * @param dest: pointer to an empty algorithm that will receive the
         * generated algorithm (default=nullptr)
        */

        bool isSolved();
        /*
        * Returns true when the cube is solved (all the blocks are in the right
        * place).
        */

        void rotateTo(const Color& topColor, const Color& frontColor);
        /*
        * Rotates the cube so as topColor and frontColor become the colors of
        * the top and front center respectively.
        * 
        * NOTE: this function may throw a `std::invalid_argument` exception if
        * topColor and frontColor are not two adjacent colors.
        */

       rcube::Coordinates find(const Color& c1);
        /*
        * Returns the coordinates of a center given its color.
        */

        rcube::Coordinates find(const Color& c1, const Color &c2);
        /*
        * Returns the coordinates of an edge given the colors of its stickers.
        *
        * NOTE: this function may throw a std::invalid_argument exception if
        * the two colors are not adjacent.
        */

        rcube::Coordinates find(const Color& c1, const Color &c2,
            const Color &c3);
        /*
        * Returns the coordinates of a corner given the colors of its stickers.
        *
        * NOTE: this function may throw a std::invalid_argument exception if
        * the three colors cannot form a corner.
        */

        rcube::Orientation getStickerOrientation(const rcube::Coordinates
            &coords, const Color &color);
        /*
        * Returns the orientation of the sticker of a given color belonging to
        * the block at given coordinates.
        * 
        * NOTE: this function may throw a std::invalid_argument exception if
        * there is no sticker of such color in the block.
        */

        bool faceMatches(const rcube::Orientation &face,
            const std::string &expr);
        /*
        * Checks if a face matches a pattern. This is one of the most powerful
        * tools of this library, please refer to the documentation for a thorough
        * understanding.
        * @param face: the face to check
        * @param expr: the pattern (its syntax is explained in the documentation)
        */

        bool layerMatches(const rcube::Orientation &layer,
            const std::string &expr);
        /*
        * Checks if a layer (the 12 stickers adjacent to the same face) matches
        * a pattern.
        * @param layer: the layer to check (expressed as the orientation of the
        * adjacent face). Use Orientation::axis=0 for central layers (for example
        * {X, 0} is the layer that is rotated by a M move).
        * @param expr: the pattern (its syntax is explained in the documentation)
        */

        rcube::Net netRender();
        /*
         * Renders the cube as an array of faces (see rcube::Net).
        */

        rcube::BlockArray blockRender();
        /*
         * Renders the cube as an array of blocks (see rcube::Blockarray).
        */

        void display ();
        /*
        * Prints the cube to stdout. This output is quite rough (mostly used for
        * debugging purposes): each face is printed individually as a 3*3 grid
        * of characters indicating the color (with the corresponging color in the
        * background).
        */

        int runScript(const std::string &path);
        /*
        * Runs a Lua script. All the C++ functions that have been ported to Lua
        * are listed in the documentation.
        * @param path: the path to the file.
        * @return: 1 on success, 0 on failure
        */

        int runCommand(const std::string &cmd);
        /*
        * Runs one line of Lua code. All the C++ functions that have been ported
        * to Lua are listed in the documentation.
        * @param cmd: the code to execute.
        * @return: 1 on success, 0 on failure
        */

    private:
        // internally, the cube is abstracted as an array of 6 rcube::Center, 12
        // rcube::Edge and 8 rcube::Corner
        rcube::Center centers[6];
        rcube::Edge edges[12];
        rcube::Corner corners[8];

        void changeViewpoint (const rcube::Move& move);
        // handle the moves x, y and z

        void rotateLayer (const rcube::Move& move);
        // handle the moves R, L, U, D, F, B

        rcube::Center* getCenterFrom(const Color &color);
        // get a pointer to the <color> center

        rcube::Center* getCenterFrom(const rcube::Coordinates &coords);
        // get a pointer to the center placed at <coords>

        rcube::Center* getCenterFrom(const rcube::Orientation &orient);
        // get a pointer to the center placed at <coords>
    };
};

#endif
