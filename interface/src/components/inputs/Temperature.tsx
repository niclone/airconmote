import React from 'react'
// @ts-ignore
import CircularSlider from '@fseehawer/react-circular-slider';

const temperatures = [
  "10.0°", "10.5°", "11.0°", "11.5°", "12.0°", "12.5°", "13.0°", "13.5°", "14.0°", "14.5°",
  "15.0°", "15.5°", "16.0°", "16.5°", "17.0°", "17.5°", "18.0°", "18.5°", "19.0°", "19.5°",
  "20.0°", "20.5°", "21.0°", "21.5°", "22.0°", "22.5°", "23.0°", "23.5°", "24.0°", "24.5°",
  "25.0°", "25.5°", "26.0°", "26.5°", "27.0°", "27.5°", "28.0°", "28.5°", "29.0°", "29.5°",
  "30.0°"
];

type Props = {
    value: number,
    onChange: (temperature: number) => void,
}

export default function Temperature({ value, onChange}: Props) {
  return (
    <>
        <CircularSlider
            label="temperature"
            labelColor="#005a58"
            knobColor="#005a58"
            knobPosition="bottom"
            progressColorFrom="#00ffff"
            progressColorTo="#ff77ff"
            progressSize={34}
            trackColor="#eeeeee"
            trackSize={36}
            //data={["1€","2€"]} //...
            data={temperatures}
            //appendToValue={"°"}
            //min={10}
            //max={30}
            dataIndex={ temperatures.findIndex(v => parseFloat(v)===value) }
            labelFontSize="1.5rem"
            valueFontSize="4rem"
            onChange={ (v: any) => { onChange(parseFloat(""+v)) } }
        />
    </>
  )
}