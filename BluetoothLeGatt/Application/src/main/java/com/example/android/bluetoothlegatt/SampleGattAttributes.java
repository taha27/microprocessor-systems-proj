/*
 * Copyright (C) 2013 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.example.android.bluetoothlegatt;

import java.util.HashMap;

/**
 * This class includes a small subset of standard GATT attributes for demonstration purposes.
 */
public class SampleGattAttributes {
    private static HashMap<String, String> attributes = new HashMap();
    public static String HEART_RATE_MEASUREMENT = "00002a37-0000-1000-8000-00805f9b34fb";
    public static String CLIENT_CHARACTERISTIC_CONFIG = "00002902-0000-1000-8000-00805f9b34fb";

    //COPY_ENV_SENS_SERVICE_UUID and COPY_ACC_SERVICE_UUID in sensor_service.c
    public static String TEMPERATURE_SERVICE = "42821a40-e477-11e2-82d0-0002a5d5c51b";
    public static String ACCELEROMETER_SERVICE = "02366e80-cf3a-11e1-9ab4-0002a5d5c51b";

    //COPY_TEMP_CHAR_UUID, COPY_FREE_FALL_UUID and COPY_ACC_UUID respectively in sensor_service.c
    public static String TEMPERATURE_MEASUREMENT = "a32e5520-e477-11e2-a9e3-0002a5d5c51b";
    public static String ROLL_ANGLE_MEASUREMENT = "e23e78a0-cf4a-11e1-8ffc-0002a5d5c51b";
    public static String PITCH_ANGLE_MEASUREMENT = "340a1b80-cf4b-11e1-ac36-0002a5d5c51b";

    static {
        // Sample Services.
        attributes.put("0000180d-0000-1000-8000-00805f9b34fb", "Heart Rate Service");
        attributes.put("0000180a-0000-1000-8000-00805f9b34fb", "Device Information Service");

        // Temp and Acc Services
        attributes.put(TEMPERATURE_SERVICE, "Temperature Service");
        attributes.put(ACCELEROMETER_SERVICE, "Accelerometer Service");

        // Sample Characteristics.
        attributes.put(HEART_RATE_MEASUREMENT, "Heart Rate Measurement");
        attributes.put("00002a29-0000-1000-8000-00805f9b34fb", "Manufacturer Name String");

        // Temp and Acc Characteristics
        attributes.put(TEMPERATURE_MEASUREMENT, "Temperature Measurement");
        attributes.put(ROLL_ANGLE_MEASUREMENT, "Roll Angle Measurement");
        attributes.put(PITCH_ANGLE_MEASUREMENT, "Pitch Angle Measurement");

    }

    public static String lookup(String uuid, String defaultName) {
        String name = attributes.get(uuid);
        return name == null ? defaultName : name;
    }
}
