#include "GridSizeDialog.hpp"

GridSizeDialog::GridSizeDialog(int x, int y) : GridSizeGUI(NULL)
{
    gridSizeX->SetValue(x);
    gridSizeY->SetValue(y);
}

void GridSizeDialog::GetGridSize(int& width, int& height)
{
    width = gridSizeX->GetValue();
    height = gridSizeY->GetValue();
}
