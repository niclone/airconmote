import { FC } from 'react';

import { List } from '@mui/material';
import SettingsRemoteIcon from '@mui/icons-material/SettingsRemote';

import { PROJECT_PATH } from '../api/env';
import LayoutMenuItem from '../components/layout/LayoutMenuItem';

const ProjectMenu: FC = () => (
  <List>
    <LayoutMenuItem icon={SettingsRemoteIcon} label="AirCon" to={`/${PROJECT_PATH}/airCon`} />
    <LayoutMenuItem icon={SettingsRemoteIcon} label="AirCon (rest)" to={`/${PROJECT_PATH}/airCon/rest`} />
    <LayoutMenuItem icon={SettingsRemoteIcon} label="AirCon (MQTT)" to={`/${PROJECT_PATH}/airCon/mqtt`} />
  </List>
);

export default ProjectMenu;
