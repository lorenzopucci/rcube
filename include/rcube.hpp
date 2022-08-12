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
        Axis axis;
        int direction;

        bool operator<(const rcube::Orientation& a) const;
        bool operator==(const rcube::Orientation& a) const;

        void rotate(const Axis& axis, int step);
        /*
        * Rotate around axis of step * 90°
        * @param axis: the rotation axis
        * @param step: the number of 90° rotations to apply (clockwise relatively
        * to someone looking at the origin from the positive side of the axis)
        */
    };

    struct Coordinates // used to store the position of a corner/edge
    {
        Coordinates(const int& x, const int& y, const int& z);
        Coordinates(const rcube::Orientation& o1); // constructor for centers
        Coordinates(const rcube::Orientation& o1,
            const rcube::Orientation& o2); // constructor for edges
        Coordinates(const rcube::Orientation& o1, const rcube::Orientation& o2,
            const rcube::Orientation& o3); // constructor for corners
        
        int coords[3];

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
        Coordinates2D();

        Coordinates2D(const int& x, const int& y);
        
        // get the coordinates of the sticker with orientation o belonging to
        // the block at coordinates coords3D relatively to its face
        Coordinates2D(const rcube::Coordinates& coords3D,
            const rcube::Orientation& o);

        bool operator<(const rcube::Coordinates2D& a) const;

        bool operator==(const rcube::Coordinates2D& a) const;

        int coords[2];
    };

    struct Center {
        Color color;
    };

    struct Sticker {
        Color color;
        rcube::Center* face;
        rcube::Orientation orientation;
    };

    struct Edge {
        rcube::Sticker stickers[2];
        rcube::Coordinates location = rcube::Coordinates(0, 0, 0);
    };

    struct Corner {
        rcube::Sticker stickers[3];
        rcube::Coordinates location = rcube::Coordinates(0, 0, 0);
    };

    struct AdjacentCenters {
        rcube::Center* centers[4];
    };

    struct Block
    {
        rcube::Coordinates position = rcube::Coordinates(0, 0, 0);
        std::map<rcube::Orientation, Color> stickers;
    };
    struct BlockArray
    {
        rcube::Block blocks[26];
    };

    struct Face
    {
        std::map<rcube::Coordinates2D, Color> stickers;
    };
    struct Net
    {
        rcube::Face faces[6];
    };

    class Move {
    public:
        Move(MoveFace face, MoveDirection direction);
	    Move(char face, int direction);
        /*
        * This class represents a single rotation of a cube's face.
        * @param face: the face to rotate
        * @param direction: the direction of the rotation
        */

        ~Move() = default;

        std::string to_string() const;

        rcube::Orientation getAffectedFace() const;

	    MoveFace face; // the face to rotate
        MoveDirection direction; // the direction of the rotation
        Axis axis; // the rotation axis
    };

    class Algorithm {

    public:
        Algorithm (const std::string& fromString);
        /*
        * This class represents a set of rotations of a cube's face.
        * @param fromString: the algorithm expressed as string e.g. "RUR'U'"
        */

        Algorithm (const std::vector<rcube::Move>& fromVector);
        /*
        * This class represents a set of rotations of a cube's face.
        * @param fromVector: the algorithm expressed as vector of moves
        */

        ~Algorithm() = default;

        rcube::Algorithm operator*(const int& factor);
        /*
        * Multiplies the algorithm by a factor. The algorithm is NOT repeated
        * factor times, but all the directions of the moves are multiplied by it.
        * @param factor: the multiplication factor
        * NOTE: only designed and tested for factor = {-1, 1, 2}
        */

       rcube::Algorithm operator+(const rcube::Algorithm a);
        /*
        * Concatenates two algorithms
        * @param a: the second addend
        */

        std::string to_string();
        /*
        * Returns string in the standard notation (e.g. "RUR'U'").
        */

        std::vector<rcube::Move> algorithm;
    };

    class Cube {
    public:
        Cube (const Color& topColor = Color::White,
            const Color& frontColor = Color::Green);
        /*
         * Initializes a solved cube with the yellow face on top and the green
         * one in front.
        */
        ~Cube() = default;

        void performMove (const rcube::Move& move);
        /*
         * Performs either a rotation of a cube's face or changes the
         * viewpoint.
         * @param move: the move to perform (refer to the Move class above)
        */

        void performAlgorithm (const rcube::Algorithm& algorithm);
        /*
         * Performs an algorithm (set of moves).
         * @param algorithm: the algorithm to perform (refer to the Algorithm
         * class above)
        */

        void scramble (int lenght = 12, rcube::Algorithm* dest = nullptr);
        /*
         * Performs a randomly generated algorithm.
         * @param length: the number of moves in the algorithm (default=12)
         * @param dest: pointer to an empty algorithm that will received the
         * generated algorithm (default=nullptr)
        */

        rcube::Net netRender();
        /*
         * Renders the cube as a 2D array (see rcube::Net)
        */

        rcube::BlockArray blockRender();
        /*
         * Renders the cube as an array of Blocks.
        */

        void display ();

    private:
        rcube::Center centers[6];
        rcube::Edge edges[12];
        rcube::Corner corners[8];
        std::map<Orientation, rcube::Center*> viewpoint;
        std::map<rcube::Center*, rcube::AdjacentCenters> adjacentCentersPtr;
        void changeViewpoint (const rcube::Move& move);
        void rotateLayer (const rcube::Move& move);
	    int getCenterIndex (const Color& color);
        rcube::Center* getCenterPtr (const Color& color);
    };
};

#endif
