import { FC, useEffect, useState } from 'react';

import { Switch, Slider, Box, ToggleButton, ToggleButtonGroup } from '@mui/material';

import ThermostatAutoIcon from '@mui/icons-material/ThermostatAuto';
import AcUnitIcon from '@mui/icons-material/AcUnit';
import WbSunnyIcon from '@mui/icons-material/WbSunny';
import FormatColorResetIcon from '@mui/icons-material/FormatColorReset';
import AirIcon from '@mui/icons-material/Air';

import HdrAutoOutlinedIcon from '@mui/icons-material/HdrAutoOutlined';
import HearingDisabledIcon from '@mui/icons-material/HearingDisabled';
import TuneIcon from '@mui/icons-material/Tune';

import { WEB_SOCKET_ROOT } from '../api/endpoints';
import { BlockFormControlLabel, FormLoader, MessageBox, SectionContent } from '../components';
import Temperature from '../components/inputs/Temperature';
import AdvancedRegisters from '../components/advanced/AdvancedRegisters';
import { updateValue, useWs } from '../utils';
//import ToggleButton from '@mui/material/ToggleButton';
//import ToggleButtonGroup from '@mui/material/ToggleButtonGroup';

import './AirConStateWebSocketForm.css';

import { AirConState } from './types';
import { MapsHomeWork } from '@mui/icons-material';

export const AIRCON_SETTINGS_WEBSOCKET_URL = WEB_SOCKET_ROOT + "airConState";

const AirConStateWebSocketForm: FC = () => {
  const { connected, updateData, data } = useWs<AirConState>(AIRCON_SETTINGS_WEBSOCKET_URL);
  const saving = false; // hack

  const updateFormValue = updateValue(updateData);
  const sxBlockForm={
    ".MuiFormControlLabel-label": {
        width: 125,
    },
  };

  function temperatureText(value: number) {
    return `${value}°C`;
  }

  const content = () => {
    if (!connected || !data) {
      return (<FormLoader message="Connecting to WebSocket…" />);
    }

    return (
        <>
          <div>
            Temperature indoor / outdoor : {temperatureText(data.sensor_temp_inside)} / {temperatureText(data.sensor_temp_outside)}
          </div>
          <BlockFormControlLabel
            control={
              <Switch
                name="onoff"
                disabled={saving}
                checked={data.onoff}
                onChange={(ev) => updateData({...data!, onoff: ev.target.checked})}
                color="primary"
              />
            }
            label="Power"
            labelPlacement='start'
            sx={sxBlockForm}
          />
          <BlockFormControlLabel
            control={
              <ToggleButtonGroup
              exclusive
              aria-label="AirCon Mode"
              sx={{ margin: 1 }}
              onChange={(ev, v) => updateData({...data!, mode: ""+v}) }
              >
              <ToggleButton value="auto" aria-label="Automatic" selected={data.mode === "auto"}>
                <ThermostatAutoIcon />
              </ToggleButton>
              <ToggleButton value="snow" aria-label="Air Conditionned" selected={data.mode === "snow"}>
                <AcUnitIcon />
              </ToggleButton>
              <ToggleButton value="heat" aria-label="Heat" selected={data.mode === "heat"}>
                <WbSunnyIcon />
              </ToggleButton>
              <ToggleButton value="dry" aria-label="Dry" selected={data.mode === "dry"}>
                <FormatColorResetIcon />
              </ToggleButton>
              <ToggleButton value="air" aria-label="Air" selected={data.mode === "air"}>
                <AirIcon />
              </ToggleButton>
              </ToggleButtonGroup>
            }
            label="Mode"
            labelPlacement='start'
            sx={sxBlockForm}
          />

          <Temperature value={data.temperature} onChange={(value: any) => { updateData({...data!, temperature: parseFloat(""+value)}) }}/>

          <BlockFormControlLabel
            control={
              <ToggleButtonGroup
              exclusive
              aria-label="Flowspeed Mode"
              sx={{ margin: 1 }}
              onChange={(ev, v) => updateData({...data!, flowspeed: parseInt(""+v)}) }
              >
              <ToggleButton value={0x11} aria-label="Automatic" selected={data.flowspeed === 0x11}>
                <HdrAutoOutlinedIcon />
              </ToggleButton>
              <ToggleButton value={0x12} aria-label="Silent" selected={data.flowspeed === 0x12}>
                <HearingDisabledIcon />
              </ToggleButton>
              <ToggleButton value={0x05} aria-label="Manual" selected={data.flowspeed < 0x11}>
                <TuneIcon />
              </ToggleButton>
              </ToggleButtonGroup>
            }
            label="Flow Mode"
            labelPlacement='start'
            sx={sxBlockForm}
          />
          <BlockFormControlLabel
            control={
              <Box sx={{ width: 300 }}>
                  <Slider
                      aria-label="Flow Speed"
                      value={data.flowspeed-2}
                      getAriaValueText={temperatureText}
                      step={1}
                      min={1}
                      max={5}
                      onChangeCommitted={(ev, v) => updateData({...data!, flowspeed: parseInt(""+v)+2})}
                  />
              </Box>
            }
            label="Flow Speed"
            labelPlacement='start'
            sx={{...sxBlockForm, marginTop: 2}}
          />
          <BlockFormControlLabel
            control={
              <Switch
                name="verticalswing"
                disabled={saving}
                checked={data.verticalswing}
                onChange={(ev) => updateData({...data!, verticalswing: ev.target.checked})}
                color="primary"
              />
            }
            label="Vertical Swing"
            labelPlacement='start'
            sx={sxBlockForm}
          />
          <AdvancedRegisters registers={data.registers}/>
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
