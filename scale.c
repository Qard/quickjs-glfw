#include "glfw.h"

#include "scale.h"

// Constructor/Destructor
JSValue glfw_scale_ctor(JSContext* ctx, JSValueConst new_target, int argc, JSValueConst* argv) {
  GLFWScale* scale;
  JSValue obj = JS_UNDEFINED;
  JSValue proto;
  
  scale = js_mallocz(ctx, sizeof(*scale));
  if (!scale) return JS_EXCEPTION;

  if (JS_ToFloat64(ctx, &scale->x, argv[0]))
    goto fail;

  if (JS_ToFloat64(ctx, &scale->y, argv[1]))
    goto fail;

  /* using new_target to get the prototype is necessary when the
      class is extended. */
  proto = JS_GetPropertyStr(ctx, new_target, "prototype");
  if (JS_IsException(proto))
    goto fail;

  obj = JS_NewObjectProtoClass(ctx, proto, glfw_scale_class_id);
  JS_FreeValue(ctx, proto);
  if (JS_IsException(obj))
    goto fail;

  JS_SetOpaque(obj, scale);
  return obj;
 fail:
  js_free(ctx, scale);
  JS_FreeValue(ctx, obj);
  return JS_EXCEPTION;
}

void glfw_scale_finalizer(JSRuntime* rt, JSValue val) {
  GLFWScale* scale = JS_GetOpaque(val, glfw_scale_class_id);
  js_free_rt(rt, scale);
}

// Properties
JSValue glfw_scale_get_axis(JSContext* ctx, JSValueConst this_val, int magic) {
  GLFWScale* scale = JS_GetOpaque2(ctx, this_val, glfw_scale_class_id);
  if (!scale) return JS_EXCEPTION;
  return JS_NewFloat64(ctx, magic == 0 ? scale->x : scale->y);
}

JSValue glfw_scale_set_axis(JSContext* ctx, JSValueConst this_val, JSValue val, int magic) {
  GLFWScale* scale = JS_GetOpaque2(ctx, this_val, glfw_scale_class_id);
  if (!scale) return JS_EXCEPTION;

  double value;
  if (JS_ToFloat64(ctx, &value, val))
    return JS_EXCEPTION;

  if (magic == 0)
    scale->x = value;
  else
    scale->y = value;

  return JS_UNDEFINED;
}

// Initialization
JSClassDef glfw_scale_class_def = {
  "Scale",
  .finalizer = glfw_scale_finalizer,
}; 

const JSCFunctionListEntry glfw_scale_proto_funcs[] = {
  JS_CGETSET_MAGIC_DEF("x", glfw_scale_get_axis, glfw_scale_set_axis, 0),
  JS_CGETSET_MAGIC_DEF("y", glfw_scale_get_axis, glfw_scale_set_axis, 1),
};

JSValue glfw_scale_proto, glfw_scale_class;

JSValue glfw_scale_constructor(JSContext* ctx) {
  JSRuntime* rt = JS_GetRuntime(ctx);

  if (!JS_IsRegisteredClass(rt, glfw_scale_class_id)) {
    JS_NewClassID(&glfw_scale_class_id);
    JS_NewClass(JS_GetRuntime(ctx), glfw_scale_class_id, &glfw_scale_class_def);

    glfw_scale_proto = JS_NewObject(ctx);
    JS_SetPropertyFunctionList(ctx, glfw_scale_proto, glfw_scale_proto_funcs, countof(glfw_scale_proto_funcs));
    JS_SetClassProto(ctx, glfw_scale_class_id, glfw_scale_proto);

    glfw_scale_class = JS_NewCFunction2(ctx, glfw_scale_ctor, "Scale", 2, JS_CFUNC_constructor, 0);
    /* set proto.constructor and ctor.prototype */
    JS_SetConstructor(ctx, glfw_scale_class, glfw_scale_proto);
  }

  return glfw_scale_class;
}

JSValue glfw_scale_new_instance(JSContext* ctx, GLFWScale* scale) {
  JSValue obj = JS_UNDEFINED;
  JSValue proto;

  proto = JS_GetPropertyStr(ctx, glfw_scale_constructor(ctx), "prototype");
  if (JS_IsException(proto))
    goto fail;

  obj = JS_NewObjectProtoClass(ctx, proto, glfw_scale_class_id);
  JS_FreeValue(ctx, proto);
  if (JS_IsException(obj))
    goto fail;

  JS_SetOpaque(obj, scale);

  return obj;
 fail:
  JS_FreeValue(ctx, obj);
  return JS_EXCEPTION;
}

int glfw_scale_init(JSContext* ctx, JSModuleDef* m) {
  JS_SetModuleExport(ctx, m, "Scale", glfw_scale_constructor(ctx));
  return 0;
}

int glfw_scale_export(JSContext* ctx, JSModuleDef* m) {
  return JS_AddModuleExport(ctx, m, "Scale");
}
