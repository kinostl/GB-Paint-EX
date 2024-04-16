#include <stdint.h>
#pragma bank 255

#include "vm.h"
#include <gb/gb.h>
#include "data_manager.h"

union tile {
    uint8_t _tile;
    struct {
        unsigned char palette : 3;
        unsigned char bank : 1;
        unsigned char nothing : 1;
        unsigned char flip_h : 1;
        unsigned char flip_v : 1;
        unsigned char priority : 1;
    } attr;
};

void drawTile(SCRIPT_CTX * THIS) OLDCALL BANKED {
    uint16_t col = (*(uint16_t *) VM_REF_TO_PTR(FN_ARG1));
    uint16_t row =  (*(uint16_t *) VM_REF_TO_PTR(FN_ARG0));

    vmemcpy(
        get_win_xy_addr(col, row),
            get_bkg_xy_addr(col, row),
            1
    );
}

void rotateBkgTile(SCRIPT_CTX * THIS) OLDCALL BANKED {
    THIS;
    uint8_t * bkg_tile_addr;
    union tile bkg_tile;
    //union tile win_tile;

    uint16_t col = (*(uint16_t *) VM_REF_TO_PTR(FN_ARG1));
    uint16_t row =  (*(uint16_t *) VM_REF_TO_PTR(FN_ARG0));

    
    VBK_REG = VBK_BANK_1;
    bkg_tile_addr = get_bkg_xy_addr(col, row);
    bkg_tile._tile = get_vram_byte(bkg_tile_addr);
    //win_tile._tile = get_vram_byte(win_tile_addr);
    if(
        !bkg_tile.attr.flip_h &&
        !bkg_tile.attr.flip_v
    ){
        bkg_tile.attr.flip_h = ~(bkg_tile.attr.flip_h);
    } else if (
        bkg_tile.attr.flip_h &&
        !bkg_tile.attr.flip_v
    ){
        bkg_tile.attr.flip_v = ~(bkg_tile.attr.flip_v);
    } else if (
        bkg_tile.attr.flip_h &&
        bkg_tile.attr.flip_v
    ){
        bkg_tile.attr.flip_h = ~(bkg_tile.attr.flip_h);
    } else if (
        !bkg_tile.attr.flip_h &&
        bkg_tile.attr.flip_v
    ){
        bkg_tile.attr.flip_v = ~(bkg_tile.attr.flip_v);
    }
    set_vram_byte(bkg_tile_addr, bkg_tile._tile);
    VBK_REG = VBK_BANK_0;
}

void rotateWinTile(SCRIPT_CTX * THIS) OLDCALL BANKED {
    THIS;
    uint8_t * win_tile_addr;
    union tile win_tile;
    //union tile win_tile;

    uint16_t col = (*(uint16_t *) VM_REF_TO_PTR(FN_ARG1));
    uint16_t row =  (*(uint16_t *) VM_REF_TO_PTR(FN_ARG0));

    
    VBK_REG = VBK_BANK_1;
    win_tile_addr = get_win_xy_addr(col, row);
    win_tile._tile = get_vram_byte(win_tile_addr);
    //win_tile._tile = get_vram_byte(win_tile_addr);
    if(
        !win_tile.attr.flip_h &&
        !win_tile.attr.flip_v
    ){
        win_tile.attr.flip_h = ~(win_tile.attr.flip_h);
    } else if (
        win_tile.attr.flip_h &&
        !win_tile.attr.flip_v
    ){
        win_tile.attr.flip_v = ~(win_tile.attr.flip_v);
    } else if (
        win_tile.attr.flip_h &&
        win_tile.attr.flip_v
    ){
        win_tile.attr.flip_h = ~(win_tile.attr.flip_h);
    } else if (
        !win_tile.attr.flip_h &&
        win_tile.attr.flip_v
    ){
        win_tile.attr.flip_v = ~(win_tile.attr.flip_v);
    }
    set_vram_byte(win_tile_addr, win_tile._tile);
    VBK_REG = VBK_BANK_0;
}

//Sprite data found via trial and error
void setTileStampB(SCRIPT_CTX * THIS) OLDCALL BANKED {
    uint16_t col = (*(uint16_t *) VM_REF_TO_PTR(FN_ARG1));
    uint16_t row =  (*(uint16_t *) VM_REF_TO_PTR(FN_ARG0));
    
    uint8_t tileBuf[16];
    get_bkg_data( get_bkg_tile_xy(col, row) , 1, tileBuf);
    set_sprite_data(15, 1, tileBuf);
}

void setTileStampA(SCRIPT_CTX * THIS) OLDCALL BANKED {
    uint16_t col = (*(uint16_t *) VM_REF_TO_PTR(FN_ARG1));
    uint16_t row =  (*(uint16_t *) VM_REF_TO_PTR(FN_ARG0));
    
    uint8_t tileBuf[16];
    get_bkg_data( get_bkg_tile_xy(col, row) , 1, tileBuf);
    set_sprite_data(9, 1, tileBuf);
}