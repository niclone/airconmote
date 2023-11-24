import { Box, Slider, Switch, ToggleButton, ToggleButtonGroup } from '@mui/material';
import React from 'react';
import BlockFormControlLabel from './BlockFormControlLabel';

import HdrAutoOutlinedIcon from '@mui/icons-material/HdrAutoOutlined';
import HearingDisabledIcon from '@mui/icons-material/HearingDisabled';
import TuneIcon from '@mui/icons-material/Tune';

type Props = {
    flowspeed: string,
    verticalswing: boolean,
    onChange: ({flowspeed, verticalswing} : {flowspeed?: string, verticalswing?: boolean}) => void,
}

const AirconFlow = (props: Props) => {

    const flowspeedAsInteger = (flowspeed: string) => {
        switch(flowspeed) {
            case "low": return 1;
            case "gentle": return 2;
            case "medium": return 3;
            case "strong": return 4;
            case "high": return 5;
            case "auto": return 0;
            case "quiet": return 0;
            default: return 0;
        }
    };

    const flowspeedAsString = (flowspeed: number) => {
        switch(flowspeed) {
            case 1: return "low";
            case 2: return "gentle";
            case 3: return "medium";
            case 4: return "strong";
            case 5: return "high";
            case 0: return "auto";
            default: return "auto";
        }
    };

    return (
    <>
        <BlockFormControlLabel
            control={
              <ToggleButtonGroup
                exclusive
                aria-label="Flowspeed Mode"
                sx={{ margin: 1 }}
                onChange={(_, v) => props.onChange({flowspeed: v}) }
              >
              <ToggleButton value={"auto"} aria-label="Automatic" selected={props.flowspeed === "auto"}>
                <HdrAutoOutlinedIcon />
              </ToggleButton>
              <ToggleButton value={"quiet"} aria-label="Quiet" selected={props.flowspeed === "quiet"}>
                <HearingDisabledIcon />
              </ToggleButton>
              <ToggleButton value={"medium"} aria-label="Manual" selected={props.flowspeed !== "auto" && props.flowspeed !== "quiet"}>
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
                      disabled={props.flowspeed === "auto" || props.flowspeed === "quiet"}
                      aria-label="Flow Speed"
                      value={flowspeedAsInteger(props.flowspeed)}
                      step={1}
                      min={1}
                      max={5}
                      onChangeCommitted={(_, v) => props.onChange({flowspeed: flowspeedAsString(v as number)})}
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
