export interface LightState {
  led_on: boolean;
}

export interface LightMqttSettings {
  unique_id: string;
  name: string;
  mqtt_path: string;
}

export interface AirConState {
    onoff: boolean;
    mode: string;
    temperature: number;
    flowspeed: number;
}

export interface AirConMqttSettings {
    unique_id: string;
    name: string;
    mqtt_path: string;
}
