#include <asm/types.h>
#include <stdint.h>
#pragma bank 255

#include "vm.h"
#include <gb/gb.h>
#include "data_manager.h"
#include "load_save.h"

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

union tile_var {
    UWORD _var;
    struct {
        UBYTE b0_tile;
        UBYTE b1_tile;
    } tiles;
};

union tile_vram {
    UWORD _var;
    struct {
        uint8_t b0_tile;
        uint8_t b1_tile;
    } tiles;
};

void drawTile(SCRIPT_CTX * THIS) OLDCALL BANKED {
    uint16_t w_col = (*(uint16_t *) VM_REF_TO_PTR(FN_ARG3));
    uint16_t w_row =  (*(uint16_t *) VM_REF_TO_PTR(FN_ARG2));
    
    uint16_t b_col = (*(uint16_t *) VM_REF_TO_PTR(FN_ARG1));
    uint16_t b_row =  (*(uint16_t *) VM_REF_TO_PTR(FN_ARG0));

    vmemcpy(
        get_win_xy_addr(w_col, w_row),
            get_bkg_xy_addr(b_col, b_row),
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

void setTileStampB(SCRIPT_CTX * THIS) OLDCALL BANKED {
    uint16_t col = (*(uint16_t *) VM_REF_TO_PTR(FN_ARG1));
    uint16_t row =  (*(uint16_t *) VM_REF_TO_PTR(FN_ARG0));
    
    vmemcpy(
        get_bkg_xy_addr(16, 16),
            get_bkg_xy_addr(col, row),
            1
    );
}

void setTileStampA(SCRIPT_CTX * THIS) OLDCALL BANKED {
    uint16_t col = (*(uint16_t *) VM_REF_TO_PTR(FN_ARG1));
    uint16_t row =  (*(uint16_t *) VM_REF_TO_PTR(FN_ARG0));
    
    vmemcpy(
        get_bkg_xy_addr(18, 16),
            get_bkg_xy_addr(col, row),
            1
    );
}

void handleSaveVram(SCRIPT_CTX * THIS) OLDCALL BANKED {
    uint8_t * win_tile_addr;
    uint16_t col, row;
    union tile_var win_tile;
    data_save(2);
    
    INT16 var_i = 0;
    vm_set_const(THIS, var_i++, 'p');
    vm_set_const(THIS, var_i++, 'a');
    vm_set_const(THIS, var_i++, 'i');
    vm_set_const(THIS, var_i++, 'n');
    vm_set_const(THIS, var_i++, 't');
    
    for(col = 1; col < 20; col++){
        for(row = 0; row < 17; row++){
            VBK_REG = VBK_BANK_1;
            win_tile_addr = get_win_xy_addr(col, row);
            win_tile.tiles.b1_tile = get_vram_byte(win_tile_addr);
            VBK_REG = VBK_BANK_0;
            win_tile_addr = get_win_xy_addr(col, row);
            win_tile.tiles.b0_tile = get_vram_byte(win_tile_addr);
            
            vm_set_const(THIS, var_i, win_tile._var);
            var_i++;
        }
    }
    
    vm_set_const(THIS, var_i++, 'b');
    vm_set_const(THIS, var_i++, 'r');
    vm_set_const(THIS, var_i++, 'u');
    vm_set_const(THIS, var_i++, 's');
    vm_set_const(THIS, var_i++, 'h');
    
    data_save(1);
    data_load(2);
    
}

void handleLoadVram(SCRIPT_CTX * THIS) OLDCALL BANKED {
    THIS;
    uint8_t * win_tile_addr;
    uint16_t col, row;
    union tile_vram win_tile;
    data_save(2);
    data_load(1);
    INT16 var_i = 5; //First unused variable
    
    for(col = 1; col < 20; col++){
        for(row = 0; row < 17; row++){
            win_tile._var = *(script_memory + var_i);
            
            VBK_REG = VBK_BANK_1;
            win_tile_addr = get_win_xy_addr(col, row);
            set_vram_byte(win_tile_addr, win_tile.tiles.b1_tile);
            
            VBK_REG = VBK_BANK_0;
            win_tile_addr = get_win_xy_addr(col, row);
            set_vram_byte(win_tile_addr, win_tile.tiles.b0_tile);
            
            var_i++;
        }
    }
    
    data_load(2);
}