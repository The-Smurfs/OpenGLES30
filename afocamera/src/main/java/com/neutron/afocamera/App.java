/***********************************************************
 ** File: - App.java
 ** Description: app
 ** Version: 1.0
 ** Date : 2021/08/23
 ** Author: WangLeqi@CameraDemo
 **
 ** ----------------------Revision History: --------------------
 **   <author>           <date>     <version >       <desc>
 **    WangLeqi       2021/08/23     1.0           first add
 ****************************************************************/
package com.neutron.afocamera;

import android.app.Application;

public class App extends Application {
    static App app;


    public static App getInstance() {
        return app;
    }

    @Override
    public void onCreate() {
        super.onCreate();
        app = this;

    }
}
