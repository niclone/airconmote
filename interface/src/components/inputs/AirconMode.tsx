import React from 'react';
import { FormControlLabel, ToggleButton, ToggleButtonGroup } from '@mui/material';
import BlockFormControlLabel from './BlockFormControlLabel';

import ThermostatAutoIcon from '@mui/icons-material/ThermostatAuto';
import AcUnitIcon from '@mui/icons-material/AcUnit';
import WbSunnyIcon from '@mui/icons-material/WbSunny';
import FormatColorResetIcon from '@mui/icons-material/FormatColorReset';
import AirIcon from '@mui/icons-material/Air';

type Props = {
    mode: string,
    onChange: (mode: string) => void,
}

const AirconMode = (props: Props) => {
    return (
        <BlockFormControlLabel
            control={
                <ToggleButtonGroup
                    exclusive
                    aria-label="AirCon Mode"
                    //sx={{ margin: 1, flexFlow: 1 }}
                    onChange={(_, v) => props.onChange(v) }
                >
                    <ToggleButton value="auto" aria-label="Automatic" selected={props.mode === "auto"}>
                        <ThermostatAutoIcon />
                    </ToggleButton>
                    <ToggleButton value="cool" aria-label="Air Conditionned" selected={props.mode === "cool"}>
                        <AcUnitIcon />
                    </ToggleButton>
                    <ToggleButton value="heat" aria-label="Heat" selected={props.mode === "heat"}>
                        <WbSunnyIcon />
                    </ToggleButton>
                    <ToggleButton value="dry" aria-label="Dry" selected={props.mode === "dry"}>
                        <FormatColorResetIcon />
                    </ToggleButton>
                    <ToggleButton value="fan_only" aria-label="Fan Only" selected={props.mode === "fan_only"}>
                        <AirIcon />
                    </ToggleButton>
                </ToggleButtonGroup>
            }
            label="Mode"
            labelPlacement='start'
            sx={{ flex: 1, flexGrow: 1}}
        />
    );
};

export default AirconMode;
