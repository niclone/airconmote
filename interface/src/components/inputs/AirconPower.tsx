import React from 'react'
import { Switch } from '@mui/material';
import BlockFormControlLabel from './BlockFormControlLabel';

type Props = {
    onoff: boolean,
    onChange: (onoff: boolean) => void,
}

const AirconPower = (props: Props) => {
  return (
    <BlockFormControlLabel
    control={
      <Switch
        name="onoff"
        checked={props.onoff}
        onChange={(ev) => props.onChange(ev.target.checked)}
        color="primary"
      />
    }
    label="Power"
    labelPlacement='start'
  />
  )
}

export default AirconPower;