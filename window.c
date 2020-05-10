#include "glfw.h"
#include "position.h"
#include "size.h"

#include "window.h"

// Constructor/Destructor
JSValue glfw_window_ctor(JSContext* ctx, JSValueConst new_target, int argc, JSValueConst *argv) {
  int width, height;
  const char* title;
  
  if (JS_ToInt32(ctx, &width, argv[0]))
    return JS_EXCEPTION;
  
  if (JS_ToInt32(ctx, &height, argv[1]))
    return JS_EXCEPTION;
  
  title = JS_ToCString(ctx, argv[2]);
  if (!title) return JS_EXCEPTION;

  return glfw_window_create_window(ctx, width, height, title, NULL, NULL);
}

void glfw_window_finalizer(JSRuntime* rt, JSValue val) {
  GLFWwindow* window = JS_GetOpaque(val, glfw_window_class_id);
  glfwDestroyWindow(window);
}

// Instance Methods
JSValue glfw_window_make_context_current(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
  GLFWwindow* window = JS_GetOpaque2(ctx, this_val, glfw_window_class_id);
  if (!window) return JS_EXCEPTION;
  glfwMakeContextCurrent(window);
  return JS_UNDEFINED;
}

JSValue glfw_window_swap_buffers(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
  GLFWwindow* window = JS_GetOpaque2(ctx, this_val, glfw_window_class_id);
  if (!window) return JS_EXCEPTION;
  glfwSwapBuffers(window);
  return JS_UNDEFINED;
}

// Static  Methods
JSValue glfw_window_hint(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv) {
  int key;

  if (JS_ToInt32(ctx, &key, argv[0]))
    return JS_EXCEPTION;

  if (JS_IsString(argv[1])) {
    const char* value = JS_ToCString(ctx, argv[1]);
    glfwWindowHintString(key, value);
  } else if (JS_IsBool(argv[1])) {
    int value = JS_VALUE_GET_BOOL(argv[1]) == 1 ? GL_TRUE : GL_FALSE;
    glfwWindowHint(key, value);
  } else if (JS_IsNumber(argv[1])) {
    int value;
    if (JS_ToInt32(ctx, &value, argv[1]))
      return JS_EXCEPTION;

    glfwWindowHint(key, value);
  } else {
    JS_ThrowTypeError(ctx, "Value must be a number or string");
    return JS_EXCEPTION;
  }

  return JS_UNDEFINED;
}

// Properties
JSValue glfw_window_get_should_close(JSContext *ctx, JSValueConst this_val) {
  GLFWwindow* window = JS_GetOpaque2(ctx, this_val, glfw_window_class_id);
  if (!window) return JS_EXCEPTION;

  if (glfwWindowShouldClose(window))
    return JS_TRUE;
  else
    return JS_FALSE;
}
JSValue glfw_window_set_should_close(JSContext *ctx, JSValueConst this_val, JSValueConst value) {
  GLFWwindow* window = JS_GetOpaque2(ctx, this_val, glfw_window_class_id);
  if (!window) return JS_EXCEPTION;
  int shouldClose = JS_VALUE_GET_BOOL(value) == 1 ? GL_TRUE : GL_FALSE;
  glfwSetWindowShouldClose(window, shouldClose);
  return JS_UNDEFINED;
}

JSValue glfw_window_set_title(JSContext *ctx, JSValueConst this_val, JSValueConst value) {
  GLFWwindow* window = JS_GetOpaque2(ctx, this_val, glfw_window_class_id);
  if (!window) return JS_EXCEPTION;

  if (!JS_IsString(value)) {
    JS_ThrowTypeError(ctx, "Title must be a string");
    return JS_EXCEPTION;
  }

  const char* title = JS_ToCString(ctx, value);
  glfwSetWindowTitle(window, title);
  return JS_UNDEFINED;
}

