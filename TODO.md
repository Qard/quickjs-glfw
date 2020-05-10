- API Documentation
- Make Window hints an object argument to the constructor?
- Make top-level context object?
  ```js
  context.current = window
  const window = context.current
  context.swapInterval = 100
  ```
- Figure out a good model for setting callbacks
  - Should it be multi-tenant or direct-mounting?
  ```js
  events.onerror = error => {}
  // vs
  on('error', error => {})
  ```
