#pragma bank 255

#include "vm.h"
#include <gb/gb.h>
#include "data_manager.h"

void paintSquare(SCRIPT_CTX * THIS) OLDCALL BANKED {
    THIS;
    uint8_t bkg_tile;

    uint16_t col = (*(uint16_t *) VM_REF_TO_PTR(FN_ARG2));
    uint16_t row =  (*(uint16_t *) VM_REF_TO_PTR(FN_ARG1));
    uint16_t shade =  (*(uint16_t *) VM_REF_TO_PTR(FN_ARG0));

    bkg_tile = get_bkg_tile_xy(shade, 0);
    set_bkg_tile_xy(col, row, bkg_tile);
}

void printImageToOverlay(SCRIPT_CTX * THIS) OLDCALL BANKED {
    THIS;
    uint8_t bkg_tile;

    for(int y=1;y<18;y++){
        for(int x=0;x<20;x++){
            bkg_tile = get_bkg_tile_xy(x, y);
            set_win_tile_xy(x, y, bkg_tile);
        }
    }
}
