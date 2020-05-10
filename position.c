#include "glfw.h"

#include "position.h"

// Constructor/Destructor
JSValue glfw_position_ctor(JSContext* ctx, JSValueConst new_target, int argc, JSValueConst* argv) {
  GLFWPosition* position;
  JSValue obj = JS_UNDEFINED;
  JSValue proto;
  
  position = js_mallocz(ctx, sizeof(*position));
  if (!position) return JS_EXCEPTION;

  if (JS_ToInt32(ctx, &position->x, argv[0]))
    goto fail;

  if (JS_ToInt32(ctx, &position->y, argv[1]))
    goto fail;

  /* using new_target to get the prototype is necessary when the
      class is extended. */
  proto = JS_GetPropertyStr(ctx, new_target, "prototype");
  if (JS_IsException(proto))
    goto fail;

  obj = JS_NewObjectProtoClass(ctx, proto, glfw_position_class_id);
  JS_FreeValue(ctx, proto);
  if (JS_IsException(obj))
    goto fail;

  JS_SetOpaque(obj, position);
  return obj;
 fail:
  js_free(ctx, position);
  JS_FreeValue(ctx, obj);
  return JS_EXCEPTION;
}

void glfw_position_finalizer(JSRuntime* rt, JSValue val) {
  GLFWPosition* position = JS_GetOpaque(val, glfw_position_class_id);
  js_free_rt(rt, position);
}

// Properties
JSValue glfw_position_get_xy(JSContext* ctx, JSValueConst this_val, int magic) {
  GLFWPosition* position = JS_GetOpaque2(ctx, this_val, glfw_position_class_id);
  if (!position) return JS_EXCEPTION;
  return JS_NewInt32(ctx, magic == 0 ? position->x : position->y);
}

JSValue glfw_position_set_xy(JSContext* ctx, JSValueConst this_val, JSValue val, int magic) {
  GLFWPosition* position = JS_GetOpaque2(ctx, this_val, glfw_position_class_id);
  if (!position) return JS_EXCEPTION;

  int value;
  if (JS_ToInt32(ctx, &value, val))
    return JS_EXCEPTION;

  if (magic == 0)
    position->x = value;
  else
    position->y = value;

  return JS_UNDEFINED;
}

// Initialization
JSClassDef glfw_position_class_def = {
  "Position",
  .finalizer = glfw_position_finalizer,
}; 

const JSCFunctionListEntry glfw_position_proto_funcs[] = {
  JS_CGETSET_MAGIC_DEF("x", glfw_position_get_xy, glfw_position_set_xy, 0),
  JS_CGETSET_MAGIC_DEF("y", glfw_position_get_xy, glfw_position_set_xy, 1),
};

JSValue glfw_position_proto, glfw_position_class;

JSValue glfw_position_constructor(JSContext* ctx) {
  JSRuntime* rt = JS_GetRuntime(ctx);

  if (!JS_IsRegisteredClass(rt, glfw_position_class_id)) {
    JS_NewClassID(&glfw_position_class_id);
    JS_NewClass(rt, glfw_position_class_id, &glfw_position_class_def);

    glfw_position_proto = JS_NewObject(ctx);
    JS_SetPropertyFunctionList(ctx, glfw_position_proto, glfw_position_proto_funcs, countof(glfw_position_proto_funcs));
    JS_SetClassProto(ctx, glfw_position_class_id, glfw_position_proto);
    
    glfw_position_class = JS_NewCFunction2(ctx, glfw_position_ctor, "Position", 2, JS_CFUNC_constructor, 0);
    /* set proto.constructor and ctor.prototype */
    JS_SetConstructor(ctx, glfw_position_class, glfw_position_proto);
  }

  return glfw_position_class;
}

int glfw_position_init(JSContext* ctx, JSModuleDef* m) {
  JS_SetModuleExport(ctx, m, "Position", glfw_position_constructor(ctx));
  return 0;
}

int glfw_position_export(JSContext* ctx, JSModuleDef* m) {
  return JS_AddModuleExport(ctx, m, "Position");
}
