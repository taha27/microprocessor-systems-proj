package com.example.android.bluetoothlegatt;


import java.util.List;

public class ServiceDataItem {

    private String title;
    private String value;
    private int imageId;
    //testing methods
    ServiceDataItem(String title, String value, int imageId) {
        this.title = title;
        this.value = value;
        this.imageId = imageId;
    }

    public int getImageId() {
        return imageId;
    }

    public void setImageId(int imageId) {
        this.imageId = imageId;
    }

    public String getTitle() {
        return title;
    }

    public void setTitle(String title) {
        this.title = title;
    }

    public String getValue() {
        return value;
    }

    public void setValue(String value) {
        this.value = value;
    }
    
}
