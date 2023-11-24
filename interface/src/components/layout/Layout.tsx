
import { FC, useEffect, useState } from 'react';

import { Box, Toolbar } from '@mui/material';

import { PROJECT_NAME } from '../../api/env';
import LayoutDrawer from './LayoutDrawer';
import LayoutAppBar from './LayoutAppBar';
import { LayoutContext } from './context';

// for the title
import * as WiFiApi from "../../api/wifi";
import { WiFiSettings } from '../../types';
import { useRest } from '../../utils';
import { error } from 'console';

export const DRAWER_WIDTH = 280;

const Layout: FC = ({ children }) => {

  const [mobileOpen, setMobileOpen] = useState(false);
  const [title, setTitle] = useState(PROJECT_NAME);
  const [longTitle, setLongTitle] = useState(PROJECT_NAME);

  const {
    loadData, data, errorMessage
  } = useRest<WiFiSettings>({ read: WiFiApi.readWiFiSettings, update: WiFiApi.updateWiFiSettings });

  useEffect(() => {
    if (data && data.hostname && !errorMessage)
        setLongTitle(data.hostname + ": " + title);
    else if (errorMessage)
        setLongTitle(title + ": " + errorMessage);
    else
        setLongTitle(title);
  }, [data, errorMessage, title]);

  const handleDrawerToggle = () => {
    setMobileOpen(!mobileOpen);
  };

  return (
    <LayoutContext.Provider value={{ title, setTitle }}>
      <Box sx={{ display: 'flex' }}>
        <LayoutAppBar title={longTitle} onToggleDrawer={handleDrawerToggle} />
        <LayoutDrawer mobileOpen={mobileOpen} onClose={handleDrawerToggle} />
        <Box
          component="main"
          sx={{ flexGrow: 1, width: { md: `calc(100% - ${DRAWER_WIDTH}px)` } }}
        >
          <Toolbar />
          {children}
        </Box>
      </Box>
    </LayoutContext.Provider >
  );

};

export default Layout;
