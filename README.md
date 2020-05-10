# quickjs-glfw

NOTE: This is _EXPERIMENTAL_. As such, expect missing/broken features.

GLFW bindings for QuickJS. Meant to be built as a static library and imported
with `qjs`.

If you can get it to work with `qjsc -M glfw.so ...` also, PRs are welcome. 🙂

## Build

```sh
make
```

## Usage

```js
import {
  poll,
  Window,

  // Constants
  CONTEXT_VERSION_MAJOR,
  CONTEXT_VERSION_MINOR,
  OPENGL_PROFILE,
  OPENGL_CORE_PROFILE,
  OPENGL_FORWARD_COMPAT,
  RESIZABLE,
  SAMPLES,
} from 'glfw.so'

Window.hint(CONTEXT_VERSION_MAJOR, 3)
Window.hint(CONTEXT_VERSION_MINOR, 2)
Window.hint(OPENGL_PROFILE, OPENGL_CORE_PROFILE)
Window.hint(OPENGL_FORWARD_COMPAT, true)
Window.hint(RESIZABLE, false)
Window.hint(SAMPLES, 4)

const window = new Window(800, 600, "OpenGL")
window.makeContextCurrent()

const { width, height } = window.size
const { x, y } = window.position

console.log(`width: ${width}, height: ${height}, x: ${x}, y: ${y}`)

while (!window.shouldClose) {
  window.swapBuffers()
  poll()
}
```

---

### Copyright (c) 2020 Stephen Belanger

#### Licensed under MIT License

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
