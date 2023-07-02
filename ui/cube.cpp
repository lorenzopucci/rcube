/*
* Copyright (c) 2023 Lorenzo Pucci
* You may use, distribute and modify this code under the terms of the MIT
* license.
*
* You should have received a copy of the MIT license along with this program. If
* not, see: <https://mit-license.org>.
*/

#include "cube.hpp"


Cube::Cube(rcube::Cube *cube)
{
    this->cube = cube;
    texture = new Texture(STICKER_TXT_PATH);
    this->update();
}

void Cube::update()
{
    rcube::BlockArray blocks = cube->blockRender();

    for (int i = 0; i < 26; ++i)
    {
        rcube::Coordinates pos = blocks.blocks[i].position;

        this->cubies[i] = (Cubie){
            blocks.blocks[i].stickers,
            pos,
            pos,
            glm::vec3(pos.x(), pos.y(), pos.z()),
            glm::vec3(pos.x(), pos.y(), pos.z())
        };
    }
}

float gety0(const rcube::Coordinates &pos, const rcube::Orientation &face)
{
    if (pos.x() * pos.y() * pos.z() != 0) return float(5.0f/6.0f); // corner
    if (abs(pos.x() + pos.y() + pos.z()) == 1) return float(4.0f/6.0f); // center

    // edge
    if (pos.x() == 0)
    {
        if ((face.axis == Axis::Z && pos.y() == 1) || (face.axis == Axis::Y
            && pos.z() == 1)) return 1.0f/6.0f;
        return 0.0f;
    }
    else if (face.axis == Axis::X && face.direction == -1)
    {
        if (pos.y() == 1) return 1.0f/6.0f;
        else if (pos.z() == 0) return 0.0f;
        else if (pos.z() == 1) return 2.0f/6.0f;
    }
    else if (face.axis == Axis::X && face.direction == 1)
    {
        if (pos.z() == 1) return 1.0f/6.0f;
        else if (pos.y() == 0) return 0.0f;
        else if (pos.y() == -1) return 2.0f/6.0f;
    }
    else if (pos.x() == -1 && face.axis != Axis::X) return 2.0f/6.0f;

    return float(3.0f/6.0f);
}

void Cube::draw(Camera* camera, Shader* shader)
{
    texture->bind();
    shader->bind();
    shader->setUniform1i("textureSampler", 0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // render all 26 blocks
    for (int block = 0; block < 26; ++block)
    {
        camera->resetTransformations();
        camera->arrangeBlock(cubies[block].pos, cubies[block].orient);
        shader->setUniformMat4f("MVP", camera->getMVP());

        for (int face = 0; face < 6; ++face)
        {
            rcube::Orientation o = {(Axis)(face % 3), (face % 2) * 2 - 1};

            float x0 = 0.0f, y0 = 0.0f, x1 = 0.0f, y1 = 0.0f;

            if (cubies[block].stickers.find(o) != cubies[block].stickers.end())
            {
                glm::vec3 col = colors[(char)cubies[block].stickers[o]];
                shader->setUniform4f("inColor", col.x, col.y, col.z, 1.0f);

                y0 = gety0(cubies[block].formalPos, o);
                x1 = 1.0f;
                y1 = y0 + 1.0f/6.0f;
            }

            float squareVertices[20] = {
                0.0f, 0.0f, 0.0f, x0, y0,
                0.0f, 0.0f, 0.0f, x1, y0,
                0.0f, 0.0f, 0.0f, x0, y1,
                0.0f, 0.0f, 0.0f, x1, y1
            };

            for (int i = 0; i < 4; ++i)
            {
                int idx = vtxIndices[(face * 4) + i];

                for (int k = 0; k < 3; ++k)
                    squareVertices[(i * 5) + k] = vertices[(idx * 3) + k];
            }

            VertexArray *va = new VertexArray();
            VertexBuffer vb(squareVertices, 20 * sizeof(float));
            VertexBufferLayout layout;
            layout.push<float>(3);
            layout.push<float>(2);
            va->addBuffer(vb, layout);

            render(va, ib, shader);

            delete va;
        }
    }
    glDisable(GL_BLEND);
}

void Cube::performMove(const rcube::Move &move)
{
    this->cube->performMove(move);
    this->addAnimation(move);
}

glm::vec3 toGlmAxis(const Axis &axis)
{
    switch (axis)
    {
        case Axis::X: return glm::vec3(1.0f, 0.0f, 0.0f);
        case Axis::Y: return glm::vec3(0.0f, 1.0f, 0.0f);
        case Axis::Z: return glm::vec3(0.0f, 0.0f, 1.0f);
    }
}

void Cube::addAnimation(const rcube::Move &move)
{
    int angleSign = -1;
    if (move.getAffectedFace().direction < 1 && move.face != SIDE)
    {
        angleSign = 1;
    }

    glm::quat rotation = glm::angleAxis(1.5708f * angleSign * move.direction,
        toGlmAxis(move.axis));

    switch (move.face)
    {
    case ROTATE_X:
    case ROTATE_Y:
    case ROTATE_Z:

        for (int i = 0; i < 26; ++i)
        {
            cubies[i].finalOrient = glm::normalize(rotation *
                cubies[i].finalOrient);
            cubies[i].finalPos.rotate(move.axis, move.direction);
        }
        break;

    default:
        
        rcube::Orientation face = move.getAffectedFace();
        for (int i = 0; i < 26; ++i)
        {
            if (cubies[i].finalPos.coords[face.axis] != face.direction)
                continue;
            
            cubies[i].finalOrient = glm::normalize(rotation *
                cubies[i].finalOrient);
            cubies[i].finalPos.rotate(move.axis, move.direction);
        }
        break;
    }
}

void Cube::updateAnimations()
{
    double now = static_cast<double>(glfwGetTime());
    bool toUpdate = false;

    for (int i = 0; i < 26; ++i)
    {
        if (cubies[i].orient == cubies[i].finalOrient) continue;

        float cosTheta = glm::dot(cubies[i].orient, cubies[i].finalOrient);

        if (abs(cosTheta) >= 0.90f)
        {
            toUpdate = true;
            cubies[i].orient = cubies[i].finalOrient;
            continue;
        }
        toUpdate = false;

        cubies[i].orient = glm::normalize(glm::slerp(cubies[i].orient,
            cubies[i].finalOrient, ROT_SPEED * float(now - lastCall)));
            
        cubies[i].pos = cubies[i].orient * cubies[i].startPos;
    }

    if (toUpdate)
    {
        this->update();
    }
    
    lastCall = now;
}