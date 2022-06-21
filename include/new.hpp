#include <unordered_map>
#include <vector>
#include <string>

#include <definitions.hpp>

#ifndef RCUBE_CUBE
#define RCUBE_CUBE

namespace rcube {

    class Move {
    public:
        Move(char face, int direction);
        /*
        * This class represents a single rotation of a cube's face.
        * @param face: the face to rotate (expressed by number)
        * @param direction: the direction of the rotation (+1, -1, 2)
        */

        ~Move();

        std::string to_string();

        char face; // the face to rotate (expressed by number)
        int direction; // the direction of the rotation (+1, -1, 2)
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
    

    struct Center {
        char color;
    };

    struct Sticker {
        char color;
        rcube::Center* face;
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

    class Cube {
    /*
     * This class represents a Rubik's cube.
    */
    public:
        Cube (char topColor = WHITE, char frontColor = GREEN);
        /*
         * Initializes a solved cube with the yellow face on top and the green
         * one in front.
        */
        ~Cube ();

        void performMove (rcube::Move* move);
        /*
         * Performs either a rotation of a cube's face or changes the
         * viewpoint.
         * @param move: the move to perform (refer to the Move class below)
        */

        void performAlgorithm (rcube::Algorithm *algorithm);
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

        char** render ();
        /*
         * Returns a 2d array representing the cube. This is a much more
         * convenient way of interfacing with the cube, rather than the
         * unmanageable data structure that is used in the backend.
        */

        void display ();

    private:
        rcube::Center centers[6];
        rcube::Edge edges[12];
        rcube::Corner corners[8];
        std::unordered_map<int, rcube::Center*> viewpoint;
        std::unordered_map<rcube::Center*, rcube::AdjacentCenters> adjacentCentersPtr;
        void changeViewpoint (rcube::Move* move);
        void rotateLayer (rcube::Move* move);
        int getCenterIndex (char color);
        rcube::Center* getCenterPtr (char color);
        //void displayRaw ();
    };
};

#endif
