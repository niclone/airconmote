import { FC, useEffect, useState } from 'react';

import { Switch, Slider, Box, ToggleButton, ToggleButtonGroup } from '@mui/material';

// @ts-ignore
import CircularSlider from '@fseehawer/react-circular-slider';

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
import { updateValue, useWs } from '../utils';
//import ToggleButton from '@mui/material/ToggleButton';
//import ToggleButtonGroup from '@mui/material/ToggleButtonGroup';

import './AirConStateWebSocketForm.css';

import { AirConState } from './types';
import { MapsHomeWork } from '@mui/icons-material';

export const AIRCON_SETTINGS_WEBSOCKET_URL = WEB_SOCKET_ROOT + "airConState";

const AirConStateWebSocketForm: FC = () => {
  const { connected, updateData, data } = useWs<AirConState>(AIRCON_SETTINGS_WEBSOCKET_URL);
  const [ oldRegisters, setOldRegisters ] = useState<number[][]>([]);
  const saving = false; // hack

  const registers = data && data.registers ? data.registers : [];

  if (registers.length > 0 && oldRegisters.length === 0) {
    setOldRegisters(registers);
  }

  let origRegisters = registers.map((row, regNum) => {
    //let changed = false;
    let oldRow = oldRegisters[regNum];
    if (JSON.stringify(row) !== JSON.stringify(oldRow)) {
      return oldRow;
      //oldRegisters[regNum]=row;
      //changed=true;
    } else {
      return [];
    }
    //if (changed) setOldRegisters(oldRegisters);
  });


  const updateFormValue = updateValue(updateData);
  const sxBlockForm={
    ".MuiFormControlLabel-label": {
        width: 125,
    },
  };

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

  const temperatures = [
    "10.0°", "10.5°", "11.0°", "11.5°", "12.0°", "12.5°", "13.0°", "13.5°", "14.0°", "14.5°",
    "15.0°", "15.5°", "16.0°", "16.5°", "17.0°", "17.5°", "18.0°", "18.5°", "19.0°", "19.5°",
    "20.0°", "20.5°", "21.0°", "21.5°", "22.0°", "22.5°", "23.0°", "23.5°", "24.0°", "24.5°",
    "25.0°", "25.5°", "26.0°", "26.5°", "27.0°", "27.5°", "28.0°", "28.5°", "29.0°", "29.5°",
    "30.0°"
  ];

  function temperatureText(value: number) {
    return `${value}°C`;
  }

  const content = () => {
    if (!connected || !data) {
      return (<FormLoader message="Connecting to WebSocket…" />);
    }

    let registersHtml = (
        <table className={"registers"}>
            <tbody>
                {data.registers.map((row, registerNum) => { return (
                    <tr key={registerNum}>
                        <th>{registerNum}</th>
                        <td>{row[0]}</td>
                        <td>{row[1]}</td>
                        <td>{row[2]}</td>
                        <td>{row[3]}</td>
                        <td>{row[4]}</td>
                        <th> original: </th>
                        <td>{origRegisters[registerNum] ? origRegisters[registerNum][0] : ''}</td>
                        <td>{origRegisters[registerNum] ? origRegisters[registerNum][1] : ''}</td>
                        <td>{origRegisters[registerNum] ? origRegisters[registerNum][2] : ''}</td>
                        <td>{origRegisters[registerNum] ? origRegisters[registerNum][3] : ''}</td>
                        <td>{origRegisters[registerNum] ? origRegisters[registerNum][4] : ''}</td>
                    </tr>
                )})}
            </tbody>
        </table>
    )

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
          <CircularSlider
            label="temperature"
            labelColor="#005a58"
            knobColor="#005a58"
            knobPosition="bottom"
            progressColorFrom="#00ffff"
            progressColorTo="#ff77ff"
            progressSize={34}
            trackColor="#eeeeee"
            trackSize={36}
            //data={["1€","2€"]} //...
            data={temperatures}
            //appendToValue={"°"}
            //min={10}
            //max={30}
            dataIndex={ temperatures.findIndex(v => parseFloat(v)==data.temperature) }
            labelFontSize="1.5rem"
            valueFontSize="4rem"
            onChange={ (value: any) => { updateData({...data!, temperature: parseFloat(""+value)}) } }
        />
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
          {registersHtml}
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
