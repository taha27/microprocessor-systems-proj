package com.example.android.bluetoothlegatt;


import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.support.v7.widget.CardView;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import java.util.ArrayList;
import java.util.List;

public class MenuListActivity extends Activity {

    public static final String EXTRAS_DEVICE_NAME = "DEVICE_NAME";
    public static final String EXTRAS_DEVICE_ADDRESS = "DEVICE_ADDRESS";

    private String mDeviceName;
    private String mDeviceAddress;

    private List<ServiceDataItem>  serviceDataItemList;

    private RecyclerView rv;
    private RecyclerView.Adapter mAdapter;
    private RecyclerView.LayoutManager mLayoutManager;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.listitem_features);

        Intent intent = getIntent();
        mDeviceName = intent.getStringExtra(EXTRAS_DEVICE_NAME);
        mDeviceAddress = intent.getStringExtra(EXTRAS_DEVICE_ADDRESS);

        rv = (RecyclerView) findViewById(R.id.rv);

        mLayoutManager = new LinearLayoutManager(this);
        rv.setLayoutManager(mLayoutManager);
        rv.setHasFixedSize(true);

        //tests data
        serviceDataItemList = new ArrayList<>();
        serviceDataItemList.add(new ServiceDataItem("Tilt", "23"));
        serviceDataItemList.add(new ServiceDataItem("Roll", "25"));
        serviceDataItemList.add(new ServiceDataItem("Temperature", "35"));
        serviceDataItemList.add(new ServiceDataItem("LED", "set values"));


        mAdapter = new ServiceItemAdapter(serviceDataItemList);
        rv.setAdapter(mAdapter);
    }

}
