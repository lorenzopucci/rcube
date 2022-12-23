#include <rcube.hpp>
#include "ui.hpp"

int main()
{
    return rcubeUI::runUI(new rcube::Cube());
}