import React, { useState } from 'react'

type Props = {
    registers: Array<Array<number>>
}

const AdvancedRegisters = (props: Props) => {
    const [ oldRegisters, setOldRegisters ] = useState<number[][]>([]);

    const registers = props.registers ? props.registers : [];

    if (registers.length > 0 && oldRegisters.length === 0) {
        setOldRegisters(registers);
    }

    let origRegisters = registers.map((row, regNum) => {
        //let changed = false;
        let oldRow = oldRegisters[regNum];
        if (JSON.stringify(row) !== JSON.stringify(oldRow)) {
          return oldRow;
          //oldRegisters[regNum]=row;
          //changed=true;
        } else {
          return [];
        }
        //if (changed) setOldRegisters(oldRegisters);
    });

    return (
        <table className={"registers"}>
            <tbody>
                {registers.map((row, registerNum) => { return (
                    <tr key={registerNum}>
                        <th>{registerNum}</th>
                        <td>{row[0]}</td>
                        <td>{row[1]}</td>
                        <td>{row[2]}</td>
                        <td>{row[3]}</td>
                        <td>{row[4]}</td>
                        <th> original: </th>
                        <td>{origRegisters[registerNum] ? origRegisters[registerNum][0] : ''}</td>
                        <td>{origRegisters[registerNum] ? origRegisters[registerNum][1] : ''}</td>
                        <td>{origRegisters[registerNum] ? origRegisters[registerNum][2] : ''}</td>
                        <td>{origRegisters[registerNum] ? origRegisters[registerNum][3] : ''}</td>
                        <td>{origRegisters[registerNum] ? origRegisters[registerNum][4] : ''}</td>
                    </tr>
                )})}
            </tbody>
        </table>
    )
}

export default AdvancedRegisters