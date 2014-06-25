package org.cocos2dx.cpp;

import android.app.Activity;
import android.app.NativeActivity;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.SurfaceView;

import org.cocos2dx.*;

public class Cocos2dxActivity extends NativeActivity{

	private GLSurfaceView touchView;
	
	@Override
    public boolean onTouchEvent(MotionEvent event) {
    	
    	Log.d("start", "onGenericMotionEvent: " + event);
    	
        if ((event.getAction() & MotionEvent.ACTION_MASK) == MotionEvent.ACTION_POINTER_DOWN) {
            finish();
            return true;
        }

        touchView.onTouchEvent(event);
        return false;
    }   
	
    @Override
    public boolean onKeyDown(int keycode, KeyEvent event) {
    	Log.d("start", "start");
        
    	if (keycode == KeyEvent.KEYCODE_DPAD_CENTER) {
            // user tapped touchpad, do something
            return true;
        }
        
        super.onKeyDown(keycode, event);
        
        return false;
    }
    
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		
		touchView = new GLSurfaceView(this);

		//For supports translucency
		
		//1.change "attribs" in cocos\2d\platform\android\nativeactivity.cpp
		/*const EGLint attribs[] = {
	            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
	            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,  
	            //EGL_BLUE_SIZE, 5,   -->delete 
	            //EGL_GREEN_SIZE, 6,  -->delete 
	            //EGL_RED_SIZE, 5,    -->delete 
	            EGL_BUFFER_SIZE, 32,  //-->new field
	            EGL_DEPTH_SIZE, 16,
	            EGL_STENCIL_SIZE, 8,
	            EGL_NONE
	    };*/
		
		//2.Set the format of window
		// getWindow().setFormat(PixelFormat.TRANSLUCENT);
		
		Log.e ("start1", "start1");
        
	}
}
