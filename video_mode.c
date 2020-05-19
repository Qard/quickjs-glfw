#include "glfw.h"

#include "video_mode.h"

// Constructor/Destructor
JSValue glfw_video_mode_ctor(JSContext* ctx, JSValueConst new_target, int argc, JSValueConst* argv) {
  JS_ThrowInternalError(ctx, "VideoMode can not be constructed directly");
  return JS_EXCEPTION;
}

// Properties
JSValue glfw_video_mode_get_int(JSContext* ctx, JSValueConst this_val, int magic) {
  GLFWvidmode* video_mode = JS_GetOpaque2(ctx, this_val, glfw_video_mode_class_id);
  if (!video_mode) return JS_EXCEPTION;

  int value;

  switch (magic) {
    case 0: value = video_mode->width; break;
    case 1: value = video_mode->height; break;
    case 2: value = video_mode->redBits; break;
    case 3: value = video_mode->greenBits; break;
    case 4: value = video_mode->blueBits; break;
    case 5: value = video_mode->refreshRate; break;
  }

  return JS_NewInt64(ctx, value);
}

JSValue glfw_video_mode_set_int(JSContext* ctx, JSValueConst this_val, JSValue val, int magic) {
  GLFWvidmode* video_mode = JS_GetOpaque2(ctx, this_val, glfw_video_mode_class_id);
  if (!video_mode) return JS_EXCEPTION;

  int value;
  if (JS_ToInt32(ctx, &value, val))
    return JS_EXCEPTION;

  switch (magic) {
    case 0: video_mode->width = value; break;
    case 1: video_mode->height = value; break;
    case 2: video_mode->redBits = value; break;
    case 3: video_mode->greenBits = value; break;
    case 4: video_mode->blueBits = value; break;
    case 5: video_mode->refreshRate = value; break;
  }

  return JS_UNDEFINED;
}

// Initialization
JSClassDef glfw_video_mode_class_def = {
  "VideoMode",
}; 

const JSCFunctionListEntry glfw_video_mode_proto_funcs[] = {
  JS_CGETSET_MAGIC_DEF("width", glfw_video_mode_get_int, glfw_video_mode_set_int, 0),
  JS_CGETSET_MAGIC_DEF("height", glfw_video_mode_get_int, glfw_video_mode_set_int, 1),
  JS_CGETSET_MAGIC_DEF("redBits", glfw_video_mode_get_int, glfw_video_mode_set_int, 2),
  JS_CGETSET_MAGIC_DEF("greenBits", glfw_video_mode_get_int, glfw_video_mode_set_int, 3),
  JS_CGETSET_MAGIC_DEF("blueBits", glfw_video_mode_get_int, glfw_video_mode_set_int, 4),
  JS_CGETSET_MAGIC_DEF("refreshRate", glfw_video_mode_get_int, glfw_video_mode_set_int, 5),
};

JSValue glfw_video_mode_proto, glfw_video_mode_class;

JSValue glfw_video_mode_constructor(JSContext* ctx) {
  JSRuntime* rt = JS_GetRuntime(ctx);

  if (!JS_IsRegisteredClass(rt, glfw_video_mode_class_id)) {
    JS_NewClassID(&glfw_video_mode_class_id);
    JS_NewClass(JS_GetRuntime(ctx), glfw_video_mode_class_id, &glfw_video_mode_class_def);

    glfw_video_mode_proto = JS_NewObject(ctx);
    JS_SetPropertyFunctionList(ctx, glfw_video_mode_proto, glfw_video_mode_proto_funcs, countof(glfw_video_mode_proto_funcs));
    JS_SetClassProto(ctx, glfw_video_mode_class_id, glfw_video_mode_proto);

    glfw_video_mode_class = JS_NewCFunction2(ctx, glfw_video_mode_ctor, "VideoMode", 2, JS_CFUNC_constructor, 0);
    /* set proto.constructor and ctor.prototype */
    JS_SetConstructor(ctx, glfw_video_mode_class, glfw_video_mode_proto);
  }

  return glfw_video_mode_class;
}

JSValue glfw_video_mode_new_instance(JSContext* ctx, const GLFWvidmode* video_mode) {
  JSValue ctor = glfw_video_mode_constructor(ctx);
  JSValue proto = JS_GetPropertyStr(ctx, ctor, "prototype");
  if (JS_IsException(proto)) {
    JS_FreeValue(ctx, proto);
    return JS_EXCEPTION;
  }

  JSValue obj = JS_NewObjectProtoClass(ctx, proto, glfw_video_mode_class_id);
  JS_FreeValue(ctx, proto);
  if (JS_IsException(proto)) {
    JS_FreeValue(ctx, obj);
    return JS_EXCEPTION;
  }

  JS_SetOpaque(obj, (void*) video_mode);
  return obj;
}

int glfw_video_mode_init(JSContext* ctx, JSModuleDef* m) {
  JS_SetModuleExport(ctx, m, "VideoMode", glfw_video_mode_constructor(ctx));
  return 0;
}

int glfw_video_mode_export(JSContext* ctx, JSModuleDef* m) {
  return JS_AddModuleExport(ctx, m, "VideoMode");
}
