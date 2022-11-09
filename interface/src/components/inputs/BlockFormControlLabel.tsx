import { FC } from "react";

import { FormControlLabel, FormControlLabelProps } from "@mui/material";

const BlockFormControlLabel: FC<FormControlLabelProps> = (props) => (
  <div className={"BlockFormControlLabel"}>
    <FormControlLabel {...props} />
  </div>
);

export default BlockFormControlLabel;
