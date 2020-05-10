#ifndef GLFW_MODULE_WINDOW
#define GLFW_MODULE_WINDOW 1

JSClassID glfw_window_class_id;
int glfw_window_init(JSContext* ctx, JSModuleDef* m);
int glfw_window_export(JSContext* ctx, JSModuleDef* m);

JSValue glfw_window_new_instance(JSContext* ctx, GLFWwindow* window);
JSValue glfw_window_create_window(JSContext* ctx, int width, int height,
                                 const char* title, GLFWmonitor* monitor,
                                 GLFWwindow* share);

#endif
