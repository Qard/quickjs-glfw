import {
  // Top-level functions
  poll,
  wait,
  postEmptyEvent,
  context,
  version
} from 'glfw.so'

import {
  // Constants
  CONTEXT_VERSION_MAJOR,
  CONTEXT_VERSION_MINOR,
  OPENGL_PROFILE,
  OPENGL_CORE_PROFILE,
  OPENGL_FORWARD_COMPAT,
  RESIZABLE,
  SAMPLES,

  // Classes
  Window,
  Position,
  Size
} from 'glfw.so'

Window.hint(CONTEXT_VERSION_MAJOR, 3)
Window.hint(CONTEXT_VERSION_MINOR, 2)
Window.hint(OPENGL_PROFILE, OPENGL_CORE_PROFILE)
Window.hint(OPENGL_FORWARD_COMPAT, true)
Window.hint(RESIZABLE, false)
Window.hint(SAMPLES, 4)

const window = new Window(800, 600, "OpenGL")
// window.makeContextCurrent()
context.current = window

// const win = context.current
const win = window

const { width, height } = win.size
const { x, y } = win.position

console.log(`width: ${width}, height: ${height}, x: ${x}, y: ${y}`)

while (!window.shouldClose) {
  window.swapBuffers()
  poll()
}