JSValue glfw_window_set_position(JSContext *ctx, JSValueConst this_val, JSValueConst value) {
  GLFWwindow* window = JS_GetOpaque2(ctx, this_val, glfw_window_class_id);
  if (!window) return JS_EXCEPTION;

  GLFWPosition* position = JS_GetOpaque2(ctx, value, glfw_position_class_id);
  if (!position) return JS_EXCEPTION;

  glfwSetWindowPos(window, position->x, position->y);
  return JS_UNDEFINED;
}
JSValue glfw_window_get_position(JSContext *ctx, JSValueConst this_val) {
  GLFWwindow* window = JS_GetOpaque2(ctx, this_val, glfw_window_class_id);
  if (!window) return JS_EXCEPTION;

  int x, y;
  glfwGetWindowPos(window, &x, &y);

  int argc = 2;
  JSValueConst argv[2];
  argv[0] = JS_NewInt32(ctx, x);
  argv[1] = JS_NewInt32(ctx, y);

  return JS_CallConstructor(ctx, glfw_position_constructor(ctx), argc, argv);
}

// TODO: magic these with position setter/getter?
JSValue glfw_window_set_size(JSContext *ctx, JSValueConst this_val, JSValueConst value) {
  GLFWwindow* window = JS_GetOpaque2(ctx, this_val, glfw_window_class_id);
  if (!window) return JS_EXCEPTION;

  GLFWSize* size = JS_GetOpaque2(ctx, value, glfw_size_class_id);
  if (!size) return JS_EXCEPTION;

  glfwSetWindowSize(window, size->width, size->height);
  return JS_UNDEFINED;
}
JSValue glfw_window_get_size(JSContext *ctx, JSValueConst this_val) {
  GLFWwindow* window = JS_GetOpaque2(ctx, this_val, glfw_window_class_id);
  if (!window) return JS_EXCEPTION;

  int width, height;
  glfwGetWindowSize(window, &width, &height);

  int argc = 2;
  JSValueConst argv[2];
  argv[0] = JS_NewInt32(ctx, width);
  argv[1] = JS_NewInt32(ctx, height);

  return JS_CallConstructor(ctx, glfw_size_constructor(ctx), argc, argv);
}

JSValue glfw_window_get_framebuffer_size(JSContext *ctx, JSValueConst this_val) {
  GLFWwindow* window = JS_GetOpaque2(ctx, this_val, glfw_window_class_id);
  if (!window) return JS_EXCEPTION;

  int width, height;
  glfwGetFramebufferSize(window, &width, &height);

  int argc = 2;
  JSValueConst argv[2];
  argv[0] = JS_NewInt32(ctx, width);
  argv[1] = JS_NewInt32(ctx, height);

  return JS_CallConstructor(ctx, glfw_size_constructor(ctx), argc, argv);
}

JSValue glfw_window_set_opacity(JSContext *ctx, JSValueConst this_val, JSValueConst value) {
  GLFWwindow* window = JS_GetOpaque2(ctx, this_val, glfw_window_class_id);
  if (!window) return JS_EXCEPTION;

  double opacity;
  if (JS_ToFloat64(ctx, &opacity, value))
    return JS_EXCEPTION;

  glfwSetWindowOpacity(window, opacity);
  return JS_UNDEFINED;
}
JSValue glfw_window_get_opacity(JSContext *ctx, JSValueConst this_val) {
  GLFWwindow* window = JS_GetOpaque2(ctx, this_val, glfw_window_class_id);
  if (!window) return JS_EXCEPTION;
  return JS_NewFloat64(ctx, glfwGetWindowOpacity(window));
}

// Generate a few simple methods with macros...because I'm lazy. :O
#define TRIGGER_FUNCTIONS(V)                                                  \
  V(IconifyWindow, iconify)                                                   \
  V(RestoreWindow, restore)                                                   \
  V(MaximizeWindow, maximize)                                                 \
  V(ShowWindow, show)                                                         \
  V(HideWindow, hide)                                                         \
  V(FocusWindow, focus)                                                       \
  V(RequestWindowAttention, requestAttention)

#define MAKE_TRIGGER_METHOD(NativeName, JSName)                               \
  JSValue glfw_window_ ## JSName(JSContext *ctx, JSValueConst this_val,       \
                                 int argc, JSValueConst *argv) {              \
    GLFWwindow* window = JS_GetOpaque2(ctx, this_val, glfw_window_class_id);  \
    glfw ## NativeName(window);                                               \
    if (!window) return JS_EXCEPTION;                                         \
    return JS_UNDEFINED;                                                      \
  }
  TRIGGER_FUNCTIONS(MAKE_TRIGGER_METHOD)
