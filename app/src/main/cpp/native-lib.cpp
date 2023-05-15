#include <jni.h>
#include <string>
#include <android/native_window_jni.h>
#include <GLES3/gl3.h>
#include <EGL/egl.h>
#include <EGL/eglplatform.h>
#include <android/log.h>
#include "FragmentShader.h"


#define TAG "GL_DEMO_NA"
#define LOGV(format, ...) __android_log_print(ANDROID_LOG_VERBOSE, TAG,"[%d]: " format,  __LINE__, ##__VA_ARGS__);
#define LOGD(format, ...) __android_log_print(ANDROID_LOG_DEBUG, TAG,"[%d]: " format,  __LINE__, ##__VA_ARGS__);
#define LOGI(format, ...) __android_log_print(ANDROID_LOG_INFO, TAG,"[%d]: " format,  __LINE__, ##__VA_ARGS__);
#define LOGW(format, ...) __android_log_print(ANDROID_LOG_WARN, TAG,"[%d]: " format,  __LINE__, ##__VA_ARGS__);
#define LOGE(format, ...) __android_log_print(ANDROID_LOG_ERROR, TAG,"[%d]: " format,  __LINE__, ##__VA_ARGS__);

GLint initShader(const char *source, GLint type);
EGLBoolean initConfig(JNIEnv *env, jobject surface, EGLDisplay display, EGLSurface winSurface) {
    /*EGL配置 start*/

    //1.获取原始窗口，ANativeWindow就是Surface在Native的对应物。这里的surface参数即从Java层穿过来的Surface对象
    EGLNativeWindowType win = ANativeWindow_fromSurface(env, surface);
    //获取OpenGL es渲染目标Display，EGL_DEFAULT_DISPLAY表示默认的Display
    display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    if (display == EGL_NO_DISPLAY) {
        LOGD("egl display failed")
        return EGL_FALSE;
    }

    //2.初始化egl Diaplay的连接，后两个参数是指针，是分别用来返回EGL主次版本号
    if (EGL_TRUE != eglInitialize(display, 0, 0)) {
        LOGD("eglInitialize failed")
        return EGL_FALSE;
    }

    //返回的EGL帧缓存配置
    EGLConfig eglConfig;
    //配置数量
    EGLint configNum;

    //期望的EGL帧缓存配置列表,配置为一个key一个value的形式，以下的EGL_RED_SIZE、EGL_GREEN_SIZE、EGL_BLUE_SIZE分别表示EGL帧缓冲中的颜色缓冲一个颜色通道用多少位表示。
    //指定EGL surface类型
    EGLint configSpec[] = {
            EGL_RED_SIZE, 4,
            EGL_GREEN_SIZE, 4,
            EGL_BLUE_SIZE, 4,
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_NONE
    };

    //返回一个和期望的EGL帧缓存配置列表configSpec匹配的EGL帧缓存配置列表，存储在eglConfig中
    if (EGL_TRUE != eglChooseConfig(display, configSpec, &eglConfig, 1, &configNum)) {
        LOGD("eglChooseConfig failed")
        return EGL_FALSE;
    }

    int surfaceAttribs[] = {
            EGL_NONE
    };
    //通过egl和NativeWindow以及EGL帧缓冲配置创建EGLSurface。最后一个参数为属性信息，0表示不需要属性)
    winSurface = eglCreateWindowSurface(display, eglConfig, win, surfaceAttribs);
    if (winSurface == EGL_NO_SURFACE) {
        LOGD("eglCreateWindowSurface failed")
        return EGL_FALSE;
    }

    //渲染上下文EGLContext关联的帧缓冲配置列表，EGL_CONTEXT_CLIENT_VERSION表示这里是配置EGLContext的版本，
    const EGLint ctxAttr[] = {
            EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE
    };

    //通过Display和上面获取到的的EGL帧缓存配置列表创建一个EGLContext， EGL_NO_CONTEXT表示不需要多个设备共享上下文
    EGLContext context = eglCreateContext(display, eglConfig, EGL_NO_CONTEXT, ctxAttr);
    if (context == EGL_NO_CONTEXT) {
        LOGD("eglCreateContext failed")
        return EGL_FALSE;
    }

    //将EGLContext和当前线程以及draw和read的EGLSurface关联，关联之后，当前线程就成为了OpenGL es的渲染线程
    if (EGL_TRUE != eglMakeCurrent(display, winSurface, winSurface, context)) {
        LOGD("eglMakeCurrent failed")
        return EGL_FALSE;
    }
    /*EGL配置 end*/

    LOGD("initConfig success")
    return EGL_TRUE;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_simple_opengldemo_IGLSurfaceView_drawTriangle(JNIEnv *env, jobject thiz, jobject surface) {

    /*EGL 绘制流程简介
    获取 EGL Display 对象：eglGetDisplay
    初始化与 EGLDisplay 之间的连接：eglInitialize
    获取 EGLConfig 对象：eglChooseConfig / eglGetConfigs
    创建 EGLContext 实例：eglCreateContext
    创建 EGLSurface 实例：eglCreateWindowSurface / eglCreatePbufferSurface
    连接 EGLContext 和 EGLSurface：eglMakeCurrent
    使用 OpenGL ES API 绘制图形：gl_*
    切换 front buffer 和 back buffer 显示：eglSwapBuffer
    断开并释放与 EGLSurface 关联的 EGLContext 对象：eglRelease
    删除 EGLSurface 对象
    删除 EGLContext 对象
    终止与 EGLDisplay 之间的连接*/

    /*EGL配置 start*/

    //1.获取原始窗口，ANativeWindow就是Surface在Native的对应物。这里的surface参数即从Java层穿过来的Surface对象
    EGLNativeWindowType win = ANativeWindow_fromSurface(env, surface);
    //获取OpenGL es渲染目标Display，EGL_DEFAULT_DISPLAY表示默认的Display
    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    if (display == EGL_NO_DISPLAY) {
        LOGD("egl display failed")
        return;
    }

    //2.初始化egl Diaplay的连接，后两个参数是指针，是分别用来返回EGL主次版本号
    if (EGL_TRUE != eglInitialize(display, 0, 0)) {
        LOGD("eglInitialize failed")
        return;
    }

    //返回的EGL帧缓存配置
    EGLConfig eglConfig;
    //配置数量
    EGLint configNum;

    //期望的EGL帧缓存配置列表,配置为一个key一个value的形式，以下的EGL_RED_SIZE、EGL_GREEN_SIZE、EGL_BLUE_SIZE分别表示EGL帧缓冲中的颜色缓冲一个颜色通道用多少位表示。
    //指定EGL surface类型
    EGLint configSpec[] = {
            EGL_RED_SIZE, 4,
            EGL_GREEN_SIZE, 4,
            EGL_BLUE_SIZE, 4,
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_NONE
    };

    //返回一个和期望的EGL帧缓存配置列表configSpec匹配的EGL帧缓存配置列表，存储在eglConfig中
    if (EGL_TRUE != eglChooseConfig(display, configSpec, &eglConfig, 1, &configNum)) {
        LOGD("eglChooseConfig failed")
        return;
    }

    int surfaceAttribs[] = {
            EGL_NONE
    };
    //通过egl和NativeWindow以及EGL帧缓冲配置创建EGLSurface。最后一个参数为属性信息，0表示不需要属性)
    EGLSurface winSurface = eglCreateWindowSurface(display, eglConfig, win, surfaceAttribs);
    if (winSurface == EGL_NO_SURFACE) {
        LOGD("eglCreateWindowSurface failed")
        return;
    }

    //渲染上下文EGLContext关联的帧缓冲配置列表，EGL_CONTEXT_CLIENT_VERSION表示这里是配置EGLContext的版本，
    const EGLint ctxAttr[] = {
            EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE
    };

    //通过Display和上面获取到的的EGL帧缓存配置列表创建一个EGLContext， EGL_NO_CONTEXT表示不需要多个设备共享上下文
    EGLContext context = eglCreateContext(display, eglConfig, EGL_NO_CONTEXT, ctxAttr);
    if (context == EGL_NO_CONTEXT) {
        LOGD("eglCreateContext failed")
        return;
    }

    //将EGLContext和当前线程以及draw和read的EGLSurface关联，关联之后，当前线程就成为了OpenGL es的渲染线程
    if (EGL_TRUE != eglMakeCurrent(display, winSurface, winSurface, context)) {
        LOGD("eglMakeCurrent failed")
        return;
    }

    /*EGL配置 end*/


    //分别创建和编译顶点着色器、片段着色器对象
    GLint vsh = initShader(vertexSimpleShape, GL_VERTEX_SHADER);
    GLint fsh = initShader(fragSimpleShape, GL_FRAGMENT_SHADER);

    //创建着色器程序对象
    GLint program = glCreateProgram();
    if (program == 0) {
        LOGD("glCreateProgram failed");
        return;
    }

    //将着色器对象关联到着色器程序对象上
    glAttachShader(program, vsh);
    glAttachShader(program, fsh);

    //链接着色器程序
    glLinkProgram(program);

    //打印链接异常现象
    GLint status = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status == 0) {
        LOGD("glLinkProgram failed")
        return;
    }
    LOGD("glLinkProgram success")

    //使用着色器对象
    glUseProgram(program);

    static float triangleVer[] = {
//            0.8f, -0.8f, 0.0f,//x,y,z
//            -0.8f, -0.8f, 0.0f,
//            0.0f, 0.8f, 0.0f,

            0.0f, 0.0f, 0.0f,
            0.8f, 0.0f, 0.0f,
            0.0f, 0.8f, 0.0f,
            0.8f, 0.8f, 0.0f,
    };
    //三角形坐标位置
    //指定接收三角形坐标的变量名，program为上面的编译链接好的着色器程序
/*    GLint apos = static_cast<GLuint>(glGetAttribLocation(program, "aPosition"));
    glEnableVertexAttribArray(apos);
    glVertexAttribPointer(apos,//色器中要接收数据的变量的引用
                          3,//每一个顶点属性需要用多少个数组元素表示。比如一个3维坐标是xyz表示，那么size就是3，即3个数可以表示一个点坐标
                          GL_FLOAT,//每一个数组元素的格式是什么，比如GL_HALF_FLOAT, GL_FLOAT, GL_DOUBLE等
                          GL_FALSE,//是否需要归一化，即是否需要将数据范围映射到-1到1的区间内。
                          0,//步长，一个重要概念，表示前一个顶点属性的起始位置到下一个顶点属性的起始位置在数组中有多少字节。如果传0，则说明顶点属性数据是紧密挨着的。
                          triangleVer);*/
    //通过layout传输数据，传给了着色器中layout为0的变量
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, triangleVer);
    //打开layout为0的变量传输开关
    glEnableVertexAttribArray(0);

    //清空屏幕和颜色缓冲区
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);//将颜色缓冲清空为什么颜色，参数为对应的RGBA值
    glClear(GL_COLOR_BUFFER_BIT);//GL_COLOR_BUFFER_BIT、GL_DEPTH_BUFFER_BIT 和 GL_STENCIL_BUFFER_BIT，分别表示颜色缓冲、深度缓冲、模板缓冲

    //绘制三角形
