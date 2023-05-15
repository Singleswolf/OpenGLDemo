//
// Created by User on 2023/3/3.
//

#ifndef OPENGLDEMO_FRAGMENTSHADER_H
#define OPENGLDEMO_FRAGMENTSHADER_H

static const char *vertexSimpleShape =
        "        #version 300 es\n"
        "        layout (location = 0) \n"
        "        in vec4 aPosition;//输入的顶点坐标，会在程序指定将数据输入到该字段\n"
        "\n"
        "        out\n"
        "        vec4 vTextColor;//输出的颜色\n"

        "        out\n"
        "        vec4 vPosition;//输出的坐标\n"
        "\n"
        "        void main() {\n"
        "            //直接把传入的坐标值作为传入渲染管线。gl_Position是OpenGL内置的\n"
        "            gl_Position = aPosition;\n"
        "            vPosition = aPosition;\n"
        "            gl_PointSize = 50.0;\n"
        //        "            vTextColor = vec4(aPosition.x ,aPosition.y ,aPosition.z,1.0);\n"
        "        }";

//图元被光栅化为多少片段，就被调用多少次
static const char *fragSimpleShape =
        "  #version 300 es\n"
        "        precision\n"
        "        mediump float;\n"
        "\n"
        "        in\n"
        "        vec4 vTextColor;//输入的颜色\n"
        "        out vec4 FragColor;\n"

        "        in\n"
        "        vec4 vPosition;//输入的坐标\n"

        "        void main() {\n"
        "            //gl_FragColor是OpenGL内置的\n"
        "            FragColor = vec4(vPosition.x ,vPosition.y ,vPosition.z,1.0);\n"
//        "           FragColor = vec4(1.0,0.5,0.5,1.0);"
        "        }";


static const char *vertexSimpleShapeWithColor =
        "        #version 300 es\n"
        "        layout (location = 0) \n"
        "        in vec4 aPosition;//输入的顶点坐标，会在程序指定将数据输入到该字段\n"
        "\n"
        "        layout (location = 1) \n"
        "        in vec4 aColor;"
        "        out\n"
        "        vec4 vTextColor;//输出的颜色\n"
        "        out\n"
        "        vec4 vPosition;//输出的坐标\n"
        "\n"
        "        void main() {\n"
        "            //直接把传入的坐标值作为传入渲染管线。gl_Position是OpenGL内置的\n"
        "            gl_Position = aPosition;\n"
        "            vPosition = aPosition;\n"
        "            gl_PointSize = 50.0;\n"
        "            vTextColor = aColor;\n"
        "        }";

//图元被光栅化为多少片段，就被调用多少次
static const char *fragSimpleShapeWithColor =
        "  #version 300 es\n"
        "        precision\n"
        "        mediump float;\n"
        "\n"
        "        in\n"
        "        vec4 vTextColor;//输入的颜色\n"
        "        out vec4 FragColor;\n"

        "        in\n"
        "        vec4 vPosition;//输入的坐标\n"

        "        void main() {\n"
        "            //gl_FragColor是OpenGL内置的\n"
        "            FragColor = vTextColor;\n"
        "        }";

#endif //OPENGLDEMO_FRAGMENTSHADER_H