#undef MAKE_TRIGGER_METHODS

// Initialization
JSClassDef glfw_window_class_def = {
  "Window",
  .finalizer = glfw_window_finalizer,
}; 

#define MAKE_TRIGGER_METHOD_ENTRY(NativeName, JSName)                         \
  JS_CFUNC_DEF(#JSName, 0, glfw_window_ ## JSName),

const JSCFunctionListEntry glfw_window_proto_funcs[] = {
  JS_CFUNC_DEF("makeContextCurrent", 0, glfw_window_make_context_current),
  JS_CFUNC_DEF("swapBuffers", 0, glfw_window_swap_buffers),
  JS_CGETSET_DEF("shouldClose", glfw_window_get_should_close, glfw_window_set_should_close),
  JS_CGETSET_DEF("title", NULL, glfw_window_set_title),
  JS_CGETSET_DEF("position", glfw_window_get_position, glfw_window_set_position),
  JS_CGETSET_DEF("size", glfw_window_get_size, glfw_window_set_size),
  JS_CGETSET_DEF("framebufferSize", glfw_window_get_framebuffer_size, NULL),
  JS_CGETSET_DEF("opacity", glfw_window_get_opacity, glfw_window_set_opacity),
  TRIGGER_FUNCTIONS(MAKE_TRIGGER_METHOD_ENTRY)
};

#undef MAKE_TRIGGER_METHOD_ENTRY

const JSCFunctionListEntry glfw_window_funcs[] = {
  JS_CFUNC_DEF("hint", 0, glfw_window_hint),
};

#undef TRIGGER_FUNCTIONS

JSValue glfw_window_proto, glfw_window_class;

JSValue glfw_window_constructor(JSContext* ctx) {
  JSRuntime* rt = JS_GetRuntime(ctx);

  if (!JS_IsRegisteredClass(rt, glfw_window_class_id)) {
    JS_NewClassID(&glfw_window_class_id);
    JS_NewClass(rt, glfw_window_class_id, &glfw_window_class_def);

    glfw_window_proto = JS_NewObject(ctx);
    JS_SetPropertyFunctionList(ctx, glfw_window_proto, glfw_window_proto_funcs, countof(glfw_window_proto_funcs));
    JS_SetClassProto(ctx, glfw_window_class_id, glfw_window_proto);

    glfw_window_class = JS_NewCFunction2(ctx, glfw_window_ctor, "Window", 2, JS_CFUNC_constructor, 0);
    JS_SetPropertyFunctionList(ctx, glfw_window_class, glfw_window_funcs, countof(glfw_window_funcs));
    JS_SetConstructor(ctx, glfw_window_class, glfw_window_proto);
  }

  return glfw_window_class;
}

JSValue glfw_window_create_window(JSContext* ctx, int width, int height,
                                 const char* title, GLFWmonitor* monitor,
                                 GLFWwindow* share) {
  GLFWwindow* window = glfwCreateWindow(width, height, title, monitor, share);
  if (window == NULL) {
    glfw_throw(ctx);
    goto fail;
  }

  return glfw_window_new_instance(ctx, window);
 fail:
  return JS_EXCEPTION;
}

JSValue glfw_window_new_instance(JSContext* ctx, GLFWwindow* window) {
  JSValue obj = JS_UNDEFINED;
  JSValue proto;

  proto = JS_GetPropertyStr(ctx, glfw_window_class, "prototype");
  if (JS_IsException(proto))
    goto fail;

  obj = JS_NewObjectProtoClass(ctx, proto, glfw_window_class_id);
  JS_FreeValue(ctx, proto);
  if (JS_IsException(obj))
    goto fail;

  JS_SetOpaque(obj, window);

  return obj;
 fail:
  JS_FreeValue(ctx, obj);
  return JS_EXCEPTION;
}

int glfw_window_init(JSContext* ctx, JSModuleDef* m) {
  JS_SetModuleExport(ctx, m, "Window", glfw_window_constructor(ctx));
  return 0;
}

int glfw_window_export(JSContext* ctx, JSModuleDef* m) {
  return JS_AddModuleExport(ctx, m, "Window");
}
