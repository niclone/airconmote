import { FC } from 'react';

import { Switch, Slider } from '@mui/material';

import { WEB_SOCKET_ROOT } from '../api/endpoints';
import { BlockFormControlLabel, FormLoader, MessageBox, SectionContent } from '../components';
import { updateValue, useWs } from '../utils';

import { AirConState } from './types';

export const AIRCON_SETTINGS_WEBSOCKET_URL = WEB_SOCKET_ROOT + "airConState";

const AirConStateWebSocketForm: FC = () => {
  const { connected, updateData, data } = useWs<AirConState>(AIRCON_SETTINGS_WEBSOCKET_URL);

  const updateFormValue = updateValue(updateData);

  const temperatureMarks = [
    {
      value: 10,
      label: '10°C',
    },
    {
      value: 15,
      label: '15°C',
    },
    {
      value: 20,
      label: '20°C',
    },
    {
      value: 25,
      label: '25°C',
    },
    {
      value: 30,
      label: '30°C',
    },
  ];

  function temperatureText(value: number) {
    return `${value}°C`;
  }

  const content = () => {
    if (!connected || !data) {
      return (<FormLoader message="Connecting to WebSocket…" />);
    }
    return (
      <>
        <MessageBox
          level="info"
          message="The switch below controls the LED via the WebSocket. It will automatically update whenever the LED state changes."
          my={2}
        />
        <BlockFormControlLabel
          control={
                <Slider
                    name="temperature"
                    aria-label="Temperature"
                    getAriaValueText={temperatureText}
                    step={0.5}
                    marks={temperatureMarks}
                    valueLabelDisplay="on"
                    min={10}
                    max={30}
                    />
          }
          label="Temperature"
        />
        <BlockFormControlLabel
          control={
            <Switch
              name="led_on"
              //checked={data.led_on}
              onChange={updateFormValue}
              color="primary"
            />
          }
          label="LED State?"
        />
      </>
    );
  };

  return (
    <SectionContent title='WebSocket Example' titleGutter>
      {content()}
    </SectionContent>
  );
};

export default AirConStateWebSocketForm;