//    glDrawArrays(GL_LINE_LOOP, 0, 3);//3个顶点连线闭合，没有填充颜色
//    glDrawArrays(GL_LINE_STRIP, 0, 3);//3个顶点连线不闭合，没有填充颜色

    //GL_TRIANGLES 是每三个顶点就分别连成一个三角形，也就是说把顶点数组每三个为一组分，互相之间没有瓜葛
    //glDrawArrays(GL_TRIANGLES, 0, 3);//3个顶点连线不闭合，填充颜色

    //GL_TRIANGLE_STRIP 是从第0个顶点开始，每3个顶点连在一起形成三角形，上面4个顶点可以组成2个三角形成四边形v0(1,2,3) v1(2,3,4)
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);//长方形

    //GL_TRIANGLE_FAN 是第一个顶点会和后面每2个顶点为一组绘制三角形v0(1,2,3) v1(1,3,4)
//    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    //窗口显示，交换双缓冲区
    eglSwapBuffers(display, winSurface);

    glDeleteProgram(program);
}

GLint initShader(const char *source, GLint type) {
    //创建着色器对象，type表示着色器类型，比如顶点着色器为GL_VERTEX_SHADER，片段着色器为GL_FRAGMENT_SHADER。返回值为一个类似引用的数字。
    GLint shader = glCreateShader(type);
    if (0 == shader) {
        LOGD("glCreateShader %d failed", type)
        return 0;
    }

    //着色器对象加载着色器对象代码source
    glShaderSource(shader,
                   1,//shader数量
                   &source,
                   0);//代码长度，传0则读到字符串结尾

    //编译着色器对象
    glCompileShader(shader);

    //以下为打印出编译异常信息
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == 0) {
        LOGD("glCompileShader %d failed", type);
        LOGD("source %s", source);
        auto *infoLog = new GLchar[1024];
        GLsizei length;
        glGetShaderInfoLog(shader, 1024, &length, infoLog);

        LOGD("ERROR::SHADER::VERTEX::COMPILATION_FAILED %s", infoLog);
        return 0;
    }

    LOGD("glCompileShader %d success", type);

    return shader;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_simple_opengldemo_IGLSurfaceView_drawTriangleWithColor(JNIEnv *env, jobject thiz, jobject surface) {

    EGLDisplay display;
    EGLSurface winSurface;
    EGLBoolean init = initConfig(env, surface, &display, &winSurface);

    if (init == EGL_FALSE) {
        return;
    }


    //分别创建和编译顶点着色器、片段着色器对象
    GLint vsh = initShader(vertexSimpleShapeWithColor, GL_VERTEX_SHADER);
    GLint fsh = initShader(fragSimpleShapeWithColor, GL_FRAGMENT_SHADER);

    //创建着色器程序对象
    GLint program = glCreateProgram();
    if (program == 0) {
        LOGD("glCreateProgram failed");
        return;
    }

    //将着色器对象关联到着色器程序对象上
    glAttachShader(program, vsh);
    glAttachShader(program, fsh);

    //链接着色器程序
    glLinkProgram(program);

    //打印链接异常现象
    GLint status = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status == 0) {
        LOGD("glLinkProgram failed")
        return;
    }
    LOGD("glLinkProgram success")

    //使用着色器对象
    glUseProgram(program);

    static float triangleVerWithColor[] = {
//            0.8f, -0.8f, 0.0f,//x,y,z
//            -0.8f, -0.8f, 0.0f,
//            0.0f, 0.8f, 0.0f,

            0.0f, 0.0f, 0.0f,//顶点
            1.0, 0.0, 0.0,//颜色

            0.8f, 0.0f, 0.0f,
            0.0, 1.0, 0.0,//颜色

            0.0f, 0.8f, 0.0f,
            0.0, 0.0, 1.0,//颜色
    };
    //三角形坐标位置
    //指定接收三角形坐标的变量名，program为上面的编译链接好的着色器程序
