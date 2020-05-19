#include "glfw.h"
#include "position.h"
#include "workarea.h"
#include "scale.h"
#include "video_mode.h"
#include "gamma_ramp.h"

#include "monitor.h"

// Constructor/Destructor
JSValue glfw_monitor_ctor(JSContext* ctx, JSValueConst new_target, int argc, JSValueConst *argv) {
  return glfw_monitor_new_instance(ctx, glfwGetPrimaryMonitor());
}

// Properties
JSValue glfw_monitor_get_name(JSContext *ctx, JSValueConst this_val) {
  GLFWmonitor* monitor = JS_GetOpaque2(ctx, this_val, glfw_monitor_class_id);
  if (!monitor) return JS_EXCEPTION;

  const char* name = glfwGetMonitorName(monitor);
  return JS_NewString(ctx, name);
}
JSValue glfw_monitor_get_position(JSContext *ctx, JSValueConst this_val) {
  GLFWmonitor* monitor = JS_GetOpaque2(ctx, this_val, glfw_monitor_class_id);
  if (!monitor) return JS_EXCEPTION;

  GLFWPosition* position = js_mallocz(ctx, sizeof(*position));
  glfwGetMonitorPos(monitor, &position->x, &position->y);
  return glfw_position_new_instance(ctx, position);
}
JSValue glfw_monitor_get_workarea(JSContext *ctx, JSValueConst this_val) {
  GLFWmonitor* monitor = JS_GetOpaque2(ctx, this_val, glfw_monitor_class_id);
  if (!monitor) return JS_EXCEPTION;

  GLFWWorkArea* workarea = js_mallocz(ctx, sizeof(*workarea));
  if (!workarea) return JS_EXCEPTION;

  GLFWPosition* position = js_mallocz(ctx, sizeof(*position));
  if (!position) return JS_EXCEPTION;

  GLFWSize* size = js_mallocz(ctx, sizeof(*size));
  if (!size) return JS_EXCEPTION;

  workarea->position = position;
  workarea->size = size;

  glfwGetMonitorWorkarea(monitor, &position->x, &position->y, &size->width, &size->height);
  return glfw_workarea_new_instance(ctx, workarea);
}
JSValue glfw_monitor_get_physical_size(JSContext *ctx, JSValueConst this_val) {
  GLFWmonitor* monitor = JS_GetOpaque2(ctx, this_val, glfw_monitor_class_id);
  if (!monitor) return JS_EXCEPTION;

  GLFWSize* size = js_mallocz(ctx, sizeof(*size));
  glfwGetMonitorPhysicalSize(monitor, &size->width, &size->height);
  return glfw_size_new_instance(ctx, size);
}
JSValue glfw_monitor_get_content_scale(JSContext *ctx, JSValueConst this_val) {
  GLFWmonitor* monitor = JS_GetOpaque2(ctx, this_val, glfw_monitor_class_id);
  if (!monitor) return JS_EXCEPTION;

  GLFWScale* scale = js_mallocz(ctx, sizeof(*scale));
  glfwGetMonitorContentScale(monitor, (float*)&scale->x, (float*)&scale->y);
  return glfw_scale_new_instance(ctx, scale);
}
JSValue glfw_monitor_get_current_video_mode(JSContext *ctx, JSValueConst this_val) {
  GLFWmonitor* monitor = JS_GetOpaque2(ctx, this_val, glfw_monitor_class_id);
  if (!monitor) return JS_EXCEPTION;

  const GLFWvidmode* video_mode = glfwGetVideoMode(monitor);
  return glfw_video_mode_new_instance(ctx, video_mode);
}
JSValue glfw_monitor_get_video_modes(JSContext *ctx, JSValueConst this_val) {
  GLFWmonitor* monitor = JS_GetOpaque2(ctx, this_val, glfw_monitor_class_id);
  if (!monitor) return JS_EXCEPTION;

  int count;
  const GLFWvidmode* video_modes = glfwGetVideoModes(monitor, &count);

  JSValue array = JS_NewArray(ctx);
  for (int i = 0; i < count; i++) {
    JSValue video_mode = glfw_video_mode_new_instance(ctx, &video_modes[i]);
    JS_SetPropertyInt64(ctx, array, i, video_mode);
  }

  return array;
}
JSValue glfw_monitor_get_gamma(JSContext *ctx, JSValueConst this_val) {
  GLFWmonitor* monitor = JS_GetOpaque2(ctx, this_val, glfw_monitor_class_id);
  if (!monitor) return JS_EXCEPTION;

  const GLFWgammaramp* gamma = glfwGetGammaRamp(monitor);
  return glfw_gamma_ramp_new_instance(ctx, gamma);
}
JSValue glfw_monitor_set_gamma(JSContext *ctx, JSValueConst this_val, JSValueConst value) {
  GLFWmonitor* monitor = JS_GetOpaque2(ctx, this_val, glfw_monitor_class_id);
  if (!monitor) return JS_EXCEPTION;

  if (JS_IsNumber(value)) {
    float gamma = JS_VALUE_GET_FLOAT64(value);
    glfwSetGamma(monitor, gamma);
    return JS_UNDEFINED;
  }

  GLFWgammaramp* gamma_ramp = JS_GetOpaque2(ctx, value, glfw_gamma_ramp_class_id);
  if (!monitor) return JS_EXCEPTION;

  glfwSetGammaRamp(monitor, gamma_ramp);

  return JS_UNDEFINED;
}

