package com.simple.opengldemo;

import android.os.Bundle;

import androidx.appcompat.app.AppCompatActivity;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'opengldemo' library on application startup.
    static {
        System.loadLibrary("opengldemo");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        IGLSurfaceView iglSurfaceView = new IGLSurfaceView(this, null);
        //setContentView(R.layout.activity_main);
        setContentView(iglSurfaceView);

        // Example of a call to a native method
        //TextView tv = binding.sampleText;
        //tv.setText(stringFromJNI());
    }
}