/*    GLint apos = static_cast<GLuint>(glGetAttribLocation(program, "aPosition"));
    glEnableVertexAttribArray(apos);
    glVertexAttribPointer(apos,//色器中要接收数据的变量的引用
                          3,//每一个顶点属性需要用多少个数组元素表示。比如一个3维坐标是xyz表示，那么size就是3，即3个数可以表示一个点坐标
                          GL_FLOAT,//每一个数组元素的格式是什么，比如GL_HALF_FLOAT, GL_FLOAT, GL_DOUBLE等
                          GL_FALSE,//是否需要归一化，即是否需要将数据范围映射到-1到1的区间内。
                          0,//步长，一个重要概念，表示前一个顶点属性的起始位置到下一个顶点属性的起始位置在数组中有多少字节。如果传0，则说明顶点属性数据是紧密挨着的。
                          triangleVerWithColor);*/
    //通过layout传输数据，传给了着色器中layout为0的变量
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 24, triangleVerWithColor);
    //解析颜色数据
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 24, triangleVerWithColor + 3);
    //打开layout为0的变量传输开关
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    //清空屏幕和颜色缓冲区
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);//将颜色缓冲清空为什么颜色，参数为对应的RGBA值
    glClear(GL_COLOR_BUFFER_BIT);//GL_COLOR_BUFFER_BIT、GL_DEPTH_BUFFER_BIT 和 GL_STENCIL_BUFFER_BIT，分别表示颜色缓冲、深度缓冲、模板缓冲

    //绘制三角形
//    glDrawArrays(GL_LINE_LOOP, 0, 3);//3个顶点连线闭合，没有填充颜色
//    glDrawArrays(GL_LINE_STRIP, 0, 3);//3个顶点连线不闭合，没有填充颜色

    //GL_TRIANGLES 是每三个顶点就分别连成一个三角形，也就是说把顶点数组每三个为一组分，互相之间没有瓜葛
    //glDrawArrays(GL_TRIANGLES, 0, 3);//3个顶点连线不闭合，填充颜色

    //GL_TRIANGLE_STRIP 是从第0个顶点开始，每3个顶点连在一起形成三角形，上面4个顶点可以组成2个三角形成四边形v0(1,2,3) v1(2,3,4)
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);//长方形

    //GL_TRIANGLE_FAN 是第一个顶点会和后面每2个顶点为一组绘制三角形v0(1,2,3) v1(1,3,4)
//    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    //窗口显示，交换双缓冲区
    eglSwapBuffers(display, winSurface);

    glDeleteProgram(program);
}