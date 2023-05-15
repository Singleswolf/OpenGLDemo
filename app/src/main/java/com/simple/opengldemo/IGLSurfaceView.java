package com.simple.opengldemo;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;
import android.util.Log;
import android.view.Surface;
import android.view.SurfaceHolder;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * Created by yong on 2023/3/3 15:21.
 */
public class IGLSurfaceView extends GLSurfaceView implements Runnable, SurfaceHolder.Callback, GLSurfaceView.Renderer {
    private final String TAG = "IGLSurfaceView";
    public IGLSurfaceView(Context context, AttributeSet attrs) {
        super(context, attrs);
        setRenderer(this);
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        //super.surfaceCreated(holder);
        new Thread(this).start();
    }

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        Log.i(TAG, "onSurfaceCreated: ");
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        Log.i(TAG, "onSurfaceChanged: ");
    }

    @Override
    public void onDrawFrame(GL10 gl) {
    }

    @Override
    public void run() {
        //drawTriangle(getHolder().getSurface());
        drawTriangleWithColor(getHolder().getSurface());
    }

    private native void drawTriangle(Surface surface);
    private native void drawTriangleWithColor(Surface surface);
}
