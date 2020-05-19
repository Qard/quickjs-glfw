- API Documentation
- Make Window hints an object argument to the constructor?
- Figure out a good model for setting callbacks
  - Should it be multi-tenant or direct-mounting?
  ```js
  events.onerror = error => {}
  // vs
  on('error', error => {})
  ```

# Class Methods

- window
  - glfwSetWindowIcon
  - glfwSetWindowSizeLimits 
  - glfwSetWindowAspectRatio
  - glfwGetWindowFrameSize
  - glfwGetWindowContentScale
  - glfwSetWindowMonitor 
  - glfwGetWindowAttrib
  - glfwSetWindowAttrib
  - glfwSetWindowUserPointer 
  - glfwGetWindowUserPointer 
  - All the callbacks
