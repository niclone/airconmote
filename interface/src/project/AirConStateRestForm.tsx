import { FC, useState } from 'react';

import { Button, Switch, Slider, Box, TextField } from '@mui/material';
import SaveIcon from '@mui/icons-material/Save';

import AcUnitIcon from '@mui/icons-material/AcUnit';
import WbSunnyIcon from '@mui/icons-material/WbSunny';
import FormatColorResetIcon from '@mui/icons-material/FormatColorReset';
import AirIcon from '@mui/icons-material/Air';

import ToggleButton from '@mui/material/ToggleButton';
import ToggleButtonGroup from '@mui/material/ToggleButtonGroup';

import { SectionContent, FormLoader, BlockFormControlLabel, ButtonRow, MessageBox } from '../components';
import { updateValue, useRest } from '../utils';

import * as AirConApi from './api';
import { AirConState } from './types';

const AirConStateRestForm: FC = () => {
  const {
    loadData, saveData, saving, setData, data, errorMessage
  } = useRest<AirConState>({ read: AirConApi.readAirConState, update: AirConApi.updateAirConState });

  const updateFormValue = updateValue(setData);
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

  function valuetext(value: number) {
    return `${value}°C`;
  }

  const content = () => {
    if (!data) {
      return (<FormLoader onRetry={loadData} errorMessage={errorMessage} />);
    }
    console.log("data : ", data);

    return (
      <>
        <MessageBox
          level="info"
          message="The form below controls the LED via the RESTful service exposed by the ESP device."
          my={2}
        />
        <BlockFormControlLabel
          control={
            <Switch
              name="onoff"
              disabled={saving}
              checked={data.onoff}
              onChange={(ev) => {
                AirConApi.updateAirConState({...data, onoff: ev.target.checked});
                setData({...data, onoff: ev.target.checked});
              }}
              color="primary"
            />
          }
          label="Power"
        />
        <BlockFormControlLabel
          control={
            <ToggleButtonGroup
            exclusive
            aria-label="AirCon Mode"
            sx={{ margin: 10 }}
            onChange={(ev, v) => { AirConApi.updateAirConState({...data, mode: ""+v}); setData({...data, mode: ""+v}); }}
            >
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
        />
        <BlockFormControlLabel
          control={
            <Box sx={{ width: 300, margin: 10 }}>
                <Slider
                    name="temperature"
                    disabled={saving}
                    aria-label="Temperature"
                    defaultValue={data.temperature}
                    getAriaValueText={valuetext}
                    step={0.5}
                    marks={temperatureMarks}
                    valueLabelDisplay="on"
                    min={10}
                    max={30}
                    //onChange={(ev, v) => AirConApi.updateAirConState({temperature: parseFloat(""+v), mode:"", flowspeed: 0})}
                    onChangeCommitted={(ev, v) => AirConApi.updateAirConState({...data, temperature: parseFloat(""+v)})}
                />
            </Box>
          }
          label="Temperature"
        />
        <BlockFormControlLabel
          control={
            <Box sx={{ width: 300, margin: 10 }}>
                <Slider
                    aria-label="Flow Speed"
                    defaultValue={data.flowspeed}
                    valueLabelDisplay="on"
                    getAriaValueText={valuetext}
                    step={1}
                    min={1}
                    max={5}
                    onChangeCommitted={(ev, v) => AirConApi.updateAirConState({...data, flowspeed: parseFloat(""+v)})}
                />
            </Box>
          }
          label="Flow Speed"
        />
      </>
    );
  };

  return (
    <SectionContent title='REST Example' titleGutter>
      {content()}
    </SectionContent>
  );
};

export default AirConStateRestForm;