// Static properties
JSValue glfw_monitor_get_monitors(JSContext *ctx, JSValueConst this_val) {
  int count;
  GLFWmonitor** monitors = glfwGetMonitors(&count);

  JSValue array = JS_NewArray(ctx);
  for (int i = 0; i < count; i++) {
    JSValue monitor = glfw_monitor_new_instance(ctx, monitors[i]);
    JS_SetPropertyInt64(ctx, array, i, monitor);
  }

  return array;
}

// Initialization
JSClassDef glfw_monitor_class_def = {
  "Monitor",
};

const JSCFunctionListEntry glfw_monitor_proto_funcs[] = {
  JS_CGETSET_DEF("name", glfw_monitor_get_name, NULL),
  JS_CGETSET_DEF("position", glfw_monitor_get_position, NULL),
  JS_CGETSET_DEF("workarea", glfw_monitor_get_workarea, NULL),
  JS_CGETSET_DEF("physical_size", glfw_monitor_get_physical_size, NULL),
  JS_CGETSET_DEF("content_scale", glfw_monitor_get_content_scale, NULL),
  JS_CGETSET_DEF("currentVideoMode", glfw_monitor_get_current_video_mode, NULL),
  JS_CGETSET_DEF("videoModes", glfw_monitor_get_video_modes, NULL),
  JS_CGETSET_DEF("gamma", glfw_monitor_get_gamma, glfw_monitor_set_gamma),
};

const JSCFunctionListEntry glfw_monitor_funcs[] = {
  JS_CGETSET_DEF("monitors", glfw_monitor_get_monitors, NULL),
};

JSValue glfw_monitor_proto, glfw_monitor_class;

JSValue glfw_monitor_constructor(JSContext* ctx) {
  JSRuntime* rt = JS_GetRuntime(ctx);

  if (!JS_IsRegisteredClass(rt, glfw_monitor_class_id)) {
    JS_NewClassID(&glfw_monitor_class_id);
    JS_NewClass(rt, glfw_monitor_class_id, &glfw_monitor_class_def);

    glfw_monitor_proto = JS_NewObject(ctx);
    JS_SetPropertyFunctionList(ctx, glfw_monitor_proto, glfw_monitor_proto_funcs, countof(glfw_monitor_proto_funcs));
    JS_SetClassProto(ctx, glfw_monitor_class_id, glfw_monitor_proto);

    glfw_monitor_class = JS_NewCFunction2(ctx, glfw_monitor_ctor, "Monitor", 5, JS_CFUNC_constructor, 0);
    JS_SetPropertyFunctionList(ctx, glfw_monitor_class, glfw_monitor_funcs, countof(glfw_monitor_funcs));
    JS_SetConstructor(ctx, glfw_monitor_class, glfw_monitor_proto);
  }

  return glfw_monitor_class;
}

JSValue glfw_monitor_new_instance(JSContext* ctx, GLFWmonitor* monitor) {
  JSValue ctor = glfw_monitor_constructor(ctx);
  JSValue proto = JS_GetPropertyStr(ctx, ctor, "prototype");
  if (JS_IsException(proto)) {
    JS_FreeValue(ctx, proto);
    return JS_EXCEPTION;
  }

  JSValue obj = JS_NewObjectProtoClass(ctx, proto, glfw_monitor_class_id);
  JS_FreeValue(ctx, proto);
  if (JS_IsException(obj)) {
    JS_FreeValue(ctx, obj);
    return JS_EXCEPTION;
  }

  JS_SetOpaque(obj, monitor);
  return obj;
}

int glfw_monitor_init(JSContext* ctx, JSModuleDef* m) {
  JS_SetModuleExport(ctx, m, "Monitor", glfw_monitor_constructor(ctx));
  return 0;
}

int glfw_monitor_export(JSContext* ctx, JSModuleDef* m) {
  return JS_AddModuleExport(ctx, m, "Monitor");
}
