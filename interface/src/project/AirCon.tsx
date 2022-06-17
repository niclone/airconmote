
import React, { FC } from 'react';
import { Navigate, Route, Routes } from 'react-router-dom';

import { useLayoutTitle } from '../components';

import AirConStateRestForm from './AirConStateRestForm';
import AirConMqttSettingsForm from './AirConMqttSettingsForm';
import AirConStateWebSocketForm from './AirConStateWebSocketForm';

const AirConProject: FC = () => {
  useLayoutTitle("AirCon");

  return (
    <>
      <Routes>
        <Route path="socket" element={<AirConStateWebSocketForm />} />
        <Route path="mqtt" element={<AirConMqttSettingsForm />} />
        <Route path="rest" element={<AirConStateRestForm />} />
        <Route path="/*" element={<Navigate replace to="socket" />} />
      </Routes>
    </>
  );
};

export default AirConProject;
