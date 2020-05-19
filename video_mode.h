#ifndef GLFW_MODULE_VIDEO_MODE
#define GLFW_MODULE_VIDEO_MODE 1

JSClassID glfw_video_mode_class_id;
JSValue glfw_video_mode_new_instance(JSContext* ctx, const GLFWvidmode* mode);
int glfw_video_mode_init(JSContext* ctx, JSModuleDef* m);
int glfw_video_mode_export(JSContext* ctx, JSModuleDef* m);

#endif
