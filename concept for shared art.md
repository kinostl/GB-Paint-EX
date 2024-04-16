itch io stores extram to localstorage. It also does some stuff with sram I'm not going to bother learning because using it would be too much of a hack.

We might be able to add a button to the html output that can copy the extram to a comment or upload it to a server. This should allow for the thing I want to do.

clicking to make a comment does not download a csrf ... inform user that it'll make a comment on their behalf?

csrf is stored as meta name `csrf_token` 
new comment url is `https://itch.io/topic/3674525/new-post` where `3674525` is a unique id provided on the game's creation, unrelated to the game's id.

Maybe I can hide comments?
Comments have a reasonable to work with class tree.

Might be too much trouble to bother.

Maybe just do a copy/paste box would be better and respect people's agency better.

itch io Blindly loads stuff.

The Debugger uses this to render the vram

```javascript
  renderVRam() {
    var ctx = this.vramCanvas.getContext("2d");
    var imgData = ctx.createImageData(256, 256);
    var ptr = this.module._malloc(4 * 256 * 256);
    this.module._emulator_render_vram(this.e, ptr);
    var buffer = new Uint8Array(this.module.HEAP8.buffer, ptr, 4 * 256 * 256);
    imgData.data.set(buffer);
    ctx.putImageData(imgData, 0, 0);
    this.module._free(ptr);
    return this.vramCanvas.toDataURL("image/png");
  }
```

not as helpful as expected ... just gave the debug style vram output

btoa the extram won't work either thats a lot of text, too long for a comment. Also, the extram is just incomprehensible.

extram will work because I made an api for it on my server.