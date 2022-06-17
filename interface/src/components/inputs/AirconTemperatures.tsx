import { Box, Grid } from '@mui/material';
import React from 'react';
import HomeIcon from '@mui/icons-material/Home';
import ForestIcon from '@mui/icons-material/Forest';

type Props = {
    insideTemperature: number,
    outsideTemperature: number,
}

const AirconTemperatures = (props: Props) => {
  return (
    <Grid container direction="row" justifyContent="center" alignItems="center" spacing={10}>
        <Grid item>
            <Grid container direction="row" justifyContent="center" alignItems="center" spacing={1}>
                <Grid item><HomeIcon/></Grid>
                <Grid item>{props.insideTemperature}</Grid>
            </Grid>
        </Grid>
        <Grid item>
            <Grid container direction="row" justifyContent="center" alignItems="center" spacing={1}>
                <Grid item><ForestIcon/></Grid>
                <Grid item>{props.insideTemperature}</Grid>
            </Grid>
        </Grid>
    </Grid>
  );
};

export default AirconTemperatures;
