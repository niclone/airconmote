import { FC, useState } from 'react';

import { Button, Switch, Slider, Box, Typography } from '@mui/material';
import SaveIcon from '@mui/icons-material/Save';

import ThermostatAutoIcon from '@mui/icons-material/ThermostatAuto';
import AcUnitIcon from '@mui/icons-material/AcUnit';
import WbSunnyIcon from '@mui/icons-material/WbSunny';
import FormatColorResetIcon from '@mui/icons-material/FormatColorReset';
import AirIcon from '@mui/icons-material/Air';

import HdrAutoOutlinedIcon from '@mui/icons-material/HdrAutoOutlined';
import HearingDisabledIcon from '@mui/icons-material/HearingDisabled';
import TuneIcon from '@mui/icons-material/Tune';

import ToggleButton from '@mui/material/ToggleButton';
import ToggleButtonGroup from '@mui/material/ToggleButtonGroup';

import { SectionContent, FormLoader, BlockFormControlLabel, ButtonRow, MessageBox } from '../components';
import { updateValue, useRest } from '../utils';

import * as AirConApi from './api';
import { AirConState } from './types';
import { NfcTwoTone } from '@mui/icons-material';

const AirConStateRestForm: FC = () => {
  const {
    loadData, saveData, saving, setData, data, errorMessage
  } = useRest<AirConState>({ read: AirConApi.readAirConState, update: AirConApi.updateAirConState });

  const myUpdateData = (newconf: object) => {
    let newdata = { ...data!, ...newconf};
    AirConApi.updateAirConState(newdata).then(answer => {
        setData(answer.data);
    });
  };

  const updateFormValue = updateValue(setData);
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

  function valuetext(value: number) {
    return `${value}°C`;
  }

  const content = () => {
    if (!data) {
      return (<FormLoader onRetry={loadData} errorMessage={errorMessage} />);
    }
    console.log("data for content : ", data);

    return (
      <>
        <BlockFormControlLabel
          control={
            <Switch
              name="onoff"
              disabled={saving}
              checked={data.onoff}
              onChange={(ev) => myUpdateData({...data, onoff: ev.target.checked})}
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
            onChange={(ev, v) => myUpdateData({...data, mode: ""+v}) }
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
        <BlockFormControlLabel
          control={
            <Box sx={{ width: 300 }}>
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
                    onChangeCommitted={(ev, v) => myUpdateData({...data, temperature: parseFloat(""+v)})}
                />
            </Box>
          }
          label="Temperature"
          labelPlacement='start'
          sx={{...sxBlockForm, marginTop: 5}}
        />
        <BlockFormControlLabel
          control={
            <ToggleButtonGroup
            exclusive
            aria-label="Flowspeed Mode"
            sx={{ margin: 1 }}
            onChange={(ev, v) => myUpdateData({...data, flowspeed: parseInt(""+v)}) }
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
                    defaultValue={data.flowspeed-2}
                    getAriaValueText={valuetext}
                    step={1}
                    min={1}
                    max={5}
                    onChangeCommitted={(ev, v) => myUpdateData({...data, flowspeed: parseInt(""+v)+2})}
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
              onChange={(ev) => myUpdateData({...data, verticalswing: ev.target.checked})}
              color="primary"
            />
          }
          label="Vertical Swing"
          labelPlacement='start'
          sx={sxBlockForm}
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
