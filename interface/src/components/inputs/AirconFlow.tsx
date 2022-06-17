import { Box, Slider, Switch, ToggleButton, ToggleButtonGroup } from '@mui/material';
import React from 'react';
import BlockFormControlLabel from './BlockFormControlLabel';

import HdrAutoOutlinedIcon from '@mui/icons-material/HdrAutoOutlined';
import HearingDisabledIcon from '@mui/icons-material/HearingDisabled';
import TuneIcon from '@mui/icons-material/Tune';

type Props = {
    flowspeed: number,
    verticalswing: boolean,
    onChange: ({flowspeed, verticalswing} : {flowspeed?: number, verticalswing?: boolean}) => void,
}

const AirconFlow = (props: Props) => {

    return (
    <>
        <BlockFormControlLabel
            control={
              <ToggleButtonGroup
                exclusive
                aria-label="Flowspeed Mode"
                sx={{ margin: 1 }}
                onChange={(_, v) => props.onChange({flowspeed: parseInt(""+v)}) }
              >
              <ToggleButton value={0x11} aria-label="Automatic" selected={props.flowspeed === 0x11}>
                <HdrAutoOutlinedIcon />
              </ToggleButton>
              <ToggleButton value={0x12} aria-label="Silent" selected={props.flowspeed === 0x12}>
                <HearingDisabledIcon />
              </ToggleButton>
              <ToggleButton value={0x05} aria-label="Manual" selected={props.flowspeed < 0x11}>
                <TuneIcon />
              </ToggleButton>
              </ToggleButtonGroup>
            }
            label="Flow Mode"
            labelPlacement='start'
        />

        <BlockFormControlLabel
            control={
              <Box sx={{ width: 240 }}>
                  <Slider
                      aria-label="Flow Speed"
                      value={props.flowspeed-2}
                      step={1}
                      min={1}
                      max={5}
                      onChangeCommitted={(_, v) => props.onChange({flowspeed: parseInt(""+v)+2})}
                  />
              </Box>
            }
            label="Flow Speed"
            labelPlacement='start'
            sx={{marginTop: 2}}
        />

        <BlockFormControlLabel
            control={
              <Switch
                name="verticalswing"
                checked={props.verticalswing}
                onChange={(ev) => props.onChange({verticalswing: ev.target.checked})}
                color="primary"
              />
            }
            label="Vertical Swing"
            labelPlacement='start'
        />
    </>
  );
};

export default AirconFlow;
