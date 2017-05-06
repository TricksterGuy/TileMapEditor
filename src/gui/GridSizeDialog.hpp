#ifndef GRIDSIZE_DIALOG_HPP
#define GRIDSIZE_DIALOG_HPP

#include "GridSizeGUI.h"

class GridSizeDialog : public GridSizeGUI {
public:
    GridSizeDialog(int gridx, int gridy);
    void GetGridSize(int& width, int& height);
};

#endif
