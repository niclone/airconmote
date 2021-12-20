
import React, { FC } from 'react';
import { Navigate, Route, Routes } from 'react-router-dom';

import { Tab } from '@mui/material';

import { RouterTabs, useRouterTab, useLayoutTitle } from '../components';

import AirConStateRestForm from './AirConStateRestForm';
import AirConMqttSettingsForm from './AirConMqttSettingsForm';
import AirConStateWebSocketForm from './AirConStateWebSocketForm';

const AirConProject: FC = () => {
  useLayoutTitle("AirCon");
  const { routerTab } = useRouterTab();

  return (
    <>
      <RouterTabs value={routerTab}>
        <Tab value="rest" label="REST Example" />
        <Tab value="socket" label="WebSocket Example" />
        <Tab value="mqtt" label="MQTT Settings" />
      </RouterTabs>
      <Routes>
        <Route path="rest" element={<AirConStateRestForm />} />
        <Route path="mqtt" element={<AirConMqttSettingsForm />} />
        <Route path="socket" element={<AirConStateWebSocketForm />} />
        <Route path="/*" element={<Navigate replace to="rest" />} />
      </Routes>
    </>
  );
};

export default AirConProject;
