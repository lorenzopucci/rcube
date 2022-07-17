#include <map>
#include <vector>
#include <string>

#include <utility.hpp>

#ifndef RCUBE
#define RCUBE

namespace rcube {
    enum Axis
    {
        X = 0, Y = 1, Z = 2
    };

    struct Face
    {
        Face(char stdNotation);
        Face(rcube::Axis axis, int direction);
        rcube::Axis axis;
        int direction; // eithher +1 or -1
        char getStdNotation;
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

        ~Move();

        std::string to_string();

	MoveFace face; // the face to rotate (expressed by number)
        MoveDirection direction; // the direction of the rotation
    };

    class Algorithm {

    public:
        Algorithm (std::string fromString);
        /*
        * This class represents a set of rotations of a cube's face.
        * @param fromString: the algorithm expressed as string e.g. "RUR'U'"
        */

        Algorithm (std::vector<rcube::Move> fromVector);
        /*
        * This class represents a set of rotations of a cube's face.
        * @param fromVector: the algorithm expressed as vector of moves
        */

        std::string to_string();
        /*
        * Returns a suitable string for being printed.
        */

        ~Algorithm();

        std::vector<rcube::Move> algorithm;
};

    /*
     * These structs are designed for internal use, only. However, if you want
     * to use them, they are quite self-explanatory.
    */
    struct Orientation
    {
        rcube::Axis axis;
        int direction;
    };
    struct Center {
        char color;
    };

    struct Sticker {
        Color color;
        rcube::Center* face;
        Orientation orientation;
    };

    struct Edge {
        rcube::Sticker stickers[2];
    };

    struct Corner {
        rcube::Sticker stickers[3];
    };

    struct AdjacentCenters {
        rcube::Center* centers[4];
    };

    struct Block
    {
        std::map<Orientation, Color> stickers; 
    };

    class Cube {
    public:
        Cube (Color topColor = WHITE, Color frontColor = GREEN);
        /*
         * Initializes a solved cube with the yellow face on top and the green
         * one in front.
        */
        ~Cube() = default;

        void performMove (const rcube::Move& move);
        /*
         * Performs either a rotation of a cube's face or changes the
         * viewpoint.
         * @param move: the move to perform (refer to the Move class below)
        */

        void performAlgorithm (const rcube::Algorithm& algorithm);
        /*
         * Performs an algorithm (set of moves).
         * @param algorithm: the algorithm to perform (refer to the Algorithm
         * class below)
        */

        void scramble (int lenght = 12, rcube::Algorithm* dest = nullptr);
        /*
         * Performs a randomly generated algorithm.
         * @param length: the number of moves in the algorithm (default=12)
         * @param dest: pointer to an empty algorithm that will received the
         * generated algorithm (default=nullptr)
        */

        char** netRender();
        /*
         * Renders the cube as a 2D array.
        */

        rcube::Block** blockRender();
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
	//int getCenterIndex (char color);
        //rcube::Center* getCenterPtr (char color);
        //void displayRaw ();
    };
};

#endif
