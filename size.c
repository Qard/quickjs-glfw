#include "glfw.h"

#include "size.h"

// Constructor/Destructor
JSValue glfw_size_ctor(JSContext* ctx, JSValueConst new_target, int argc, JSValueConst* argv) {
  GLFWSize* size;
  JSValue obj = JS_UNDEFINED;
  JSValue proto;
  
  size = js_mallocz(ctx, sizeof(*size));
  if (!size) return JS_EXCEPTION;

  if (JS_ToInt32(ctx, &size->width, argv[0]))
    goto fail;

  if (JS_ToInt32(ctx, &size->height, argv[1]))
    goto fail;

  /* using new_target to get the prototype is necessary when the
      class is extended. */
  proto = JS_GetPropertyStr(ctx, new_target, "prototype");
  if (JS_IsException(proto))
    goto fail;

  obj = JS_NewObjectProtoClass(ctx, proto, glfw_size_class_id);
  JS_FreeValue(ctx, proto);
  if (JS_IsException(obj))
    goto fail;

  JS_SetOpaque(obj, size);
  return obj;
 fail:
  js_free(ctx, size);
  JS_FreeValue(ctx, obj);
  return JS_EXCEPTION;
}

void glfw_size_finalizer(JSRuntime* rt, JSValue val) {
  GLFWSize* size = JS_GetOpaque(val, glfw_size_class_id);
  js_free_rt(rt, size);
}

// Properties
JSValue glfw_size_get_axis(JSContext* ctx, JSValueConst this_val, int magic) {
  GLFWSize* size = JS_GetOpaque2(ctx, this_val, glfw_size_class_id);
  if (!size) return JS_EXCEPTION;
  return JS_NewInt32(ctx, magic == 0 ? size->width : size->height);
}

JSValue glfw_size_set_axis(JSContext* ctx, JSValueConst this_val, JSValue val, int magic) {
  GLFWSize* size = JS_GetOpaque2(ctx, this_val, glfw_size_class_id);
  if (!size) return JS_EXCEPTION;

  int value;
  if (JS_ToInt32(ctx, &value, val))
    return JS_EXCEPTION;

  if (magic == 0)
    size->width = value;
  else
    size->height = value;

  return JS_UNDEFINED;
}

// Initialization
JSClassDef glfw_size_class_def = {
  "Size",
  .finalizer = glfw_size_finalizer,
}; 

const JSCFunctionListEntry glfw_size_proto_funcs[] = {
  JS_CGETSET_MAGIC_DEF("width", glfw_size_get_axis, glfw_size_set_axis, 0),
  JS_CGETSET_MAGIC_DEF("height", glfw_size_get_axis, glfw_size_set_axis, 1),
};

JSValue glfw_size_proto, glfw_size_class;

JSValue glfw_size_constructor(JSContext* ctx) {
  JSRuntime* rt = JS_GetRuntime(ctx);

  if (!JS_IsRegisteredClass(rt, glfw_size_class_id)) {
    JS_NewClassID(&glfw_size_class_id);
    JS_NewClass(JS_GetRuntime(ctx), glfw_size_class_id, &glfw_size_class_def);

    glfw_size_proto = JS_NewObject(ctx);
    JS_SetPropertyFunctionList(ctx, glfw_size_proto, glfw_size_proto_funcs, countof(glfw_size_proto_funcs));
    JS_SetClassProto(ctx, glfw_size_class_id, glfw_size_proto);

    glfw_size_class = JS_NewCFunction2(ctx, glfw_size_ctor, "Size", 2, JS_CFUNC_constructor, 0);
    /* set proto.constructor and ctor.prototype */
    JS_SetConstructor(ctx, glfw_size_class, glfw_size_proto);
  }

  return glfw_size_class;
}

JSValue glfw_size_new_instance(JSContext* ctx, GLFWSize* size) {
  JSValue obj = JS_UNDEFINED;
  JSValue proto;

  proto = JS_GetPropertyStr(ctx, glfw_size_constructor(ctx), "prototype");
  if (JS_IsException(proto))
    goto fail;

  obj = JS_NewObjectProtoClass(ctx, proto, glfw_size_class_id);
  JS_FreeValue(ctx, proto);
  if (JS_IsException(obj))
    goto fail;

  JS_SetOpaque(obj, size);

  return obj;
 fail:
  JS_FreeValue(ctx, obj);
  return JS_EXCEPTION;
}

int glfw_size_init(JSContext* ctx, JSModuleDef* m) {
  JS_SetModuleExport(ctx, m, "Size", glfw_size_constructor(ctx));
  return 0;
}

int glfw_size_export(JSContext* ctx, JSModuleDef* m) {
  return JS_AddModuleExport(ctx, m, "Size");
}
