import React from 'react';
import { Switch } from '@mui/material';
import BlockFormControlLabel from './BlockFormControlLabel';

type Props = {
    power: boolean,
    onChange: (power: boolean) => void,
}

const AirconPower = (props: Props) => {
  return (
    <BlockFormControlLabel
        control={
            <Switch
                name="power"
                checked={props.power}
                onChange={(ev) => props.onChange(ev.target.checked)}
                color="primary"
            />
        }
        label="Power"
        labelPlacement='start'
    />
  );
};

export default AirconPower;
