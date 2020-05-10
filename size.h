#ifndef GLFW_MODULE_SIZE
#define GLFW_MODULE_SIZE 1

typedef struct {
  int width;
  int height;
} GLFWSize;

JSClassID glfw_size_class_id;
JSValue glfw_size_constructor(JSContext* ctx);
int glfw_size_init(JSContext* ctx, JSModuleDef* m);
int glfw_size_export(JSContext* ctx, JSModuleDef* m);

#endif
