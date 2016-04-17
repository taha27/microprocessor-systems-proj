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

    public static String CLIENT_CHARACTERISTIC_CONFIG = "00002902-0000-1000-8000-00805f9b34fb";

    //Temperature Service (COPY_ENV_SENS_SERVICE_UUID) and  in sensor_service.c
    public static String TEMPERATURE_SERVICE = "42821a40-e477-11e2-82d0-0002a5d5c51b";
    //Temperature Characteristics (COPY_TEMP_CHAR_UUID)
    public static String TEMPERATURE_MEASUREMENT = "a32e5520-e477-11e2-a9e3-0002a5d5c51b";

    //Accelerometer Service (COPY_ACC_SERVICE_UUID)
    public static String ACCELEROMETER_SERVICE = "02366e80-cf3a-11e1-9ab4-0002a5d5c51b";
    //Accelerometer Characteristics (COPY_FREE_FALL_UUID and COPY_ACC_UUID)
    public static String PITCH_ANGLE_MEASUREMENT = "e23e78a0-cf4a-11e1-8ffc-0002a5d5c51b";
    public static String ROLL_ANGLE_MEASUREMENT = "340a1b80-cf4b-11e1-ac36-0002a5d5c51b";

    //LED Service
    public static String LED_SETTINGS_SERVICE = "ab366e80-cf3a-11e1-9ab4-0002a5d5c51b";
    //LED Characteristics
    public static String LED_SWITCH_SETTING = "bc366e80-cf3a-11e1-9ab4-0002a5d5c51b";
    public static String LED_SPEED_SETTING = "0d366e80-cf3a-11e1-9ab4-0002a5d5c51b";

    static {
        // LED Service and Characteristics
        attributes.put(LED_SETTINGS_SERVICE, "LED Settings Service");
        attributes.put(LED_SWITCH_SETTING, "LED Switch Setting");
        attributes.put(LED_SPEED_SETTING, "LED Speed Setting");

        // Temperature Sensor Services and Characteristics
        attributes.put(TEMPERATURE_SERVICE, "Temperature Service");
        attributes.put(TEMPERATURE_MEASUREMENT, "Temperature Measurement");

        // Accelerometer Service and Characteristics
        attributes.put(ACCELEROMETER_SERVICE, "Accelerometer Service");
        attributes.put(ROLL_ANGLE_MEASUREMENT, "Roll Angle Measurement");
        attributes.put(PITCH_ANGLE_MEASUREMENT, "Pitch Angle Measurement");
    }

    public static String lookup(String uuid, String defaultName) {
        String name = attributes.get(uuid);
        return name == null ? defaultName : name;
    }
}