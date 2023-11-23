import { FC, useEffect, useState } from 'react';

import { Switch, Slider, Box, ToggleButton, ToggleButtonGroup } from '@mui/material';
import Accordion from '@mui/material/Accordion';
import AccordionSummary from '@mui/material/AccordionSummary';
import AccordionDetails from '@mui/material/AccordionDetails';
import Typography from '@mui/material/Typography';
import ExpandMoreIcon from '@mui/icons-material/ExpandMore';
import Grid from "@mui/material/Grid";

import { WEB_SOCKET_ROOT } from '../api/endpoints';
import { FormLoader, MessageBox, SectionContent } from '../components';
import AirconTemperature from '../components/inputs/AirconTemperature';
import AdvancedRegisters from '../components/advanced/AdvancedRegisters';
import { updateValue, useWs } from '../utils';

import './AirConStateWebSocketForm.css';

import { AirConState } from './types';
import { MapsHomeWork } from '@mui/icons-material';
import AirconMode from '../components/inputs/AirconMode';
import AirconPower from '../components/inputs/AirconPower';
import AirconFlow from '../components/inputs/AirconFlow';
import AirconTemperatures from '../components/inputs/AirconTemperatures';

export const AIRCON_SETTINGS_WEBSOCKET_URL = WEB_SOCKET_ROOT + "airConState";

const AirConStateWebSocketForm: FC = () => {
  const { connected, updateData, data } = useWs<AirConState>(AIRCON_SETTINGS_WEBSOCKET_URL, 1000);

  const debug=data?.registers ? true : false;

  const content = () => {
    if (!connected || !data) {
      return (<FormLoader message="Connecting to WebSocket…" />);
    }

    return (
      <Box sx={{ flexGrow: 1 }}>
        <Grid
          container
          spacing={{xs: 2, md: 3 }}
          columns={{xs: 1, sm: 2, md: 2 }}
        >

          <Grid item xs={1} sm={1} md={1}>
            <AirconPower onoff={data.onoff} onChange={(onoff: boolean) => updateData({...data!, onoff})} />

            <AirconTemperatures insideTemperature={data.sensor_temp_inside} outsideTemperature={data.sensor_temp_outside} />

            <AirconMode mode={data.mode} onChange={(v: string) => updateData({...data!, mode: v})} />

            <AirconTemperature mode={data.mode} value={data.temperature} onChange={(value: number) => { updateData({...data!, temperature: value }) }} />

            <AirconFlow
              flowspeed={data.flowspeed}
              verticalswing={data.verticalswing}
              onChange={(params: object) => updateData({...data!, ...params})}
            />
          </Grid>

          {debug && (
            <Grid item xs={1} sm={1} md={1}>
              <Accordion>
                <AccordionSummary
                  expandIcon={<ExpandMoreIcon />}
                  aria-controls="panel1a-content"
                  id="panel1a-header"
                >
                  <Typography>Advanced debug registers</Typography>
                </AccordionSummary>
                <AccordionDetails>
                  <AdvancedRegisters registers={data.registers}/>
                </AccordionDetails>
              </Accordion>
            </Grid>
          )}
        </Grid>
      </Box>
    );
  };

  return content();
  /*
  return (
    <SectionContent title='WebSocket Example' titleGutter>
      {content()}
    </SectionContent>
  );
  */
};

export default AirConStateWebSocketForm;
