/***********************************************************
 ** File: - ResReadUtils.java
 ** Description: read glsl resources to String
 ** Version: 1.0
 ** Date : 2021/09/01
 ** Author: WangLeqi@CameraDemo
 **
 ** ----------------------Revision History: --------------------
 **   <author>           <date>     <version >       <desc>
 **    WangLeqi       2021/09/01     1.0           first add
 ****************************************************************/
package com.neutron.afocamera.utils;

import android.app.Application;
import android.content.res.Resources;

import com.neutron.afocamera.App;
import com.neutron.afocamera.MainActivity;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;

public class ResReadUtils {

    /**
     * 读取资源
     *
     * @param resourceId
     * @return
     */
    public static String readResource(int resourceId) {
        StringBuilder builder = new StringBuilder();
        try {
            InputStream inputStream = App.getInstance().getResources().openRawResource(resourceId);
            InputStreamReader streamReader = new InputStreamReader(inputStream);

            BufferedReader bufferedReader = new BufferedReader(streamReader);
            String textLine;
            while ((textLine = bufferedReader.readLine()) != null) {
                builder.append(textLine);
                builder.append("\n");
            }
        } catch (IOException e) {
            e.printStackTrace();
        } catch (Resources.NotFoundException e) {
            e.printStackTrace();
        }
        return builder.toString();
    }

}
