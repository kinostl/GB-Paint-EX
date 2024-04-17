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

extram saves the image to extram 8192 ... 8192+360

20 * 18 = 360

Unsure why a block randomly gets placed into things but shrug

Well, 19 * 17 now. 

19 * 17 = 323

grab 8192 to 360 just to be safe? Its a clean number that way.
368 cause int16s 

323 + 4 = 327

Image starts on 4? Maybe 5 and ends on 648

I do not know how mathematics works.

Definitely starts at 4. I think these are 8 bit numbers being stored funny.

Yeah confirmed I'm striping bank 0 bank 1 bank 0 as 8 bit numbers.

`(19*17*2)+4 = 650`

So we want 4 to 650? Yup, 648 has 1,0.


so the updateExtRam mod should check to see if 8192+4 ... 8192+650 has changed.

in script.js of the export edit this

```js
  updateExtRam() {
    if (!emulator) return;
    const extram = emulator.getExtRam();
    localStorage.setItem("extram", JSON.stringify(Array.from(extram)));
  }
```

to be something like this

```js
  updateExtRam() {
    if (!emulator) return;
    const extram = emulator.getExtRam();
    localStorage.setItem("extram", JSON.stringify(Array.from(extram)));
    
    const e_slice = JSON.stringify(extram.slice(8196, 8842));
    const le_slice = localStorage.getItem("le_slice");
    if(e_slice != le_slice){
      localStorage.setItem("e_slice", e_slice);
      //api stuff here
    }
  }
```

and then change `async go` out from this

```js
(async function go() {
  let extram = await fetch('https://zonebooth.xyz/api/extram.php');
  extram = await extram.json();
  extram = extram['extram'];
  localStorage.setItem('extram', JSON.stringify(extram));
  let response = await fetch(ROM_FILENAME);
  let romBuffer = await response.arrayBuffer();
  const extRam = new Uint8Array(JSON.parse(localStorage.getItem("extram")));
  Emulator.start(await binjgbPromise, romBuffer, extRam);
  emulator.setBuiltinPalette(vm.palIdx);
})();
```

to this

```js
(async function go() {
  let e_slice = await fetch('https://zonebooth.xyz/api/extram.php');
  e_slice = await extram.json();
  e_slice = extram['extram'];
  
  let extram = localStorage.getItem('extram')
  extram = JSON.parse(extram)
  extram.splice(8196, 8842, ...e_slice)
  
  localStorage.setItem('extram', JSON.stringify(extram));
  
  let response = await fetch(ROM_FILENAME);
  let romBuffer = await response.arrayBuffer();
  const extRam = new Uint8Array(JSON.parse(localStorage.getItem("extram")));
  Emulator.start(await binjgbPromise, romBuffer, extRam);
  emulator.setBuiltinPalette(vm.palIdx);
})();
```

API Stuff 

```js
      localStorage.setItem("extram", extram);
      fetch('https://zonebooth.xyz/api/extram.php', {
        method: "POST", // *GET, POST, PUT, DELETE, etc.
        mode: "cors", // no-cors, *cors, same-origin
        cache: "no-cache", // *default, no-cache, reload, force-cache, only-if-cached
        credentials: "omit", // include, *same-origin, omit
        headers: {
          "Content-Type": "application/json",
          // 'Content-Type': 'application/x-www-form-urlencoded',
        },
        redirect: "follow", // manual, *follow, error
        referrerPolicy: "no-referrer", // no-referrer, *no-referrer-when-downgrade, origin, origin-when-cross-origin, same-origin, strict-origin, strict-origin-when-cross-origin, unsafe-url
        body: e_slice, // body data type must match "Content-Type" header
      })
```